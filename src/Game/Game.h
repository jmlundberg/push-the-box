#ifndef PushTheBox_Game_Game_h
#define PushTheBox_Game_Game_h

/** @file
 * @brief Class PushTheBox::Game::Game
 */

#include <ResourceManager.h>
#include <Timeline.h>
#include <SceneGraph/Drawable.h>
#include <SceneGraph/MatrixTransformation3D.h>
#include <SceneGraph/Scene.h>
#include <Shaders/Shaders.h>

#include "AbstractScreen.h"
#include "Level.h"
#include "Box.h"

namespace PushTheBox { namespace Game {

class Camera;
class Player;

/** @brief %Game screen */
class Game: public AbstractScreen {
    public:
        Game();

    protected:
        void focusEvent() override;
        void blurEvent() override;
        void viewportEvent(const Vector2i& size) override;
        void drawEvent() override;
        void keyPressEvent(KeyEvent& event) override;
        void mousePressEvent(MouseEvent& event) override;
        void mouseMoveEvent(MouseMoveEvent& event) override;

    private:
        Scene3D scene;
        SceneGraph::DrawableGroup<3> drawables;

        Resource<AbstractShaderProgram, Shaders::PhongShader> shader;
        Camera* camera;

        Player* player;
        Level* level;
};

}}

#endif
