#include "Game.h"

#include <Math/Constants.h>
#include <SceneGraph/Camera3D.h>
#include <Shaders/PhongShader.h>

#include "Player.h"

namespace PushTheBox { namespace Game {

Game::Game() {
    /* Initial propagated events */
    setPropagatedEvents(PropagatedEvent::Draw);

    /* Add shader to resource manager */
    SceneResourceManager::instance()->set<AbstractShaderProgram>("phong",
        new Shaders::PhongShader, ResourceDataState::Final, ResourcePolicy::Resident);
    shader = SceneResourceManager::instance()->get<AbstractShaderProgram, Shaders::PhongShader>("phong");

    /* Create level */
    level = new Level("0", &scene, &drawables);

    /* Add player */
    player = new Player(level->startingPosition(), &scene, &drawables);

    /* Configure camera */
    (cameraObject = new Object3D(player))
        ->translate({0.0f, 1.0f, 7.5f})
        ->rotateX(deg(-35.0f));
    (camera = new SceneGraph::Camera3D<>(cameraObject))
        ->setPerspective(deg(35.0f), 0.001f, 100.0f)
        ->setAspectRatioPolicy(SceneGraph::AspectRatioPolicy::Extend);
}

void Game::focusEvent() {
    #ifdef MAGNUM_TARGET_NACL
    application()->setFullscreen(true);
    #endif
    application()->setMouseLocked(true);
    setPropagatedEvents(PropagatedEvent::Draw|PropagatedEvent::Input);
}

void Game::blurEvent() {
    /* Draw the game in the background */
    setPropagatedEvents(PropagatedEvent::Draw);

    application()->setMouseLocked(false);
    #ifdef MAGNUM_TARGET_NACL
    application()->setFullscreen(false);
    #endif
}

void Game::viewportEvent(const Math::Vector2<GLsizei>& size) {
    camera->setViewport(size);
}

void Game::drawEvent() {
    /* Shader settings commn for all objects */
    shader->setLightPosition((camera->cameraMatrix()*Point3D(1.0f, 4.0f, 1.2f)).xyz())
          ->setAmbientColor(Color3<>::fromHSV(15.0f, 0.5f, 0.06f))
          ->setSpecularColor(Color3<>::fromHSV(50.0f, 0.5f, 1.0f));
    camera->draw(drawables);
}

void Game::keyPressEvent(KeyEvent& event) {
    /* Move forward */
    if(event.key() == KeyEvent::Key::Up || event.key() == KeyEvent::Key::W) {
        Vector3 direction = player->transformation().backward().normalized();
        GLfloat angle = Vector3::angle(Vector3::zAxis(), direction);

        if(angle < Constants::pi()/6)
            player->move({0, -1});
        else if(angle > Constants::pi()/3 && angle < Constants::pi()*2/3)
            player->move({direction.x() > 0 ? -1 : 1, 0});
        else if(angle > Constants::pi()*5/6)
            player->move({0, 1});

    /* Switch to menu */
    } else if(event.key() == KeyEvent::Key::Esc)
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
