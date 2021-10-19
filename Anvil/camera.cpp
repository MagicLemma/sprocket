#include "Camera.h"

#include <Sprocket/Core/events.h>
#include <Sprocket/Core/Window.h>
#include <Sprocket/Graphics/Viewport.h>
#include <Sprocket/Core/input_codes.h>

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/trigonometric.hpp>

#include <algorithm>

namespace anvil {

camera::camera(spkt::window* window, const glm::vec3& target)
    : d_window(window)
    , d_input(window)
    , d_position()
    , d_target(target)
    , d_yaw(0.0f)
    , d_distance(8.0f)
    , d_move_speed(10.0f)
    , d_rotate_speed(glm::half_pi<float>())
    , d_abs_vert(2.0f)
    , d_abs_min(2.0f)
    , d_abs_max(10.0f)
{
}

void camera::on_update(double dt)
{
    using namespace spkt;

    float rotate_speed = d_rotate_speed * dt;
    float move_speed = d_move_speed * dt;

    auto f = d_target - d_position;
    f.y = 0;
    f = glm::normalize(f);

    glm::vec3 up = {0, 1, 0};
    glm::vec3 r = glm::cross(f, up);

    if (d_input.is_keyboard_down(Keyboard::W)) {
        d_target += move_speed * f;
    }
    if (d_input.is_keyboard_down(Keyboard::S)) {
        d_target -= move_speed * f;
    }
    if (d_input.is_keyboard_down(Keyboard::D)) {
        d_target += move_speed * r;
    }
    if (d_input.is_keyboard_down(Keyboard::A)) {
        d_target -= move_speed * r;
    }

    if (d_input.is_keyboard_down(Keyboard::E)) {
        d_yaw -= rotate_speed;
    }
    if (d_input.is_keyboard_down(Keyboard::Q)) {
        d_yaw += rotate_speed;
    }

    if (d_input.is_keyboard_down(Keyboard::SPACE)) {
        d_position.y += d_move_speed * dt;
        d_target.y += d_move_speed * dt;
        d_abs_vert += d_move_speed * dt;
        d_abs_min += d_move_speed * dt;
        d_abs_max += d_move_speed * dt;
    }
    if (d_input.is_keyboard_down(Keyboard::LSHIFT)) {
        d_position.y -= d_move_speed * dt;
        d_target.y -= d_move_speed * dt;
        d_abs_vert -= d_move_speed * dt;
        d_abs_min -= d_move_speed * dt;
        d_abs_max -= d_move_speed * dt;
    }

    d_position.x = d_target.x + d_distance * glm::cos(d_yaw);
    d_position.z = d_target.z + d_distance * glm::sin(d_yaw);

    if (d_position.y != d_abs_vert) {
        float d = d_abs_vert - d_position.y;
        d_position.y += 2 * d * dt;
    }
}

void camera::on_event(spkt::event& event)
{
    d_input.on_event(event);

    if (auto data = event.get_if<spkt::mouse_scrolled_event>()) {
        if (event.is_consumed()) { return; }
        d_abs_vert -= data->y_offset;
        d_abs_vert = std::clamp(d_abs_vert, d_abs_min, d_abs_max);
        event.consume();
    }
}

glm::mat4 camera::proj() const
{
    const float aspect_ratio = spkt::viewport::current_aspect_ratio();
    return glm::perspective(70.0f, aspect_ratio, 0.1f, 1000.0f);
}

glm::mat4 camera::view() const
{
    return glm::lookAt(d_position, d_target, {0.0, 1.0, 0.0});
}

}