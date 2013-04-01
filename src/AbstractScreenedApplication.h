#ifndef PushTheBox_AbstractScreenedApplication_h
#define PushTheBox_AbstractScreenedApplication_h

/** @file
 * @brief Class PushTheBox::AbstractScreenedApplication
 */

#include <Containers/LinkedList.h>
#include <Magnum.h>

#ifndef CORRADE_TARGET_NACL
#include <Platform/Sdl2Application.h>
#else
#include <Platform/NaClApplication.h>
#endif

#include "PushTheBox.h"

namespace PushTheBox {

class AbstractScreen;

/**
@brief Base for applications with screen management

Manages list of screens and propagates events to them.

Traversing through the list of screens is done like following:
@code
// front-to-back
for(AbstractScreen* s = app.frontScreen(); s; s = s->nextFartherScreen()) {
    // ...
}

// back-to-front
for(AbstractScreen* s = app.backScreen(); s; s = s->nextNearerScreen()) {
    // ...
}
@endcode

@section AbstractScreenedApplication-event-propagation Event propagation

Each AbstractScreen specifies which set of events should be propagated to it
using AbstractScreen::setPropagatedEvents(). When application gets an event,
they are propagated to the screens:

 - @ref Magnum::Platform::Sdl2Application::viewportEvent() "viewportEvent()"
   is propagated to all screens.
 - @ref Magnum::Platform::Sdl2Application::drawEvent() "drawEvent()" is
   propagated in back-to-front order to screens which have
   @ref AbstractScreen::PropagatedEvent "AbstractScreen::PropagatedEvent::Draw"
   enabled.
 - Input events (@ref Magnum::Platform::Sdl2Application::keyPressEvent() "keyPressEvent()",
   @ref Magnum::Platform::Sdl2Application::keyReleaseEvent() "keyReleaseEvent()",
   @ref Magnum::Platform::Sdl2Application::mousePressEvent() "mousePressEvent()",
   @ref Magnum::Platform::Sdl2Application::mouseReleaseEvent() "mouseReleaseEvent()"
   and @ref Magnum::Platform::Sdl2Application::mouseMoveEvent() "mouseMoveEvent()")
   are propagated in front-to-back order to screens which have
   @ref AbstractScreen::PropagatedEvent "AbstractScreen::PropagatedEvent::Input"
   enabled. If any screen sets the event as accepted, it is not propagated
   further.

*/
class AbstractScreenedApplication: public Platform::Application, private Corrade::Containers::LinkedList<AbstractScreen> {
    friend class Corrade::Containers::LinkedList<AbstractScreen>;
    friend class Corrade::Containers::LinkedListItem<AbstractScreen, AbstractScreenedApplication>;
    friend class AbstractScreen;

    public:
        /** @seemagnum{Platform::Sdl2Application::Sdl2Application()} */
        #ifndef CORRADE_TARGET_NACL
        inline AbstractScreenedApplication(int argc, char** argv, Configuration* configuration): Platform::Application(argc, argv, configuration) {}
        #else
        inline AbstractScreenedApplication(PP_Instance instance, Configuration* configuration): Platform::Application(instance, configuration) {}
        #endif

        /**
         * @brief Add screen to application
         * @return Pointer to self (for method chaining)
         *
         * The new screen is added as backmost. If this is the first screen
         * added, AbstractScreen::focus() is called. If not, neither
         * AbstractScreen::blur() nor AbstractScreen::focus() is called (i.e.
         * the screen default state is used).
         */
        AbstractScreenedApplication* addScreen(AbstractScreen* screen);

        /**
         * @brief Remove screen from application
         * @return Pointer to self (for method chaining)
         *
         * The screen is blurred before removing. Deleting the object is left
         * up to the user.
         * @see AbstractScreen::blur()
         */
        AbstractScreenedApplication* removeScreen(AbstractScreen* screen);

        /**
         * @brief Focus screen
         * @return Pointer to self (for method chaining)
         *
         * Moves the screen to front. Previously focused screen is blurred and
         * this screen is focused.
         * @see AbstractScreen::blur(), AbstractScreen::focus()
         */
        AbstractScreenedApplication* focusScreen(AbstractScreen* screen);

        /**
         * @brief Front screen
         *
         * @see AbstractScreen::nextFartherScreen(), AbstractScreen::nextNearerScreen()
         */
        inline AbstractScreen* frontScreen() { return first(); }
        inline const AbstractScreen* frontScreen() const { return first(); } /**< @overload */

        /**
         * @brief Back screen
         *
         * @see AbstractScreen::nextFartherScreen(), AbstractScreen::nextNearerScreen()
         */
        inline AbstractScreen* backScreen() { return last(); }
        inline const AbstractScreen* backScreen() const { return last(); } /**< @overload */

    protected:
        /**
         * @brief Viewport event
         *
         * Called when viewport size changes. You should pass the new size to
         * Framebuffer::setViewport() and then call this function to propagate
         * the event to all screens.
         * @seemagnum{Platform::Sdl2Application::viewportEvent()}
         */
        void viewportEvent(const Vector2i& size) override = 0;

        /**
         * @brief Draw event
         *
         * Called when the screen is redrawn. You should clean the framebuffer
         * using Framebuffer::clear(). and then call this function to
         * propagate the event to all screens. After the drawing is finished,
         * call swapBuffers().
         * @seemagnum{Platform::Sdl2Application::drawEvent()}
         */
        void drawEvent() override = 0;

        /** @seemagnum{Platform::Sdl2Application::keyPressEvent()} */
        void keyPressEvent(KeyEvent& event) override;

        /** @seemagnum{Platform::Sdl2Application::keyReleaseEvent()} */
        void keyReleaseEvent(KeyEvent& event) override;

        /** @seemagnum{Platform::Sdl2Application::mousePressEvent()} */
        void mousePressEvent(MouseEvent& event) override;

        /** @seemagnum{Platform::Sdl2Application::mouseReleaseEvent()} */
        void mouseReleaseEvent(MouseEvent& event) override;

        /** @seemagnum{Platform::Sdl2Application::mouseMoveEvent()} */
        void mouseMoveEvent(MouseMoveEvent& event) override;
};

}

#endif
