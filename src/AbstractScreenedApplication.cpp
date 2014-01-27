#include "AbstractScreenedApplication.h"

#include <Magnum/DefaultFramebuffer.h>

#include "AbstractScreen.h"

namespace PushTheBox {

AbstractScreenedApplication& AbstractScreenedApplication::addScreen(AbstractScreen* screen) {
    insert(screen);
    if(frontScreen() == screen) screen->focusEvent();
    return *this;
}

AbstractScreenedApplication& AbstractScreenedApplication::removeScreen(AbstractScreen* screen) {
    screen->blurEvent();
    erase(screen);
    return *this;
}

AbstractScreenedApplication& AbstractScreenedApplication::focusScreen(AbstractScreen* screen) {
    /* Already focused, nothing to do */
    if(frontScreen() == screen) return *this;

    frontScreen()->blurEvent();
    move(screen, frontScreen());
    screen->focusEvent();
    return *this;
}

void AbstractScreenedApplication::viewportEvent(const Vector2i& size) {
    defaultFramebuffer.setViewport({{}, size});

    for(AbstractScreen* s = first(); s; s = s->next())
        s->viewportEvent(size);
}

void AbstractScreenedApplication::drawEvent() {
    /* Back-to-front rendering */
    for(AbstractScreen* s = backScreen(); s; s = s->nextNearerScreen())
        if(s->propagatedEvents() & AbstractScreen::PropagatedEvent::Draw) s->drawEvent();
}

void AbstractScreenedApplication::keyPressEvent(KeyEvent& event) {
    /* Front-to-back event propagation, stop when the event gets accepted */
    for(AbstractScreen* s = frontScreen(); s; s = s->nextFartherScreen()) {
        if(s->propagatedEvents() & AbstractScreen::PropagatedEvent::Input) {
            s->keyPressEvent(event);
            if(event.isAccepted()) break;
        }
    }
}

void AbstractScreenedApplication::keyReleaseEvent(KeyEvent& event) {
    /* Front-to-back event propagation, stop when the event gets accepted */
    for(AbstractScreen* s = frontScreen(); s; s = s->nextFartherScreen()) {
        if(s->propagatedEvents() & AbstractScreen::PropagatedEvent::Input) {
            s->keyReleaseEvent(event);
            if(event.isAccepted()) break;
        }
    }
}

void AbstractScreenedApplication::mousePressEvent(MouseEvent& event) {
    /* Front-to-back event propagation, stop when the event gets accepted */
    for(AbstractScreen* s = frontScreen(); s; s = s->nextFartherScreen()) {
        if(s->propagatedEvents() & AbstractScreen::PropagatedEvent::Input) {
            s->mousePressEvent(event);
            if(event.isAccepted()) break;
        }
    }
}

void AbstractScreenedApplication::mouseReleaseEvent(MouseEvent& event) {
    /* Front-to-back event propagation, stop when the event gets accepted */
    for(AbstractScreen* s = frontScreen(); s; s = s->nextFartherScreen()) {
        if(s->propagatedEvents() & AbstractScreen::PropagatedEvent::Input) {
            s->mouseReleaseEvent(event);
            if(event.isAccepted()) break;
        }
    }
}

void AbstractScreenedApplication::mouseMoveEvent(MouseMoveEvent& event) {
    /* Front-to-back event propagation, stop when the event gets accepted */
    for(AbstractScreen* s = frontScreen(); s; s = s->nextFartherScreen()) {
        if(s->propagatedEvents() & AbstractScreen::PropagatedEvent::Input) {
            s->mouseMoveEvent(event);
            if(event.isAccepted()) break;
        }
    }
}

}
