#ifndef PushTheBox_Splash_Splash_h
#define PushTheBox_Splash_Splash_h

/** @file
 * @brief Class PushTheBox::Splash::Splash
 */

#include <SceneGraph/Drawable.h>
#include <SceneGraph/DualComplexTransformation.h>
#include <SceneGraph/Scene.h>

#include "AbstractScreen.h"

namespace PushTheBox { namespace Splash {

/** @brief %Splash screen */
class Splash: public AbstractScreen {
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
