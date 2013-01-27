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
#include <Timeline.h>
#include <Text/FontRenderer.h>

#include "AbstractScreenedApplication.h"
#include "ResourceManagement/MeshResourceLoader.h"

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

        /** @brief Timeline */
        inline Timeline& timeline() { return _timeline; }
        inline const Timeline& timeline() const { return _timeline; } /**< @overload */

    protected:
        void drawEvent() override;
        void viewportEvent(const Vector2i& size) override;

    private:
        Text::FontRenderer fontRenderer;
        SceneResourceManager sceneResourceManager;
        ResourceManagement::MeshResourceLoader meshResourceLoader;
        Timeline _timeline;
};

}

#endif
