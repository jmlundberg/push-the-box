#ifndef PushTheBox_Application_h
#define PushTheBox_Application_h

/** @file
 * @brief Class PushTheBox::Application
 */

#include "PushTheBox.h"

#ifndef CORRADE_TARGET_NACL
#include <Platform/Sdl2Application.h>
#else
#include <Platform/NaClApplication.h>
#endif

#include <Interconnect/Receiver.h>
#include <PluginManager/PluginManager.h>
#include <ResourceManager.h>
#include <Timeline.h>

#include "AbstractScreenedApplication.h"
#include "ResourceManagement/MeshResourceLoader.h"

namespace PushTheBox {

namespace Game {
    class Game;
}

namespace Menu {
    class Menu;
}

/** @brief Main application class */
class Application: public AbstractScreenedApplication, public Corrade::Interconnect::Receiver {
    public:
        static Application* instance();

        /** @brief Constructor */
        Application(const Arguments& arguments);

        ~Application();

        /** @brief Game screen */
        inline Game::Game* gameScreen() { return _gameScreen; }

        /** @brief Menu screen */
        inline Menu::Menu* menuScreen() { return _menuScreen; }

        /** @brief Timeline */
        inline Timeline& timeline() { return _timeline; }

    protected:
        void drawEvent() override;
        void viewportEvent(const Vector2i& size) override;

    private:
        static Application* _instance;

        Corrade::PluginManager::PluginManager<Text::AbstractFont> fontPluginManager;
        SceneResourceManager sceneResourceManager;
        ResourceManagement::MeshResourceLoader meshResourceLoader;
        Timeline _timeline;

        Game::Game* _gameScreen;
        Menu::Menu* _menuScreen;
};

}

#endif
