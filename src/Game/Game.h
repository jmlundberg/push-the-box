#ifndef PushTheBox_Game_Game_h
#define PushTheBox_Game_Game_h

/** @file
 * @brief Class PushTheBox::Game::Game
 */

#include <SceneGraph/Drawable.h>
#include <SceneGraph/MatrixTransformation3D.h>
#include <SceneGraph/Scene.h>

#include "AbstractScreen.h"

namespace PushTheBox { namespace Game {

class Player;

/** @brief %Game screen */
class Game: public AbstractScreen {
    public:
        Game();

    protected:
        void focusEvent() override;
        void blurEvent() override;
        void viewportEvent(const Math::Vector2<GLsizei>& size) override;
        void drawEvent() override;
        void keyPressEvent(KeyEvent& event) override;
        void mousePressEvent(MouseEvent& event) override;
        void mouseMoveEvent(MouseMoveEvent& event) override;

    private:
        Scene3D scene;
        SceneGraph::DrawableGroup<3> drawables;

        Object3D* cameraObject;
        SceneGraph::Camera3D<>* camera;

        Player* player;
};

}}

#endif
