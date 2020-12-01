#include "MouseProxy.h"
#include "MouseEvent.h"
#include "Log.h"

namespace Sprocket {

MouseProxy::MouseProxy()
    : d_pressed({false, false, false, false, false})
{
}

void MouseProxy::OnEvent(Event& event)
{
    if (!event.In<EventCategory::MOUSE>()) { return; }

    if (auto e = event.As<MouseButtonPressedEvent>()) {
        if (event.IsConsumed()) { return; }
        d_pressed[e->Button()] = true;
    }
    else if (auto e = event.As<MouseButtonReleasedEvent>()) {
        d_pressed[e->Button()] = false;
    }
}

bool MouseProxy::IsButtonDown(int key) const
{
    return d_pressed[key];
}

}