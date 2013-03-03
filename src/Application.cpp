#include "Application.h"

#include <Utility/Resource.h>
#include <AbstractShaderProgram.h>
#include <DefaultFramebuffer.h>
#include <Renderer.h>
#include <Mesh.h>
#include <Shaders/DistanceFieldVectorShader.h>
#include <Text/Font.h>

#include "Game/Game.h"
#include "Menu/Menu.h"

namespace PushTheBox {

Application* Application::_instance = nullptr;

Application* Application::instance() {
    CORRADE_INTERNAL_ASSERT(_instance);
    return _instance;
}

#ifndef MAGNUM_TARGET_NACL
Application::Application(int argc, char** argv): AbstractScreenedApplication(argc, argv, "Push The Box") {
#else
Application::Application(PP_Instance instance): AbstractScreenedApplication(instance) {
#endif
    CORRADE_INTERNAL_ASSERT(!_instance);
    _instance = this;

    Renderer::setFeature(Renderer::Feature::FaceCulling, true);
    Renderer::setFeature(Renderer::Feature::DepthTest, true);
    Renderer::setClearColor(Color3<>(0.0f));

    /* Add resource loader and fallback meshes */
    sceneResourceManager.setLoader(&meshResourceLoader);
    sceneResourceManager.setFallback<Mesh>(new Mesh);

    /* Text rendering... */
    SceneResourceManager::instance()->set<AbstractShaderProgram>("text2d", new Shaders::DistanceFieldVectorShader2D);
    Corrade::Utility::Resource rs("PushTheBoxData");
    const unsigned char* fontData;
    std::size_t fontSize;
    std::tie(fontData, fontSize) = rs.getRaw("luckiest-guy.ttf");
    Text::Font* font = new Text::Font(fontRenderer, fontData, fontSize, 128.0f);
    font->prerenderDistanceField("abcdefghijklmnopqrstuvwxyz0123456789 ", Vector2i(1536), Vector2i(256), 24);
    SceneResourceManager::instance()->set("font", font);

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
    defaultFramebuffer.clear(AbstractFramebuffer::Clear::Color|AbstractFramebuffer::Clear::Depth);

    AbstractScreenedApplication::drawEvent();

    swapBuffers();
    _timeline.nextFrame();
}

}

MAGNUM_APPLICATION_MAIN(PushTheBox::Application)
