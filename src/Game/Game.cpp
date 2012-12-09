#include "Game.h"

#include <Math/Constants.h>
#include <Swizzle.h>
#include <SceneGraph/Camera3D.h>
#include <Shaders/PhongShader.h>

#include "Camera.h"
#include "Player.h"

namespace PushTheBox { namespace Game {

Game::Game() {
    /* Initial propagated events */
    setPropagatedEvents(PropagatedEvent::Draw);

    /* Add shader to resource manager */
    SceneResourceManager::instance()->set<AbstractShaderProgram>("phong",
        new Shaders::PhongShader, ResourceDataState::Final, ResourcePolicy::Resident);
    shader = SceneResourceManager::instance()->get<AbstractShaderProgram, Shaders::PhongShader>("phong");

    /* Add player */
    player = new Player(&scene, &drawables);

    /* Create level */
    (level = new Level("easy1", &scene, &drawables))
        ->resetPlayer(player);

    /* Add camera */
    (camera = new Camera(player))
        ->translate({0.0f, 1.0f, 7.5f})
        ->rotateX(deg(-35.0f));
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

void Game::viewportEvent(const Vector2i& size) {
    camera->setViewport(size);
}

void Game::drawEvent() {
    /* Light is above the center of level */
    Vector3 lightPosition = Vector3(1.0f, 4.0f, 1.2f) +
            Vector3::from(swizzle<'x', '0', 'y'>(level->size()/2));

    /* Shader settings commn for all objects */
    shader->setLightPosition((camera->cameraMatrix()*Point3D(lightPosition)).xyz())
          ->setProjectionMatrix(camera->projectionMatrix())
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
            level->movePlayer(player, {0, -1});
        else if(angle > Constants::pi()/3 && angle < Constants::pi()*2/3)
            level->movePlayer(player, {direction.x() > 0 ? -1 : 1, 0});
        else if(angle > Constants::pi()*5/6)
            level->movePlayer(player, {0, 1});

        if(level->remainingTargets() == 0) {
            std::string nextName = level->nextLevel();

            /* Load new level */
            delete level;
            (level = new Level(nextName, &scene, &drawables))
                ->resetPlayer(player);
        }

    /* Restart level */
    } else if(event.key() == KeyEvent::Key::R) {
        std::string name = level->name();

        /* Recreate level */
        delete level;
        (level = new Level(name, &scene, &drawables))
            ->resetPlayer(player);

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
        camera->transformation();

    /* Don't rotate under the floor */
    if(std::abs(Vector3::dot((xRotation.rotation()*Vector3::yAxis()).normalized(), Vector3(0.0f, 1.0f, -1.0f).normalized())) > 0.75f)
        camera->setTransformation(xRotation);

    event.setAccepted();
    redraw();
}

}}
