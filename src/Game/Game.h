#ifndef PushTheBox_Game_Game_h
#define PushTheBox_Game_Game_h

/** @file
 * @brief Class PushTheBox::Game::Game
 */

#include <Corrade/Interconnect/Receiver.h>
#include <Magnum/ResourceManager.h>
#include <Magnum/Timeline.h>
#include <Magnum/Platform/Screen.h>
#include <Magnum/SceneGraph/AnimableGroup.h>
#include <Magnum/SceneGraph/Drawable.h>
#include <Magnum/SceneGraph/TranslationTransformation.h>
#include <Magnum/SceneGraph/DualQuaternionTransformation.h>
#include <Magnum/SceneGraph/Scene.h>
#include <Magnum/Shaders/Shaders.h>

#include "PushTheBox.h"

namespace PushTheBox { namespace Game {

class Camera;
class Level;
class LevelTitle;
class Moves;
class Player;
class RemainingTargets;

/**
@brief %Game screen

Renders current state of the game.
*/
class Game: public Platform::Screen, public Interconnect::Receiver {
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
        SceneGraph::DrawableGroup3D drawables;
        SceneGraph::AnimableGroup3D animables;

        Resource<AbstractShaderProgram, Shaders::Phong> shader;
        Camera* camera;
        Level* level;
        Player* player;

        Scene2D hudScene;
        SceneGraph::DrawableGroup2D hudDrawables;
        SceneGraph::AnimableGroup2D hudAnimables;
        SceneGraph::Camera2D* hudCamera;
        bool paused;

        LevelTitle* levelTitle;
        RemainingTargets* remainingTargets;
        Moves* moves;
};

}}

#endif
