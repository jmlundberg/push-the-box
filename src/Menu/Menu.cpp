#include "Menu.h"

#include <SceneGraph/Camera2D.h>

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
    o->color = Color3<GLfloat>::fromHSV(150.0f, 0.2f, 0.9f);
    debugDrawResourceManager.set<Physics::DebugDrawResourceManager::Options>("menu", o, ResourceDataState::Final, ResourcePolicy::Resident);

    /* Add some dummy menu items */
    (new MenuItem(&scene, &drawables, &shapes))
        ->translate(Vector2::yAxis(0.4f));
    (new MenuItem(&scene, &drawables, &shapes));
    (new MenuItem(&scene, &drawables, &shapes))
        ->translate(Vector2::yAxis(-0.4f));

    /** @todo fix this in magnum, so it doesn't have to be called? */
    shapes.setClean();
}

void Menu::focusEvent() {
    setPropagatedEvents(PropagatedEvent::Draw|PropagatedEvent::Input);
}

void Menu::blurEvent() {
    setPropagatedEvents({});
}

void Menu::viewportEvent(const Math::Vector2<GLsizei>& size) {
    camera->setViewport(size);
}

void Menu::drawEvent() {
    camera->draw(drawables);
}

void Menu::Menu::keyPressEvent(KeyEvent& event) {
    /* Switch to game */
    if(event.key() == KeyEvent::Key::Esc)
        application()->focusScreen(application()->backScreen()); /** @todo Implement me better */
    else return;

    event.setAccepted();
    redraw();
}

}}
