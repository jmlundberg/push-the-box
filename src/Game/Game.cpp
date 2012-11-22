#include "Game.h"

#include <Math/Constants.h>
#include <SceneGraph/Camera3D.h>
#include <Shaders/PhongShader.h>

#include "Box.h"
#include "FloorTile.h"
#include "Player.h"
#include "WallBrick.h"

namespace PushTheBox { namespace Game {

Game::Game() {
    /* Add shader to resource manager */
    SceneResourceManager::instance()->set<AbstractShaderProgram>("phong",
        new Shaders::PhongShader, ResourceDataState::Final, ResourcePolicy::Resident);

    /* Add player */
    player = new Player(&scene, &drawables);

    /* Configure camera */
    (cameraObject = new Object3D(player))
        ->translate({0.0f, 1.0f, 5.5f})
        ->rotateX(deg(-35.0f));
    (camera = new SceneGraph::Camera3D<>(cameraObject))
        ->setPerspective(deg(35.0f), 0.001f, 100.0f)
        ->setAspectRatioPolicy(SceneGraph::AspectRatioPolicy::Extend);

    /* Add some default crap to the scene */
    new FloorTile(Color3<>::fromHSV(15.0f, 0.55f, 0.6f), &scene, &drawables);
    (new FloorTile(Color3<>::fromHSV(75.0f, 0.55f, 0.6f), &scene, &drawables))
        ->translate({1, 0, 0});
    (new FloorTile(Color3<>::fromHSV(210.0f, 0.55f, 0.6f), &scene, &drawables))
        ->translate({0, 0, -1});
    (new FloorTile(Color3<>::fromHSV(150.0f, 0.55f, 0.6f), &scene, &drawables))
        ->translate({-1, 0, 1});

    (new WallBrick(Color3<>::fromHSV(15.0f, 0.2f, 0.8f), &scene, &drawables))
        ->translate({0, 0, -2});
    (new WallBrick(Color3<>::fromHSV(98.0f, 0.2f, 0.8f), &scene, &drawables))
        ->translate({-1, 0, -1});
    (new WallBrick(Color3<>::fromHSV(330.0f, 0.2f, 0.8f), &scene, &drawables))
        ->translate({-1, 0, 0});
    (new WallBrick(Color3<>::fromHSV(255.0f, 0.2f, 0.8f), &scene, &drawables))
        ->translate({1, 0, -1});

    (new Box(&scene, &drawables))
        ->translate({1, 0, 0});
}

void Game::focusEvent() {
    application()->setMouseLocked(true);
    setPropagatedEvents(PropagatedEvent::Draw|PropagatedEvent::Input);
}

void Game::blurEvent() {
    /* Draw the game in the background */
    setPropagatedEvents(PropagatedEvent::Draw);
    application()->setMouseLocked(false);
}

void Game::viewportEvent(const Math::Vector2<GLsizei>& size) {
    camera->setViewport(size);
}

void Game::drawEvent() {
    camera->draw(drawables);
}

void Game::keyPressEvent(KeyEvent& event) {
    if(event.key() == KeyEvent::Key::Esc)
        application()->focusScreen(application()->backScreen()); /** @todo Implement me better */
    else return;

    event.setAccepted();
    redraw();
}

void Game::mousePressEvent(MouseEvent& event) {
    if(event.button() == MouseEvent::Button::Left)
        application()->focusScreen(application()->backScreen()); /** @todo Implement me better */
    else return;

    event.setAccepted();
    redraw();
}

void Game::mouseMoveEvent(AbstractScreen::MouseMoveEvent& event) {
    /** @todo mouse sensitivity */
    player->rotateY(-rad((Constants::pi()*event.relativePosition().x()/500.0f)),
        SceneGraph::TransformationType::Local);

    Matrix4 xRotation = Matrix4::rotationX(rad(-Constants::pi()*event.relativePosition().y()/500.0f))*
        cameraObject->transformation();

    /* Don't rotate under the floor */
    if(std::abs(Vector3::dot((xRotation.rotation()*Vector3::yAxis()).normalized(), Vector3(0.0f, 1.0f, -1.0f).normalized())) > 0.75f)
        cameraObject->setTransformation(xRotation);

    event.setAccepted();
    redraw();
}

}}
