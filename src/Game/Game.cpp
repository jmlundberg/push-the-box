#include "Game.h"

#include <Swizzle.h>
#include <SceneGraph/Animable.h>
#include <SceneGraph/AnimableGroup.h>
#include <SceneGraph/Camera2D.h>
#include <Shaders/Phong.h>
#include <Renderer.h>

#include "Application.h"
#include "Game/Camera.h"
#include "Game/Level.h"
#include "Game/Player.h"
#include "Hud.h"
#include "Menu/Menu.h"

namespace PushTheBox { namespace Game {

Game* Game::_instance = nullptr;

Game* Game::instance() {
    CORRADE_INTERNAL_ASSERT(_instance);
    return _instance;
}

Game::Game(): level(nullptr), paused(true) {
    CORRADE_INTERNAL_ASSERT(!_instance);
    _instance = this;

    /* Initial propagated events */
    setPropagatedEvents(PropagatedEvent::Draw);

    /* Add shader to resource manager */
    SceneResourceManager::instance().set<AbstractShaderProgram>("phong", new Magnum::Shaders::Phong);
    shader = SceneResourceManager::instance().get<AbstractShaderProgram, Magnum::Shaders::Phong>("phong");

    /* Add player */
    player = new Player(&scene, &drawables);

    /* Add camera */
    (camera = new Camera(player))
        ->translate({0.0f, 1.0f, 7.5f})
        .rotateX(Deg(-35.0f));

    /* Hud */
    levelTitle = new LevelTitle(&hudScene, &hudDrawables);
    remainingTargets = new RemainingTargets(&hudScene, &hudDrawables, &hudAnimables);
    moves = new Moves(&hudScene, &hudDrawables);

    /* Hud camera */
    hudCameraObject = new Object2D(&hudScene);
    (hudCamera = new SceneGraph::Camera2D(hudScene))
        ->setAspectRatioPolicy(SceneGraph::AspectRatioPolicy::Extend)
        .setProjection({2.667f, 2.0f});

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
          .translate(Vector3(swizzle<'x', '0', 'y'>(level->playerPosition())));

    /* Connect HUD to level state changes */
    levelTitle->update(level->title());
    remainingTargets->update(level->remainingTargets());
    moves->update(level->moves());
    Level::connect(level, &Level::remainingTargetsChanged, remainingTargets, &RemainingTargets::update);
    Level::connect(level, &Level::movesChanged, moves, &Moves::update);
}

void Game::restartLevel() {
    CORRADE_ASSERT(level, "Game::Game::restartLevel(): no level loaded", );

    /* copy string to avoid dangling reference */
    loadLevel(std::string(level->name()));

    resume();
}

void Game::nextLevel() {
    CORRADE_ASSERT(level, "Game::Game::nextLevel(): no level loaded", );
    CORRADE_ASSERT(!level->remainingTargets(), "Game::Game::nextLevel(): level not solved", );

    /* copy string to avoid dangling reference */
    loadLevel(std::string(level->nextName()));
}

void Game::movePlayer(const Vector2i& direction) {
    CORRADE_ASSERT(level, "Game::Game::movePlayer(): no level loaded", );

    if(level->movePlayer(direction))
        player->translate(Vector3(swizzle<'x', '0', 'y'>(direction)));
}

void Game::pause() {
    Application::instance()->focusScreen(Application::instance()->menuScreen());
}

void Game::resume() {
    Application::instance()->focusScreen(this);
}

void Game::focusEvent() {
    Application::instance()->setMouseLocked(true);

    paused = false;
    camera->setBlurred(false);
    setPropagatedEvents(PropagatedEvent::Draw|PropagatedEvent::Input);
}

void Game::blurEvent() {
    /* Draw the game in the background */
    setPropagatedEvents(PropagatedEvent::Draw);
    camera->setBlurred(true);
    paused = true;

    Application::instance()->setMouseLocked(false);
}

void Game::viewportEvent(const Vector2i& size) {
    camera->setViewport(size);
    hudCamera->setViewport(size);
}

void Game::drawEvent() {
    /* Animate */
    animables.step(Application::instance()->timeline().previousFrameTime(),
                   Application::instance()->timeline().previousFrameDuration());
    hudAnimables.step(Application::instance()->timeline().previousFrameTime(),
                      Application::instance()->timeline().previousFrameDuration());

    /* Light is above the center of level */
    Vector3 lightPosition = Vector3(1.0f, 4.0f, 1.2f) +
            Vector3(swizzle<'x', '0', 'y'>(level->size()/2));

    /* Shader settings commn for all objects */
    shader->setLightPosition(camera->cameraMatrix().transformPoint(lightPosition))
          .setProjectionMatrix(camera->projectionMatrix())
          .setAmbientColor(Color3::fromHSV(Deg(15.0f), 0.5f, 0.06f))
          .setSpecularColor(Color3::fromHSV(Deg(50.0f), 0.5f, 1.0f));
    camera->draw(drawables);

    /* Draw HUD */
    if(!paused) {
        Renderer::setFeature(Renderer::Feature::Blending, true);
        Renderer::setBlendFunction(Renderer::BlendFunction::One, Renderer::BlendFunction::OneMinusSourceAlpha);
        Renderer::setFeature(Renderer::Feature::DepthTest, false);
        hudCamera->draw(hudDrawables);
        Renderer::setFeature(Renderer::Feature::DepthTest, true);
        Renderer::setFeature(Renderer::Feature::Blending, false);
    }

    /** @todo properly schedule redraw only if there is something to animate */
    redraw();
}

void Game::keyPressEvent(KeyEvent& event) {
    /* Move forward */
    if(event.key() == KeyEvent::Key::Up || event.key() == KeyEvent::Key::W) {
        Vector3 direction = player->transformation().rotation().transformVectorNormalized(Vector3::zAxis());
        Deg angle = Vector3::angle(Vector3::zAxis(), direction);

        if(angle < Deg(30.0f))
            movePlayer({0, -1});
        else if(angle > Deg(60.0f) && angle < Deg(120.0f))
            movePlayer({direction.x() > 0 ? -1 : 1, 0});
        else if(angle > Deg(150.0f))
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
    player->normalizeRotation().rotateY(-Rad(Constants::pi())*event.relativePosition().x()/500.0f,
        SceneGraph::TransformationType::Local);

    Rad angle(-Constants::pi()*event.relativePosition().y()/500.0f);
    DualQuaternion xRotation = DualQuaternion::rotation(angle, Vector3::xAxis())*camera->transformation();

    /* Don't rotate under the floor */
    if(Math::abs(Vector3::dot(xRotation.real().transformVector(Vector3::yAxis()), Vector3(0.0f, 1.0f, -1.0f).normalized())) > 0.75f)
        camera->setTransformation(xRotation.normalized());

    event.setAccepted();
    redraw();
}

}}
