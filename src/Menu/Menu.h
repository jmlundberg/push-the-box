#ifndef PushTheBox_Menu_Menu_h
#define PushTheBox_Menu_Menu_h

/** @file
 * @brief Class PushTheBox::Menu::Menu
 */

#include <Corrade/Interconnect/Receiver.h>
#include <Magnum/Platform/Screen.h>
#include <Magnum/Platform/Sdl2Application.h>
#include <Magnum/Shapes/ShapeGroup.h>
#include <Magnum/SceneGraph/Drawable.h>
#include <Magnum/SceneGraph/TranslationTransformation.h>
#include <Magnum/SceneGraph/Scene.h>

#include "PushTheBox.h"

namespace PushTheBox { namespace Menu {

class Cursor;

/** @brief %Menu screen */
class Menu: public Platform::Screen, public Interconnect::Receiver {
    public:
        Menu();

    protected:
        void focusEvent() override;
        void blurEvent() override;
        void viewportEvent(const Vector2i& size) override;
        void drawEvent() override;
        void mousePressEvent(MouseEvent& event) override;
        void mouseMoveEvent(MouseMoveEvent& event) override;

    private:
        Scene2D scene;
        SceneGraph::DrawableGroup2D drawables;
        Shapes::ShapeGroup2D shapes;

        SceneGraph::Camera2D* camera;
        Cursor* cursor;
};

}}

#endif
