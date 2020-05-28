#include "MouseProxy.h"
#include "MouseEvent.h"

namespace Sprocket {

MouseProxy::MouseProxy()
    : d_position({0.0f, 0.0f})
    , d_offsetSum({0.0f, 0.0f})
    , d_offset({0.0f, 0.0f})
{
    
}

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

    else if (auto e = event.as<MouseMovedEvent>()) {
        Sprocket::Maths::vec2 p = {e->xPos(), e->yPos()};
        d_offsetSum = p - d_position;
        d_position = p;
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

void MouseProxy::update()
{
    d_offset = d_offsetSum;
    d_offsetSum = Maths::vec2(0.0f, 0.0f);
}

}