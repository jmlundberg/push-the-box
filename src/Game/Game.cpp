#include "Game.h"

#include <Math/Constants.h>
#include <Swizzle.h>
#include <SceneGraph/Animable.h>
#include <SceneGraph/AnimableGroup.h>
#include <SceneGraph/Camera3D.h>
#include <Shaders/PhongShader.h>

#include "Application.h"
#include "Game/Camera.h"
#include "Game/Level.h"
#include "Game/Player.h"
#include "Menu/Menu.h"

namespace PushTheBox { namespace Game {

Game* Game::_instance = nullptr;

Game* Game::instance() {
    CORRADE_INTERNAL_ASSERT(_instance);
    return _instance;
}

Game::Game(): level(nullptr) {
    CORRADE_INTERNAL_ASSERT(!_instance);
    _instance = this;

    /* Initial propagated events */
    setPropagatedEvents(PropagatedEvent::Draw);

    /* Add shader to resource manager */
    SceneResourceManager::instance()->set<AbstractShaderProgram>("phong", new Shaders::PhongShader);
    shader = SceneResourceManager::instance()->get<AbstractShaderProgram, Shaders::PhongShader>("phong");

    /* Add player */
    player = new Player(&scene, &drawables);

    /* Add camera */
    (camera = new Camera(player))
        ->translate({0.0f, 1.0f, 7.5f})
        ->rotateX(deg(-35.0f));

    loadLevel("easy1");
}

Game::~Game() {
    CORRADE_INTERNAL_ASSERT(_instance == this);
    _instance = nullptr;
}

void Game::loadLevel(const std::string& name) {
    delete level;
    level = new Level(name, &scene, &drawables, &animables);
    player->resetTransformation()
          ->translate(Vector3(swizzle<'x', '0', 'y'>(level->playerPosition())));
}

void Game::restartLevel() {
    CORRADE_ASSERT(level, "Game::Game::restartLevel(): no level loaded", );

    /* copy string to avoid dangling reference */
    loadLevel(std::string(level->name()));
}

void Game::nextLevel() {
    CORRADE_ASSERT(level, "Game::Game::nextLevel(): no level loaded", );
    CORRADE_ASSERT(!level->remainingTargets(), "Game::Game::nextLevel(): level not solved", );

    /* copy string to avoid dangling reference */
    loadLevel(std::string(level->nextName()));
}

void Game::movePlayer(const Vector2i& direction) {
    CORRADE_ASSERT(level, "Game::Game::movePlayer(): no level loaded", );

    level->movePlayer(direction);
    player->translate(Vector3(swizzle<'x', '0', 'y'>(direction)));
}

void Game::pause() {
    Application::instance()->focusScreen(Application::instance()->menuScreen());
}

void Game::resume() {
    Application::instance()->focusScreen(this);
}

void Game::focusEvent() {
    #ifdef MAGNUM_TARGET_NACL
    Application::instance()->setFullscreen(true);
    #endif
    Application::instance()->setMouseLocked(true);
    setPropagatedEvents(PropagatedEvent::Draw|PropagatedEvent::Input);
}

void Game::blurEvent() {
    /* Draw the game in the background */
    setPropagatedEvents(PropagatedEvent::Draw);

    Application::instance()->setMouseLocked(false);
    #ifdef MAGNUM_TARGET_NACL
    Application::instance()->setFullscreen(false);
    #endif
}

void Game::viewportEvent(const Vector2i& size) {
    camera->setViewport(size);
}

void Game::drawEvent() {
    /* Animate */
    animables.step(Application::instance()->timeline().previousFrameTime(),
                   Application::instance()->timeline().previousFrameDuration());

    /* Light is above the center of level */
    Vector3 lightPosition = Vector3(1.0f, 4.0f, 1.2f) +
            Vector3(swizzle<'x', '0', 'y'>(level->size()/2));

    /* Shader settings commn for all objects */
    shader->setLightPosition((camera->cameraMatrix()*Point3D(lightPosition)).xyz())
          ->setProjectionMatrix(camera->projectionMatrix())
          ->setAmbientColor(Color3<>::fromHSV(15.0f, 0.5f, 0.06f))
          ->setSpecularColor(Color3<>::fromHSV(50.0f, 0.5f, 1.0f));
    camera->draw(drawables);

    /* Schedule redraw, if there is something to animate */
    if(animables.runningCount()) redraw();
}

void Game::keyPressEvent(KeyEvent& event) {
    /* Move forward */
    if(event.key() == KeyEvent::Key::Up || event.key() == KeyEvent::Key::W) {
        Vector3 direction = player->transformation().backward();
        GLfloat angle = Vector3::angle(Vector3::zAxis(), direction);

        if(angle < Constants::pi()/6)
            movePlayer({0, -1});
        else if(angle > Constants::pi()/3 && angle < Constants::pi()*2/3)
            movePlayer({direction.x() > 0 ? -1 : 1, 0});
        else if(angle > Constants::pi()*5/6)
            movePlayer({0, 1});

        if(!level->remainingTargets()) nextLevel();

    /* Restart level */
    } else if(event.key() == KeyEvent::Key::R) {
        restartLevel();

    /* Switch to menu */
    } else if(event.key() == KeyEvent::Key::Esc)
        pause();

    else return;

    event.setAccepted();
    redraw();
}

void Game::mousePressEvent(MouseEvent& event) {
    if(event.button() == MouseEvent::Button::Left)
        pause();
    else return;

    event.setAccepted();
    redraw();
}

void Game::mouseMoveEvent(AbstractScreen::MouseMoveEvent& event) {
    /** @todo mouse sensitivity */
    player->normalizeRotation()->rotateY(-rad((Constants::pi()*event.relativePosition().x()/500.0f)),
        SceneGraph::TransformationType::Local);

    GLfloat angle = rad(-Constants::pi()*event.relativePosition().y()/500.0f);
    Matrix4 xRotation = Matrix4::rotationX(angle)*camera->transformation();

    /* Don't rotate under the floor */
    if(std::abs(Vector3::dot(xRotation.rotation()*Vector3::yAxis(), Vector3(0.0f, 1.0f, -1.0f).normalized())) > 0.75f)
        camera->normalizeRotation()->rotateX(angle);

    event.setAccepted();
    redraw();
}

}}
