#ifndef PushTheBox_Game_Game_h
#define PushTheBox_Game_Game_h

/** @file
 * @brief Class PushTheBox::Game::Game
 */

#include <Interconnect/Receiver.h>
#include <ResourceManager.h>
#include <Timeline.h>
#include <SceneGraph/AnimableGroup.h>
#include <SceneGraph/Drawable.h>
#include <SceneGraph/EuclideanMatrixTransformation2D.h>
#include <SceneGraph/EuclideanMatrixTransformation3D.h>
#include <SceneGraph/Scene.h>
#include <Shaders/Shaders.h>

#include "AbstractScreen.h"

namespace PushTheBox { namespace Game {

class Camera;
class Level;
class Moves;
class Player;
class RemainingTargets;

/**
@brief %Game screen

Renders current state of the game.
*/
class Game: public AbstractScreen, public Corrade::Interconnect::Receiver {
    public:
        static Game* instance();

        Game();

        ~Game();

        void restartLevel();
        void nextLevel();
        void loadLevel(const std::string& name);
        void movePlayer(const Vector2i& direction);

        void pause();
        void resume();

    protected:
        void focusEvent() override;
        void blurEvent() override;
        void viewportEvent(const Vector2i& size) override;
        void drawEvent() override;
        void keyPressEvent(KeyEvent& event) override;
        void mousePressEvent(MouseEvent& event) override;
        void mouseMoveEvent(MouseMoveEvent& event) override;

    private:
        static Game* _instance;

        Scene3D scene;
        SceneGraph::DrawableGroup<3> drawables;
        SceneGraph::AnimableGroup<3> animables;

        Resource<AbstractShaderProgram, Shaders::PhongShader> shader;
        Camera* camera;
        Level* level;
        Player* player;

        Scene2D hudScene;
        SceneGraph::DrawableGroup<2> hudDrawables;
        SceneGraph::AnimableGroup<2> hudAnimables;
        Object2D* hudCameraObject;
        SceneGraph::Camera2D<>* hudCamera;

        RemainingTargets* remainingTargets;
        Moves* moves;
};

}}

#endif
