#include "InputProxy.h"
#include "Events.h"

#include <cassert>

namespace spkt {

InputProxy::InputProxy()
{
    d_keyboard.fill(false);
    d_mouse.fill(false);
}
  
void InputProxy::on_event(ev::Event& event)
{
    if (auto data = event.get_if<ev::KeyboardButtonPressed>()) {
        if (event.is_consumed()) { return; }
        d_keyboard[data->key] = true;
    }
    else if (auto data = event.get_if<ev::KeyboardButtonReleased>()) {
        d_keyboard[data->key] = false;
    }
    else if (auto data = event.get_if<ev::MouseButtonPressed>()) {
        if (event.is_consumed()) { return; }
        d_mouse[data->button] = true;
    }
    else if (auto data = event.get_if<ev::MouseButtonReleased>()) {
        d_mouse[data->button] = false;
    }
}

bool InputProxy::is_mouse_down(int button) const
{
    return d_mouse[button];
}

bool InputProxy::is_keyboard_down(int key) const
{
    assert(key < NUM_KEYS);
    return d_keyboard[key];
}

}