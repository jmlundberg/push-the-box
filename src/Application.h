#ifndef PushTheBox_Application_h
#define PushTheBox_Application_h

/** @file
 * @brief Class PushTheBox::Application
 */

#include "PushTheBox.h"

#ifndef MAGNUM_TARGET_NACL
#include <Platform/Sdl2Application.h>
#else
#include <Platform/NaClApplication.h>
#endif

#include <ResourceManager.h>
#include <SceneGraph/Drawable.h>
#include <SceneGraph/MatrixTransformation3D.h>
#include <SceneGraph/Scene.h>

namespace PushTheBox {

/** @brief Main application class */
class Application: public ApplicationBase {
    public:
        /** @brief Constructor */
        #ifndef MAGNUM_TARGET_NACL
        Application(int argc, char** argv);
        #else
        Application(PP_Instance instance);
        #endif

        ~Application();

    protected:
        /** @seemagnum{Platform::Sdl2Application::viewportEvent()} */
        void viewportEvent(const Math::Vector2<GLsizei>& size) override;

        /** @seemagnum{Platform::Sdl2Application::drawEvent()} */
        void drawEvent() override;

        /** @seemagnum{Platform::Sdl2Application::keyPressEvent()} */
        void keyPressEvent(KeyEvent& event) override;

    private:
        SceneResourceManager sceneResourceManager;

        Scene3D scene;
        SceneGraph::DrawableGroup<3> drawables;

        Object3D* cameraObject;
        SceneGraph::Camera3D<>* camera;
};

}

#endif
