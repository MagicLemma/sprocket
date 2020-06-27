#include "MouseProxy.h"
#include "MouseEvent.h"
#include "Log.h"

namespace Sprocket {

MouseProxy::MouseProxy()
    : d_position({0.0f, 0.0f})
    , d_offsetSum({0.0f, 0.0f})
    , d_offset({0.0f, 0.0f})
    , d_pressedButtonsA({false, false, false, false, false})
    , d_pressedButtonsB({false, false, false, false, false})
    , d_pressedButtonsC({false, false, false, false, false})
{
    
}

void MouseProxy::OnEvent(Event& event)
{
    if (!event.In<EventCategory::MOUSE>()) { return; }

    if (auto e = event.As<MouseButtonPressedEvent>()) {
        if (event.IsConsumed()) { return; }
        d_pressedButtonsA[e->Button()] = true;
    }

    else if (auto e = event.As<MouseButtonReleasedEvent>()) {
        d_pressedButtonsA[e->Button()] = false;
    }

    else if (auto e = event.As<MouseMovedEvent>()) {
        Sprocket::Maths::vec2 p = {e->XPos(), e->YPos()};
        d_offsetSum = p - d_position;
        d_position = p;
    }
}

bool MouseProxy::IsButtonDown(int key) const
{
    return d_pressedButtonsB[key];
}

void MouseProxy::OnUpdate()
{
    d_pressedButtonsC = d_pressedButtonsB;
    d_pressedButtonsB = d_pressedButtonsA;

    d_offset = d_offsetSum;
    d_offsetSum = Maths::vec2(0.0f, 0.0f);
}

}