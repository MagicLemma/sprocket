#include "input_system.h"
#include "Components.h"
#include "Scene.h"

namespace spkt {

void input_system_on_event(apx::registry& registry, ev::Event& event)
{
    auto& input = get_singleton<InputSingleton>(registry);
    if (auto data = event.get_if<ev::KeyboardButtonPressed>()) {
        if (!event.is_consumed()) {
            input.keyboard[data->key] = true;
        }
    }
    else if (auto data = event.get_if<ev::KeyboardButtonReleased>()) {
        input.keyboard[data->key] = false;
    }
    else if (auto data = event.get_if<ev::MouseButtonPressed>()) {
        if (!event.is_consumed()) { 
            input.mouse[data->button] = true;
            input.mouse_click[data->button] = true;
        }
    }
    else if (auto data = event.get_if<ev::MouseButtonReleased>()) {
        input.mouse[data->button] = false;
        input.mouse_unclick[data->button] = true;
    }
    else if (auto data = event.get_if<ev::MouseScrolled>()) {
        input.mouse_scrolled.x += data->x_offset;
        input.mouse_scrolled.y += data->y_offset;
    }
    else if (auto data = event.get_if<ev::WindowResize>()) {
        input.window_resized = true;
        input.window_width = (float)data->width;
        input.window_height = (float)data->height;
    }
    else if (auto data = event.get_if<ev::MouseMoved>()) {
        input.mouse_pos = {data->x_pos, data->y_pos};
    }
}

void input_system_begin(apx::registry& registry, double dt)
{
    auto& input = get_singleton<InputSingleton>(registry);
    input.mouse_offset = input.mouse_pos - input.mouse_pos_last_frame;
}

void input_system_end(apx::registry& registry, double dt)
{
    auto& input = get_singleton<InputSingleton>(registry);
    input.mouse_click.fill(false);
    input.mouse_unclick.fill(false);
    input.mouse_scrolled = {0.0, 0.0};
    input.mouse_pos_last_frame = input.mouse_pos;
}
    
}