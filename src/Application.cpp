#include "Application.h"

#include <Framebuffer.h>
#include <SceneGraph/Camera3D.h>

namespace PushTheBox {

#ifndef MAGNUM_TARGET_NACL
Application::Application(int argc, char** argv): Sdl2Application(argc, argv, "Push The Box") {
#else
Application::Application(PP_Instance instance): NaClApplication(instance) {
#endif

    Framebuffer::setFeature(Framebuffer::Feature::FaceCulling, true);
    Framebuffer::setFeature(Framebuffer::Feature::DepthTest, true);
    Framebuffer::setClearColor(Color3<GLfloat>::fromHSV(15.0f, 0.95f, 0.2f));

    cameraObject = new Object3D(&scene);
    camera = new SceneGraph::Camera3D<>(cameraObject);
}

void Application::viewportEvent(const Math::Vector2<GLsizei>& size) {
    Framebuffer::setViewport({}, size);
    camera->setViewport(size);
}

void Application::drawEvent() {
    Framebuffer::clear(Framebuffer::Clear::Color);
    camera->draw(drawables);
    swapBuffers();
}

}

MAGNUM_APPLICATION_MAIN(PushTheBox::Application)
