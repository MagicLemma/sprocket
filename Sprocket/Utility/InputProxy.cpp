#include "InputProxy.h"
#include "Events.h"

#include <cassert>

namespace Sprocket {

InputProxy::InputProxy()
{
    d_keyboard.fill(false);
    d_mouse.fill(false);
}

void InputProxy::on_startup(ev::Dispatcher& dispatcher)
{
    dispatcher.subscribe<ev::KeyboardButtonPressed>([&](ev::Event& event) {
        if (event.is_consumed()) { return; }
        d_keyboard[event.get<ev::KeyboardButtonPressed>().key] = true;
    });

    dispatcher.subscribe<ev::KeyboardButtonReleased>([&](ev::Event& event) {
        d_keyboard[event.get<ev::KeyboardButtonReleased>().key] = false;
    });

    dispatcher.subscribe<ev::MouseButtonPressed>([&](ev::Event& event) {
        if (event.is_consumed()) { return; }
        d_mouse[event.get<ev::MouseButtonPressed>().button] = true;
    });

    dispatcher.subscribe<ev::MouseButtonReleased>([&](ev::Event& event) {
        d_mouse[event.get<ev::MouseButtonReleased>().button] = false;
    });
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