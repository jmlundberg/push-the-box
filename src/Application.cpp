#include "Application.h"

#include <Corrade/Containers/Array.h>
#include <Corrade/Utility/Resource.h>
#include <Magnum/AbstractShaderProgram.h>
#include <Magnum/DefaultFramebuffer.h>
#include <Magnum/Renderer.h>
#include <Magnum/Buffer.h>
#include <Magnum/Mesh.h>
#include <Magnum/MeshTools/FullScreenTriangle.h>
#include <Magnum/Shaders/DistanceFieldVector.h>
#include <Magnum/Text/DistanceFieldGlyphCache.h>
#include <Magnum/Text/AbstractFont.h>
#include <Magnum/Trade/AbstractImporter.h>

#ifdef MAGNUM_BUILD_STATIC
#include <Magnum/Shaders/magnumShadersResourceImport.hpp>
#include <Magnum/TextureTools/magnumTextureToolsResourceImport.hpp>
#endif

#include "Game/Game.h"
#include "Menu/Menu.h"
#include "Splash/Splash.h"
#include "configure.h"

#if defined(CORRADE_TARGET_NACL_NEWLIB) || defined(CORRADE_TARGET_EMSCRIPTEN)
static int importStaticPlugins() {
    CORRADE_PLUGIN_IMPORT(MagnumFont)
    CORRADE_PLUGIN_IMPORT(TgaImporter)
    return 0;
} CORRADE_AUTOMATIC_INITIALIZER(importStaticPlugins)
#endif

namespace PushTheBox {

Application* Application::_instance = nullptr;

Application* Application::instance() {
    CORRADE_INTERNAL_ASSERT(_instance);
    return _instance;
}

Application::Application(const Arguments& arguments): Platform::ScreenedApplication(arguments, nullptr), importerPluginManager(MAGNUM_PLUGINS_IMPORTER_DIR), fontPluginManager(MAGNUM_PLUGINS_FONT_DIR)
{
    CORRADE_INTERNAL_ASSERT(!_instance);
    _instance = this;

    /* Try to create MSAA context, fall back to no-AA */
    Configuration conf;
    conf.setSampleCount(16);
    #ifndef CORRADE_TARGET_NACL
    conf.setTitle("Push The Box");
    #endif
    if(!tryCreateContext(conf)) {
        Debug() << "Cannot create 16x MSAA context, fallback to no antialiasing";
        createContext(conf.setSampleCount(0));
    }

    Renderer::setFeature(Renderer::Feature::FaceCulling, true);
    Renderer::setFeature(Renderer::Feature::DepthTest, true);
    Renderer::setClearColor(Color3(0.0f));

    /* Add resource loader and fallback meshes */
    sceneResourceManager.setLoader(&meshResourceLoader);
    sceneResourceManager.setFallback<Mesh>(new Mesh);

    /* TGA importer */
    Trade::AbstractImporter* tgaImporter;
    if(importerPluginManager.load("TgaImporter") & PluginManager::LoadState::Loaded)
        CORRADE_INTERNAL_ASSERT_OUTPUT(tgaImporter = importerPluginManager.instance("TgaImporter").release());
    else {
        Error() << "Cannot open TGA importer plugin";
        std::exit(1);
    }

    /* Save full screen triangle and TGA importer to resource manager */
    std::pair<std::unique_ptr<Buffer>, Mesh> triangle = MeshTools::fullScreenTriangle();
    if(triangle.first) SceneResourceManager::instance().set("fullscreentriangle", std::move(triangle.first.release()));
    SceneResourceManager::instance()
        .set("fullscreentriangle", std::move(triangle.second))
        .set("tga-importer", tgaImporter);

    /* Font plugin */
    std::unique_ptr<Text::AbstractFont> font;
    if(fontPluginManager.load("MagnumFont") & PluginManager::LoadState::Loaded)
        CORRADE_INTERNAL_ASSERT_OUTPUT(font = fontPluginManager.instance("MagnumFont"));
    else {
        Error() << "Cannot open font plugin";
        std::exit(1);
    }

    /* Load font and create glyph cache */
    Utility::Resource rs("PushTheBoxResources");
    font->openData(std::vector<std::pair<std::string, Containers::ArrayReference<const unsigned char>>>{
        {"luckiest-guy.conf", rs.getRaw("luckiest-guy.conf")},
        {"luckiest-guy.tga",  rs.getRaw("luckiest-guy.tga")}}, 0.0f);
    std::unique_ptr<Text::GlyphCache> cache = font->createGlyphCache();

    /* Save font resources to resource manager */
    SceneResourceManager::instance().set<AbstractShaderProgram>("text2d", new Shaders::DistanceFieldVector2D)
        .set("font", font.release()).set("cache", cache.release());

    /* Add the screens */
    _gameScreen = new Game::Game;
    _menuScreen = new Menu::Menu;
    _splashScreen = new Splash::Splash;
    addScreen(*_splashScreen);
    addScreen(*_gameScreen);
    addScreen(*_menuScreen);

    _timeline.start();
}

Application::~Application() {
    CORRADE_INTERNAL_ASSERT(_instance == this);

    /* Remove all screens before deleting the resource manager, so the
       resources can be properly freed */
    while(backScreen()) removeScreen(*backScreen());
}

void Application::globalViewportEvent(const Vector2i& size) {
    defaultFramebuffer.setViewport({{}, size});
}

void Application::globalDrawEvent() {
    swapBuffers();
    _timeline.nextFrame();
}

}

MAGNUM_APPLICATION_MAIN(PushTheBox::Application)
