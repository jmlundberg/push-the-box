#include "Menu.h"

#include <SceneGraph/Camera2D.h>
#include <Renderer.h>

#include "MenuItem.h"

namespace PushTheBox { namespace Menu {

Menu::Menu() {
    /* Configure camera */
    (cameraObject = new Object2D(&scene));
    (camera = new SceneGraph::Camera2D<>(cameraObject))
        ->setProjection({8.0f/3.0f, 2.0f})
        ->setAspectRatioPolicy(SceneGraph::AspectRatioPolicy::Extend);

    /* Configure color of menu debug shapes */
    /** @todo remove when done properly */
    auto o = new Physics::DebugDrawResourceManager::Options;
    o->color = Color3<>::fromHSV(240.0f, 0.2f, 0.5f);
    debugDrawResourceManager.set<Physics::DebugDrawResourceManager::Options>("menu", o, ResourceDataState::Final, ResourcePolicy::Resident);

    /* Add some dummy menu items */
    (new MenuItem(&scene, &drawables, &shapes));

    /** @todo fix this in magnum, so it doesn't have to be called? */
    shapes.setClean();
}

void Menu::focusEvent() {
    setPropagatedEvents(PropagatedEvent::Draw|PropagatedEvent::Input);
}

void Menu::blurEvent() {
    setPropagatedEvents({});
}

void Menu::viewportEvent(const Vector2i& size) {
    camera->setViewport(size);
}

void Menu::drawEvent() {
    Renderer::setFeature(Renderer::Feature::DepthTest, false);
    camera->draw(drawables);
    Renderer::setFeature(Renderer::Feature::DepthTest, true);
}

void Menu::mousePressEvent(MouseEvent& event) {
    if(event.button() == MouseEvent::Button::Left)
        application()->focusScreen(application()->backScreen()); /** @todo Implement me better */
    else return;

    event.setAccepted();
    redraw();
}

}}
