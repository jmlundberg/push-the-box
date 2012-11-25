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
    /* Initial propagated events */
    setPropagatedEvents(PropagatedEvent::Draw);

    /* Add shader to resource manager */
    SceneResourceManager::instance()->set<AbstractShaderProgram>("phong",
        new Shaders::PhongShader, ResourceDataState::Final, ResourcePolicy::Resident);
    shader = SceneResourceManager::instance()->get<AbstractShaderProgram, Shaders::PhongShader>("phong");

    /* Add level */
    actualLevel = new Level("some shity level name");

    /* Add player */
    player = new Player(&scene, &drawables, actualLevel);

    /* Configure camera */
    (cameraObject = new Object3D(player))
        ->translate({0.0f, 1.0f, 5.5f})
        ->rotateX(deg(-35.0f));
    (camera = new SceneGraph::Camera3D<>(cameraObject))
        ->setPerspective(deg(35.0f), 0.001f, 100.0f)
        ->setAspectRatioPolicy(SceneGraph::AspectRatioPolicy::Extend);

    /* Add some default crap to the scene */

    initializeLevel();
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
          ->setAmbientColor(Color3<>::fromHSV(15.0f, 0.6f, 0.15f));
    camera->draw(drawables);
}

void Game::keyPressEvent(KeyEvent& event) {
    if(event.key() == KeyEvent::Key::Up){
        GLfloat pi = Math::Constants<GLfloat>::pi();
        Vector3 direction = player->transformation().backward().normalized();
        GLfloat angle = Vector3::angle(Vector3::zAxis(), direction);

        if(angle< pi/6){
            player->move(0,-1);
        }
        if(angle> pi/3 && angle< pi*2/3){
            //move left or right
            if(direction.x() > 0){
                //move left(- in coordinates)
                player->move(-1,0);
            } else {
                //move right(+ in coordinates)
                player->move(1,0);
            }
        }
        if(angle > pi*5/6){
            player->move(0,1);
        }
    }
    /* Switch to menu */
    else if(event.key() == KeyEvent::Key::Esc)
        application()->focusScreen(application()->backScreen()); /** @todo Implement me better */
    else return;

    event.setAccepted();
    redraw();
}

void Game::mousePressEvent(MouseEvent& event) {
    if(event.button() == MouseEvent::Button::Left)
        application()->focusScreen(application()->backScreen()); /** @todo Implement me better */
    else return;

    //GLfloat angle = Vector3::angle(Vector3::zAxis(), player->transformation().backward().normalized());//vektor miri za hráèe


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

void Game::initializeLevel(){
    //add corner bricks:
    (new WallBrick(Color3<GLfloat>::fromHSV(0.0f, 0.2f, 1.0f), &scene, &drawables))
        ->translate({-1.0f, 0.0f, -1.0f});
    (new WallBrick(Color3<GLfloat>::fromHSV(0.0f, 0.2f, 1.0f), &scene, &drawables))
        ->translate({-1.0f, 0.0f, float(actualLevel->isTarget[0].size())});
    (new WallBrick(Color3<GLfloat>::fromHSV(0.0f, 0.2f, 1.0f), &scene, &drawables))
        ->translate({float(actualLevel->isTarget.size()), 0.0f, -1.0f});
    (new WallBrick(Color3<GLfloat>::fromHSV(0.0f, 0.2f, 1.0f), &scene, &drawables))
        ->translate({float(actualLevel->isTarget.size()), 0.0f, float(actualLevel->isTarget[0].size())});

    for(size_t i=0; i<actualLevel->isTarget.size(); ++i){
        (new WallBrick(Color3<GLfloat>::fromHSV(0.0f, 0.2f, 1.0f), &scene, &drawables))
            ->translate({float(i), 0.0f, -1.0f});
        (new WallBrick(Color3<GLfloat>::fromHSV(0.0f,0.2f,1.0f), &scene, &drawables))
            ->translate({float(i), 0.0f, float(actualLevel->isTarget[i].size())});
        for(size_t j=0; j<actualLevel->isTarget[i].size();++j){
            (new WallBrick(Color3<GLfloat>::fromHSV(0.0f, 0.2f, 1.0f), &scene, &drawables))
                ->translate({-1.0f, 0.0f, float(j)});
            (new WallBrick(Color3<GLfloat>::fromHSV(0.0f, 0.2f, 1.0f), &scene, &drawables))
                ->translate({actualLevel->isTarget.size(), 0.0f, float(j)});
            if(actualLevel->isTarget[i][j]==true){
                (new FloorTile(Color3<GLfloat>::fromHSV(0.0f, 1.0f, 0.5f), &scene, &drawables))
                    ->translate({float(i),0.0f,float(j)});
                continue;
            }
            (new FloorTile(Color3<GLfloat>::fromHSV(0.0f,0.1f,0.3f), &scene, &drawables))
                ->translate({float(i),0.0f,float(j)});
        }
    }
    for(size_t i=0; i<actualLevel->level.size(); ++i){
        for(size_t j=0; j<actualLevel->level[i].size(); ++j){
            if(actualLevel->level[i][j]==BOX){
                (new Box(&scene, &drawables))
                    ->translate({float(i),0.0f,float(j)});
            }
        }
    }
}

}}
