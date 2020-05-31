#include "KeyboardProxy.h"
#include "KeyboardEvent.h"

namespace Sprocket {

void KeyboardProxy::OnEvent(Event& event)
{
    if (!event.In<EventCategory::KEYBOARD>()) { return; }

    if (auto e = event.As<KeyboardButtonPressedEvent>()) {
        if (event.IsConsumed()) { return; }
        d_pressedKeys[e->Key()] = true;
        e->Consume();
    }

    else if (auto e = event.As<KeyboardButtonReleasedEvent>()) {
        d_pressedKeys[e->Key()] = false;
        e->Consume();
    }
}

bool KeyboardProxy::IsKeyDown(int key) const
{
    auto it = d_pressedKeys.find(key);
    if (it != d_pressedKeys.end()) {
        return it->second;
    }
    return false;
}

}