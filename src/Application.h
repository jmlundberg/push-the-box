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

#include "AbstractScreenedApplication.h"

namespace PushTheBox {

/** @brief Main application class */
class Application: public AbstractScreenedApplication {
    public:
        /** @brief Constructor */
        #ifndef MAGNUM_TARGET_NACL
        Application(int argc, char** argv);
        #else
        Application(PP_Instance instance);
        #endif

        ~Application();

    protected:
        void drawEvent() override;
        void viewportEvent(const Math::Vector2<GLsizei>& size) override;

    private:
        SceneResourceManager sceneResourceManager;
};

}

#endif
