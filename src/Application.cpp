#include "Application.h"

#include <Utility/Resource.h>
#include <AbstractShaderProgram.h>
#include <DefaultFramebuffer.h>
#include <Renderer.h>
#include <Mesh.h>
#include <Shaders/DistanceFieldVectorShader.h>
#include <Text/DistanceFieldGlyphCache.h>
#include <Text/AbstractFont.h>

#include "Game/Game.h"
#include "Menu/Menu.h"
#include "configure.h"

using Corrade::PluginManager::LoadState;

namespace PushTheBox {

Application* Application::_instance = nullptr;

Application* Application::instance() {
    CORRADE_INTERNAL_ASSERT(_instance);
    return _instance;
}

#ifndef MAGNUM_TARGET_NACL
Application::Application(int argc, char** argv): AbstractScreenedApplication(argc, argv, (new Configuration())
    ->setTitle("Push The Box")
    ->setSampleCount(16)),
#else
Application::Application(PP_Instance instance): AbstractScreenedApplication(instance),
#endif
        fontPluginManager(MAGNUM_PLUGINS_FONT_DIR)
{
    CORRADE_INTERNAL_ASSERT(!_instance);
    _instance = this;

    Renderer::setFeature(Renderer::Feature::FaceCulling, true);
    Renderer::setFeature(Renderer::Feature::DepthTest, true);
    Renderer::setClearColor(Color3<>(0.0f));

    /* Add resource loader and fallback meshes */
    sceneResourceManager.setLoader(&meshResourceLoader);
    sceneResourceManager.setFallback<Mesh>(new Mesh);

    /* Font plugin -- try HarfBuzz or FreeType as fallback */
    Text::AbstractFont* font;
    if(fontPluginManager.load("HarfBuzzFont") & (LoadState::Loaded|LoadState::Static))
        CORRADE_INTERNAL_ASSERT_OUTPUT(font = fontPluginManager.instance("HarfBuzzFont"));
    else if(fontPluginManager.load("FreeTypeFont") & (LoadState::Loaded|LoadState::Static))
        CORRADE_INTERNAL_ASSERT_OUTPUT(font = fontPluginManager.instance("FreeTypeFont"));
    else {
        Error() << "Cannot open any font plugin";
        std::exit(1);
    }

    /* Load font and create glyph cache */
    Corrade::Utility::Resource rs("PushTheBoxData");
    const unsigned char* fontData;
    std::size_t fontSize;
    std::tie(fontData, fontSize) = rs.getRaw("luckiest-guy.ttf");
    font->open(fontData, fontSize, 128.0f);
    Text::GlyphCache* cache = new Text::DistanceFieldGlyphCache(Vector2i(1536), Vector2i(256), 24);
    font->createGlyphCache(cache, "abcdefghijklmnopqrstuvwxyz0123456789 ");

    /* Save font resources to resource manager */
    SceneResourceManager::instance()->set<AbstractShaderProgram>("text2d", new Shaders::DistanceFieldVectorShader2D);
    /** @todo No need to have manual policy when plugin is unloaded automatically */
    SceneResourceManager::instance()->set("font", font, ResourceDataState::Final, ResourcePolicy::Manual);
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

    /** @todo fix this in PluginManager */
    sceneResourceManager.free<Text::AbstractFont>();
    fontPluginManager.unload("HarfBuzzFont");
}

void Application::viewportEvent(const Vector2i& size) {
    defaultFramebuffer.setViewport({{}, size});

    AbstractScreenedApplication::viewportEvent(size);
}

void Application::drawEvent() {
    defaultFramebuffer.clear(AbstractFramebuffer::Clear::Color|AbstractFramebuffer::Clear::Depth);

    AbstractScreenedApplication::drawEvent();

    swapBuffers();
    _timeline.nextFrame();
}

}

MAGNUM_APPLICATION_MAIN(PushTheBox::Application)
