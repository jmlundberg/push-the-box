#ifndef PushTheBox_Splash_Splash_h
#define PushTheBox_Splash_Splash_h

/** @file
 * @brief Class PushTheBox::Splash::Splash
 */

#include <Magnum/Platform/Screen.h>
#include <Magnum/Platform/Sdl2Application.h>
#include <Magnum/SceneGraph/Drawable.h>
#include <Magnum/SceneGraph/Scene.h>
#include <Magnum/SceneGraph/TranslationTransformation.h>

#include "PushTheBox.h"

namespace PushTheBox { namespace Splash {

/** @brief %Splash screen */
class Splash: public Platform::Screen {
    public:
        Splash();

    protected:
        void focusEvent() override;
        void blurEvent() override;
        void viewportEvent(const Vector2i& size) override;
        void drawEvent() override;
        void mouseReleaseEvent(MouseEvent& event) override;

    private:
        Scene2D scene;
        SceneGraph::DrawableGroup2D drawables;

        SceneGraph::Camera2D* camera;
};

}}

#endif
