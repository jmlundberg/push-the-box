#include "Menu.h"

#include <Magnum/DefaultFramebuffer.h>
#include <Magnum/Renderer.h>
#include <Magnum/SceneGraph/Camera2D.h>

#include "Application.h"
#include "Game/Game.h"
#include "Menu/Cursor.h"
#include "Menu/MenuItem.h"

namespace PushTheBox { namespace Menu {

Menu::Menu() {
    /* Configure camera */
    camera = new SceneGraph::Camera2D(scene);
    camera->setProjectionMatrix(Matrix3::projection({8.0f/3.0f, 2.0f}))
        .setAspectRatioPolicy(SceneGraph::AspectRatioPolicy::Extend)
        .setViewport(defaultFramebuffer.viewport().size());

    /* Add menu items */
    MenuItem* i;
    i = new MenuItem("resume", &scene, &drawables, &shapes);
    i->translate(Vector2::yAxis(0.3f));
    Interconnect::connect(*i, &MenuItem::clicked, *Game::Game::instance(), &Game::Game::resume);

    i = new MenuItem("restart level", &scene, &drawables, &shapes);
    Interconnect::connect(*i, &MenuItem::clicked, *Game::Game::instance(), &Game::Game::restartLevel);

    i = new MenuItem("exit", &scene, &drawables, &shapes);
    i->translate(Vector2::yAxis(-0.3f));
    /** @todo What about this? */
    #ifndef CORRADE_TARGET_NACL
    Interconnect::connect(*i, &MenuItem::clicked, *Application::instance(), &Application::exit);
    #endif

    /* Add cursor */
    cursor = new Cursor(&scene, &shapes);
    cursor->translate({-10.0f, -10.0f});
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
    Renderer::enable(Renderer::Feature::Blending);
    Renderer::setBlendFunction(Renderer::BlendFunction::One, Renderer::BlendFunction::OneMinusSourceAlpha);
    Renderer::disable(Renderer::Feature::DepthTest);
    /** @todo fix this in magnum, so it doesn't have to be called? */
    shapes.setClean();
    camera->draw(drawables);
    Renderer::enable(Renderer::Feature::DepthTest);
    Renderer::disable(Renderer::Feature::Blending);
}

void Menu::mousePressEvent(MouseEvent& event) {
    if(event.button() == MouseEvent::Button::Left) {
        cursor->resetTransformation().translate(
        Vector2::yScale(-1.0f)*(Vector2(event.position())/Vector2(defaultFramebuffer.viewport().size())-Vector2(0.5f))*camera->projectionSize());

        MenuItem* item = static_cast<MenuItem*>(shapes.firstCollision(*cursor));
        if(item) item->clicked();

    } else return;

    event.setAccepted();
    redraw();
}

void Menu::mouseMoveEvent(MouseMoveEvent& event) {
    Vector2 cursorPosition =
        Vector2::yScale(-1.0f)*(Vector2(event.position())/Vector2(defaultFramebuffer.viewport().size())-Vector2(0.5f))*camera->projectionSize();

    MenuItem* collisionBefore = static_cast<MenuItem*>(shapes.firstCollision(*cursor));
    cursor->resetTransformation().translate(cursorPosition);
    MenuItem* collisionAfter = static_cast<MenuItem*>(shapes.firstCollision(*cursor));

    if(collisionBefore != collisionAfter) {
        if(collisionBefore) collisionBefore->hoverChanged(false);
        if(collisionAfter) collisionAfter->hoverChanged(true);
    }

    event.setAccepted();
    redraw();
}

}}
