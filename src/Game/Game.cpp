#include "Game.h"

#include <Math/Constants.h>
#include <SceneGraph/Camera3D.h>
#include <Shaders/PhongShader.h>

#include "Floor.h"
#include "Player.h"

namespace PushTheBox { namespace Game {

Game::Game() {
    /* Add shader to resource manager */
    SceneResourceManager::instance()->set<AbstractShaderProgram>("phong",
        new Shaders::PhongShader, ResourceDataState::Final, ResourcePolicy::Resident);

    /* Configure camera */
    (cameraObject = new Object3D(&scene))
        ->translate(Vector3::zAxis(5.5f))
        ->rotateX(deg(-35.0f));
    (camera = new SceneGraph::Camera3D<>(cameraObject))
        ->setPerspective(deg(35.0f), 0.001f, 100.0f)
        ->setAspectRatioPolicy(SceneGraph::AspectRatioPolicy::Extend);

    /* Add some default crap to the scene */
    new Floor(Color3<GLfloat>::fromHSV(15.0f, 0.55f, 0.6f), &scene, &drawables);
    (new Floor(Color3<GLfloat>::fromHSV(75.0f, 0.55f, 0.6f), &scene, &drawables))
        ->translate({1, 0, 0});
    (new Floor(Color3<GLfloat>::fromHSV(210.0f, 0.55f, 0.6f), &scene, &drawables))
        ->translate({0, 0, -1});
    (new Floor(Color3<GLfloat>::fromHSV(150.0f, 0.55f, 0.6f), &scene, &drawables))
        ->translate({-1, 0, 1});

    /* Add player */
    (new Player(&scene, &drawables));
}

void Game::focusEvent() {
    setPropagatedEvents(PropagatedEvent::Draw|PropagatedEvent::Input);
}

void Game::blurEvent() {
    /* Draw the game in the background */
    setPropagatedEvents(PropagatedEvent::Draw);
}

void Game::viewportEvent(const Math::Vector2<GLsizei>& size) {
    camera->setViewport(size);
}

void Game::drawEvent() {
    camera->draw(drawables);
}

void Game::keyPressEvent(KeyEvent& event) {
    if(event.key() == KeyEvent::Key::Up)
        cameraObject->rotateX(deg(-5.0f));
    else if(event.key() == KeyEvent::Key::Down)
        cameraObject->rotateX(deg(5.0f));

    /* Switch to menu */
    else if(event.key() == KeyEvent::Key::Esc)
        application()->focusScreen(application()->backScreen()); /** @todo Implement me better */

    else return;

    event.setAccepted();
    redraw();
}

}}
