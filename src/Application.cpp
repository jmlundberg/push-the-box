#include "Application.h"

#include <AbstractShaderProgram.h>
#include <Framebuffer.h>
#include <Mesh.h>

#include "Game/Game.h"
#include "Menu/Menu.h"

namespace PushTheBox {

#ifndef MAGNUM_TARGET_NACL
Application::Application(int argc, char** argv): AbstractScreenedApplication(argc, argv, "Push The Box") {
#else
Application::Application(PP_Instance instance): AbstractScreenedApplication(instance) {
#endif

    Framebuffer::setFeature(Framebuffer::Feature::FaceCulling, true);
    Framebuffer::setFeature(Framebuffer::Feature::DepthTest, true);
    Framebuffer::setClearColor(Color3<GLfloat>::fromHSV(15.0f, 0.95f, 0.2f));

    /* Add the screens */
    addScreen(new Menu::Menu);
    addScreen(new Game::Game);
}

Application::~Application() {
    /* Remove all screens before deleting the resource manager, so the
       resources can be properly freed */
    while(backScreen()) removeScreen(backScreen());
}

void Application::viewportEvent(const Math::Vector2<GLsizei>& size) {
    Framebuffer::setViewport({}, size);

    AbstractScreenedApplication::viewportEvent(size);
}

void Application::drawEvent() {
    Framebuffer::clear(Framebuffer::Clear::Color|Framebuffer::Clear::Depth);

    AbstractScreenedApplication::drawEvent();

    swapBuffers();
}

}

MAGNUM_APPLICATION_MAIN(PushTheBox::Application)
