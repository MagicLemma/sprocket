#include "MouseProxy.h"
#include "MouseEvent.h"
#include "Log.h"

namespace Sprocket {

MouseProxy::MouseProxy()
    : d_position({0.0f, 0.0f})
    , d_offsetSum({0.0f, 0.0f})
    , d_offset({0.0f, 0.0f})
    , d_pressedButtons({false, false, false, false, false})
{
    
}

void MouseProxy::OnEvent(Event& event)
{
    if (!event.In<EventCategory::MOUSE>()) { return; }

    if (auto e = event.As<MouseButtonPressedEvent>()) {
        if (event.IsConsumed()) { return; }
        d_pressedButtons[e->Button()] = true;
    }

    else if (auto e = event.As<MouseButtonReleasedEvent>()) {
        d_pressedButtons[e->Button()] = false;
    }

    else if (auto e = event.As<MouseMovedEvent>()) {
        Sprocket::Maths::vec2 p = {e->XPos(), e->YPos()};
        d_offsetSum = p - d_position;
        d_position = p;
    }
}

bool MouseProxy::IsButtonDown(int key) const
{
    return d_pressedButtons[key];
}

void MouseProxy::OnUpdate()
{
    d_offset = d_offsetSum;
    d_offsetSum = Maths::vec2(0.0f, 0.0f);
}

}