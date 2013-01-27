#include "Application.h"

#include <AbstractShaderProgram.h>
#include <DefaultFramebuffer.h>
#include <Renderer.h>
#include <Mesh.h>
#include <Text/Font.h>

#include "Game/Game.h"
#include "Menu/Menu.h"

namespace PushTheBox {

#ifndef MAGNUM_TARGET_NACL
Application::Application(int argc, char** argv): AbstractScreenedApplication(argc, argv, "Push The Box") {
#else
Application::Application(PP_Instance instance): AbstractScreenedApplication(instance) {
#endif

    Renderer::setFeature(Renderer::Feature::FaceCulling, true);
    Renderer::setFeature(Renderer::Feature::DepthTest, true);
    Renderer::setClearColor(Color3<>(0.0f));

    /* Add resource loader and fallback meshes */
    sceneResourceManager.setLoader(&meshResourceLoader);
    sceneResourceManager.setFallback<Mesh>(new Mesh);

    /* Add the screens */
    addScreen(new Menu::Menu(fontRenderer));
    addScreen(new Game::Game);

    _timeline.start();
}

Application::~Application() {
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
