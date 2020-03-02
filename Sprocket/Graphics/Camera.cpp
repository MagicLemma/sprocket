#include "Graphics/Camera.h"

#include <cmath>
#include <algorithm>

namespace Sprocket {

Camera::Camera()
    : d_position(Maths::vec3(0.0f, 0.0f, 0.0f))
    , d_pitch(0)
    , d_yaw(0)
    , d_roll(0)
    , d_sensitivity(0.15f)
{
}

void Camera::move(Window* window, bool active)
{
    // Updated at the end of the function.
    static Maths::vec2 prevMousePos = window->getMousePos();
    Maths::vec2 mousePos = window->getMousePos();

    if (active) {
        if (window->isKeyDown(Keyboard::W))
        {
            d_position.z += -std::cos(Maths::radians(d_yaw)) * 0.02f;
            d_position.x += std::sin(Maths::radians(d_yaw)) * 0.02f;
        }
        if (window->isKeyDown(Keyboard::D))
        {
            d_position.z += std::sin(Maths::radians(d_yaw)) * 0.02f;
            d_position.x += std::cos(Maths::radians(d_yaw)) * 0.02f;
        }
        if (window->isKeyDown(Keyboard::A))
        {
            d_position.z += -std::sin(Maths::radians(d_yaw)) * 0.02f;
            d_position.x += -std::cos(Maths::radians(d_yaw)) * 0.02f;
        }
        if (window->isKeyDown(Keyboard::S))
        {
            d_position.z += std::cos(Maths::radians(d_yaw)) * 0.02f;
            d_position.x += -std::sin(Maths::radians(d_yaw)) * 0.02f;
        }
        if (window->isKeyDown(Keyboard::SPACE))
        {
            d_position.y += 0.02f;
        }
        if (window->isKeyDown(Keyboard::LSHIFT))
        {
            d_position.y -= 0.02f;
        }

        float pitchDiff = d_sensitivity * (mousePos.y - prevMousePos.y);
        d_pitch = std::min(std::max(d_pitch + pitchDiff, -90.0f), 90.0f);
        d_yaw += d_sensitivity * (mousePos.x - prevMousePos.x);
    }

    prevMousePos = window->getMousePos();
}

}