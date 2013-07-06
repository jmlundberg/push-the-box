#include "Application.h"

#include <Containers/Array.h>
#include <Utility/Resource.h>
#include <AbstractShaderProgram.h>
#include <DefaultFramebuffer.h>
#include <Renderer.h>
#include <Mesh.h>
#include <Shaders/DistanceFieldVector.h>
#include <Text/DistanceFieldGlyphCache.h>
#include <Text/AbstractFont.h>
#include <Trade/AbstractImporter.h>

#ifdef MAGNUM_BUILD_STATIC
#include <Shaders/magnumShadersResourceImport.hpp>
#include <TextureTools/magnumTextureToolsResourceImport.hpp>
#endif

#include "Game/Game.h"
#include "Menu/Menu.h"
#include "configure.h"

#ifdef CORRADE_TARGET_NACL_NEWLIB
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

Application::Application(const Arguments& arguments): AbstractScreenedApplication(arguments, Configuration()
    #ifndef CORRADE_TARGET_NACL
    .setTitle("Push The Box")
    .setSampleCount(16)
    #endif
    ), importerPluginManager(MAGNUM_PLUGINS_IMPORTER_DIR), fontPluginManager(MAGNUM_PLUGINS_FONT_DIR)
{
    CORRADE_INTERNAL_ASSERT(!_instance);
    _instance = this;

    Renderer::setFeature(Renderer::Feature::FaceCulling, true);
    Renderer::setFeature(Renderer::Feature::DepthTest, true);
    Renderer::setClearColor(Color3(0.0f));

    /* Add resource loader and fallback meshes */
    sceneResourceManager.setLoader(&meshResourceLoader);
    sceneResourceManager.setFallback<Mesh>(new Mesh);

    /* Font plugin -- try HarfBuzz or FreeType as fallback */
    Text::AbstractFont* font;
    #ifndef CORRADE_TARGET_NACL_NEWLIB
    const PluginManager::LoadStates loaded = (PluginManager::LoadState::Loaded|PluginManager::LoadState::Static);
    #else
    const PluginManager::LoadStates loaded = PluginManager::LoadState::Static;
    #endif
    if(fontPluginManager.load("MagnumFont") & loaded)
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
    Text::GlyphCache* cache = font->createGlyphCache();

    /* Save font resources to resource manager */
    SceneResourceManager::instance()->set<AbstractShaderProgram>("text2d", new Shaders::DistanceFieldVector2D);
    SceneResourceManager::instance()->set("font", font);
    SceneResourceManager::instance()->set("cache", cache);

    /* Add the screens */
    _gameScreen = new Game::Game;
    _menuScreen = new Menu::Menu;
    addScreen(_menuScreen);
    addScreen(_gameScreen);

    _timeline.start();
}

Application::~Application() {
    CORRADE_INTERNAL_ASSERT(_instance == this);

    /* Remove all screens before deleting the resource manager, so the
       resources can be properly freed */
    while(backScreen()) removeScreen(backScreen());
}

void Application::viewportEvent(const Vector2i& size) {
    defaultFramebuffer.setViewport({{}, size});

    AbstractScreenedApplication::viewportEvent(size);
}

void Application::drawEvent() {
    defaultFramebuffer.clear(FramebufferClear::Color|FramebufferClear::Depth);

    AbstractScreenedApplication::drawEvent();

    swapBuffers();
    _timeline.nextFrame();
}

}

MAGNUM_APPLICATION_MAIN(PushTheBox::Application)
