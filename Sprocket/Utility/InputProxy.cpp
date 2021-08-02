#include "InputProxy.h"

#include <Sprocket/Core/Events.h>

#include <cassert>

namespace spkt {

InputProxy::InputProxy()
{
    d_keyboard.fill(false);
    d_mouse.fill(false);
}
  
void InputProxy::on_event(spkt::event& event)
{
    if (auto data = event.get_if<keyboard_pressed_event>()) {
        if (event.is_consumed()) { return; }
        d_keyboard[data->key] = true;
    }
    else if (auto data = event.get_if<keyboard_released_event>()) {
        d_keyboard[data->key] = false;
    }
    else if (auto data = event.get_if<mouse_pressed_event>()) {
        if (event.is_consumed()) { return; }
        d_mouse[data->button] = true;
    }
    else if (auto data = event.get_if<mouse_released_event>()) {
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