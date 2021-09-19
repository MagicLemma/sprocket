#include "InputProxy.h"

#include <Sprocket/Core/events.h>

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

void input_store::on_event(spkt::event& event)
{
    if (auto data = event.get_if<keyboard_pressed_event>()) {
        if (event.is_consumed()) { return; }
        d_keyboard[data->key] = true;
        d_keyboard_click[data->key] = true;
    }
    else if (auto data = event.get_if<keyboard_released_event>()) {
        d_keyboard[data->key] = false;
        d_keyboard_unclick[data->key] = true;
    }
    else if (auto data = event.get_if<mouse_pressed_event>()) {
        if (event.is_consumed()) { return; }
        d_mouse[data->button] = true;
        d_mouse_click[data->button] = true;
    }
    else if (auto data = event.get_if<mouse_released_event>()) {
        d_mouse[data->button] = false;
        d_mouse_unclick[data->button] = true;
    }
}

void input_store::end_frame()
{
    d_keyboard_click.fill(false);
    d_keyboard_unclick.fill(false);
    
    d_mouse_click.fill(false);
    d_mouse_unclick.fill(false);
}

}