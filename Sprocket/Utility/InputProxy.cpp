#include "InputProxy.h"
#include "KeyboardEvent.h"
#include "MouseEvent.h"
#include "Event.h"

namespace Sprocket {
    
void InputProxy::OnEvent(Event& event)
{
    if (event.In<EventCategory::KEYBOARD>()) {
        if (auto e = event.As<KeyboardButtonPressedEvent>()) {
            if (event.IsConsumed()) { return; }
            d_keys[e->Key()] = true;
            if (d_consumedKeys.contains(e->Key())) { e->Consume(); }
        }
        else if (auto e = event.As<KeyboardButtonReleasedEvent>()) {
            d_keys[e->Key()] = false;
        }
    }
    else if (event.In<EventCategory::MOUSE>()) {
        if (auto e = event.As<MouseButtonPressedEvent>()) {
            if (event.IsConsumed()) { return; }
            d_buttons[e->Button()] = true;
        }
        else if (auto e = event.As<MouseButtonReleasedEvent>()) {
            d_buttons[e->Button()] = false;
        }
    }
}

bool InputProxy::IsMouseDown(int button) const
{
    return d_buttons[button];
}

bool InputProxy::IsKeyboardDown(int key) const
{
    if (auto it = d_keys.find(key); it != d_keys.end()) {
        return it->second;
    }
    return false;
}

void InputProxy::ConsumeEventsFor(int key)
{
    d_consumedKeys.insert(key);
}

}