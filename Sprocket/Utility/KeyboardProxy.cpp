#include "KeyboardProxy.h"
#include "KeyboardEvent.h"

namespace Sprocket {

void KeyboardProxy::handleEvent(Event& event)
{
    if (!event.isInCategory(EventCategory::KEYBOARD)) { return; }

    if (auto e = event.as<KeyboardButtonPressedEvent>()) {
        if (event.isConsumed()) { return; }
        d_pressedKeys[e->key()] = true;
        e->consume();
    }

    else if (auto e = event.as<KeyboardButtonReleasedEvent>()) {
        d_pressedKeys[e->key()] = false;
        e->consume();
    }
}

bool KeyboardProxy::isKeyDown(int key) const
{
    auto it = d_pressedKeys.find(key);
    if (it != d_pressedKeys.end()) {
        return it->second;
    }
    return false;
}

}