#include "MouseProxy.h"
#include "MouseEvent.h"
#include "Log.h"

namespace Sprocket {

MouseProxy::MouseProxy()
    : d_pressedTemp({false, false, false, false, false})
    , d_pressedCurr({false, false, false, false, false})
    , d_pressedPrev({false, false, false, false, false})
{
    
}

void MouseProxy::OnEvent(Event& event)
{
    if (!event.In<EventCategory::MOUSE>()) { return; }

    if (auto e = event.As<MouseButtonPressedEvent>()) {
        if (event.IsConsumed()) { return; }
        d_pressedTemp[e->Button()] = true;
    }

    else if (auto e = event.As<MouseButtonReleasedEvent>()) {
        d_pressedTemp[e->Button()] = false;
    }

    if (d_consumeEvents) {
        event.Consume();
    }
}

bool MouseProxy::IsButtonDown(int key) const
{
    return d_pressedCurr[key];
}

void MouseProxy::OnUpdate()
{
    d_pressedPrev = d_pressedCurr;
    d_pressedCurr = d_pressedTemp;
}

bool MouseProxy::IsButtonClicked(int button) const
{
    return d_pressedCurr[button] && !d_pressedPrev[button];
}

bool MouseProxy::IsButtonReleased(int button) const
{
    return !d_pressedCurr[button] && d_pressedPrev[button];
}

}