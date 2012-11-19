#ifndef PushTheBox_Screen_h
#define PushTheBox_Screen_h

/** @file
 * @brief Class PushTheBox::AbstractScreen
 */

#include "AbstractScreenedApplication.h"

namespace PushTheBox {

/**
@brief Base for application screens

See AbstractScreenedApplication for more information.
*/
class AbstractScreen: private Corrade::Containers::LinkedListItem<AbstractScreen, AbstractScreenedApplication> {
    friend class Corrade::Containers::LinkedListItem<AbstractScreen, AbstractScreenedApplication>;
    friend class Corrade::Containers::LinkedList<AbstractScreen>;
    friend class AbstractScreenedApplication;

    public:
        /**
         * @brief Event propagated to this screen
         *
         * @see PropagatedEvents, setPropagatedEvents()
         */
        enum class PropagatedEvent: std::uint8_t {
            /**
             * Draw event.
             *
             * When enabled, drawEvent() is propagated to this screen.
             */
            Draw = 1 << 0,

            /**
             * Input events.
             *
             * When enabled, keyPressEvent(), keyReleaseEvent(), mousePressEvent(),
             * mouseReleaseEvent() and mouseMoveEvent() are propagated to this
             * screen.
             */
            Input = 1 << 1
        };

        /**
         * @brief Events propagated to this screen
         *
         * @see setPropagatedEvents()
         */
        typedef Corrade::Containers::EnumSet<PropagatedEvent, std::uint8_t> PropagatedEvents;

        /** @brief Input event */
        typedef AbstractScreenedApplication::InputEvent InputEvent;

        /** @brief Key event */
        typedef AbstractScreenedApplication::KeyEvent KeyEvent;

        /** @brief Mouse event */
        typedef AbstractScreenedApplication::MouseEvent MouseEvent;

        /** @brief Mouse move event */
        typedef AbstractScreenedApplication::MouseMoveEvent MouseMoveEvent;

        inline AbstractScreen() {}

        /** @brief Events propagated to this screen */
        inline PropagatedEvents propagatedEvents() const { return _propagatedEvents; }

        /**
         * @brief Set events propagated to this screen
         *
         * For non-propagated events related event functions are not called.
         */
        inline void setPropagatedEvents(PropagatedEvents events) { _propagatedEvents = events; }

        /** @brief %Application holding this screen */
        AbstractScreenedApplication* application() { return list(); }
        const AbstractScreenedApplication* application() const { return list(); } /**< @overload */

        /**
         * @brief Next nearer screen
         *
         * @see AbstractScreenedApplication::frontScreen(), AbstractScreenedApplication::backScreen()
         */
        AbstractScreen* nextNearerScreen() { return previous(); }
        const AbstractScreen* nextNearerScreen() const { return previous(); } /**< @overload */

        /**
         * @brief Next farther screen
         *
         * @see AbstractScreenedApplication::frontScreen(), AbstractScreenedApplication::backScreen()
         */
        AbstractScreen* nextFartherScreen() { return next(); }
        const AbstractScreen* nextFartherScreen() const { return next(); } /**< @overload */

    protected:
        /** @brief Request redraw */
        virtual void redraw() { application()->redraw(); }

        /**
         * @brief Focus event
         *
         * Called when screen is focused using AbstractScreenedApplication::focusScreen()
         * or AbstractScreenedApplication::addScreen().
         */
        virtual void focusEvent() = 0;

        /**
         * @brief Blur event
         *
         * Called when another screen is focused using AbstractScreenedApplication::focusScreen(),
         * AbstractScreenedApplication::addScreen() or before the screen is removed
         * from application using AbstractScreenedApplication::removeScreen().
         */
        virtual void blurEvent() = 0;

        /**
         * @brief Viewport event
         *
         * Called from holder application when viewport size changes. You
         * should pass the new size to your camera, if using scene graph.
         *
         * Setting viewport on default framebuffer should be done by the
         * application.
         */
        virtual void viewportEvent(const Math::Vector2<GLsizei>& size) = 0;

        /**
         * @brief Draw event
         *
         * Called when @ref PropagatedEvent "PropagatedEvent::Draw" is enabled.
         * Here implement your drawing functions, such as calling
         * SceneGraph::AbstractCamera::draw(). If you want to draw immediately
         * again, call also redraw().
         *
         * Buffer swapping and clearing of default framebufer should be done
         * by the application.
         */
        virtual void drawEvent() = 0;

        /**
         * @brief Key press event
         *
         * Called when @ref PropagatedEvent "PropagatedEvent::Input" is enabled
         * and an key is pressed. Default implementation does nothing.
         */
        virtual void keyPressEvent(KeyEvent& event);

        /**
         * @brief Key release event
         *
         * Called when @ref PropagatedEvent "PropagatedEvent::Input" is enabled
         * and an key is released. Default implementation does nothing.
         */
        virtual void keyReleaseEvent(KeyEvent& event);

        /**
         * @brief Mouse press event
         *
         * Called when @ref PropagatedEvent "PropagatedEvent::Input" is enabled
         * and mouse button is pressed. Default implementation does nothing.
         */
        virtual void mousePressEvent(MouseEvent& event);

        /**
         * @brief Mouse release event
         *
         * Called when @ref PropagatedEvent "PropagatedEvent::Input" is enabled
         * and mouse button is released. Default implementation does nothing.
         */
        virtual void mouseReleaseEvent(MouseEvent& event);

        /**
         * @brief Mouse move event
         *
         * Called when @ref PropagatedEvent "PropagatedEvent::Input" is enabled
         * and mouse is moved. Default implementation does nothing.
         */
        virtual void mouseMoveEvent(MouseMoveEvent& event);

    private:
        PropagatedEvents _propagatedEvents;
};

CORRADE_ENUMSET_OPERATORS(AbstractScreen::PropagatedEvents)

inline void AbstractScreen::keyPressEvent(KeyEvent&) {}
inline void AbstractScreen::keyReleaseEvent(KeyEvent&) {}
inline void AbstractScreen::mousePressEvent(MouseEvent&) {}
inline void AbstractScreen::mouseReleaseEvent(MouseEvent&) {}
inline void AbstractScreen::mouseMoveEvent(MouseMoveEvent&) {}

}

#endif
