#include "input_system.h"

#include <Sprocket/Core/Events.h>
#include <Sprocket/Scene/ecs.h>
#include <Sprocket/Scene/Scene.h>

namespace spkt {

void input_system_on_event(spkt::registry& registry, spkt::event& event)
{
    auto& input = get_singleton<InputSingleton>(registry);
    if (auto data = event.get_if<KeyboardButtonPressed>()) {
        if (!event.is_consumed()) {
            input.keyboard[data->key] = true;
        }
    }
    else if (auto data = event.get_if<KeyboardButtonReleased>()) {
        input.keyboard[data->key] = false;
    }
    else if (auto data = event.get_if<MouseButtonPressed>()) {
        if (!event.is_consumed()) { 
            input.mouse[data->button] = true;
            input.mouse_click[data->button] = true;
        }
    }
    else if (auto data = event.get_if<MouseButtonReleased>()) {
        input.mouse[data->button] = false;
        input.mouse_unclick[data->button] = true;
    }
    else if (auto data = event.get_if<MouseScrolled>()) {
        input.mouse_scrolled.x += data->x_offset;
        input.mouse_scrolled.y += data->y_offset;
    }
    else if (auto data = event.get_if<WindowResize>()) {
        input.window_resized = true;
        input.window_width = (float)data->width;
        input.window_height = (float)data->height;
    }
    else if (auto data = event.get_if<MouseMoved>()) {
        input.mouse_pos = {data->x_pos, data->y_pos};
    }
}

void input_system_begin(spkt::registry& registry, double dt)
{
    auto& input = get_singleton<InputSingleton>(registry);
    input.mouse_offset = input.mouse_pos - input.mouse_pos_last_frame;
}

void input_system_end(spkt::registry& registry, double dt)
{
    auto& input = get_singleton<InputSingleton>(registry);
    input.mouse_click.fill(false);
    input.mouse_unclick.fill(false);
    input.mouse_scrolled = {0.0, 0.0};
    input.mouse_pos_last_frame = input.mouse_pos;
}
    
}