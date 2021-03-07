#include "InputProxy.h"
#include "Events.h"

namespace Sprocket {
    
void InputProxy::OnEvent(ev::Event& event)
{
    if (auto data = event.get_if<ev::KeyboardButtonPressed>()) {
        if (event.is_consumed()) { return; }
        d_keys[data->key] = true;
        if (d_consumedKeys.contains(data->key)) { event.consume(); }
    }
    else if (auto data = event.get_if<ev::KeyboardButtonReleased>()) {
        d_keys[data->key] = false;
    }
    else if (auto data = event.get_if<ev::MouseButtonPressed>()) {
        if (event.is_consumed()) { return; }
        d_buttons[data->button] = true;
    }
    else if (auto data = event.get_if<ev::MouseButtonReleased>()) {
        d_buttons[data->button] = false;
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