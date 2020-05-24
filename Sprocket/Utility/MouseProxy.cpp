#include "MouseProxy.h"
#include "MouseEvent.h"

namespace Sprocket {

void MouseProxy::handleEvent(Event& event)
{
    if (!event.in<EventCategory::MOUSE>()) { return; }

    if (auto e = event.as<MouseButtonPressedEvent>()) {
        if (event.isConsumed()) { return; }
        d_pressedButtons[e->button()] = true;
    }

    else if (auto e = event.as<MouseButtonReleasedEvent>()) {
        d_pressedButtons[e->button()] = false;
    }
}

bool MouseProxy::isButtonDown(int key) const
{
    auto it = d_pressedButtons.find(key);
    if (it != d_pressedButtons.end()) {
        return it->second;
    }
    return false;
}

}