#include "Graphics/Camera.h"

#include <cmath>
#include <algorithm>

namespace Sprocket {

Camera::Camera()
    : d_position(Maths::vec3(0.0f, 0.0f, 0.0f))
    , d_direction(Maths::vec3(0.0f, 0.0f, -1.0f))
    , d_pitch(0)
    , d_yaw(0)
    , d_roll(0)
    , d_sensitivity(0.15f)
{
}

void Camera::move(Window* window, bool active)
{
    using namespace Maths;
    // Updated at the end of the function.
    static vec2 prevMousePos = window->getMousePos();
    vec2 mousePos = window->getMousePos();
    vec2 offset = mousePos - prevMousePos;

    float speed = 0.02f;

    if (active) {
        vec3 forwards = d_direction;
        forwards.y = 0;
        normalise(forwards);

        vec3 up = {0.0f, 1.0f, 0.0f};
        vec3 right = cross(forwards, up);
        
        if (window->isKeyDown(Keyboard::W)){
            d_position += speed * forwards;
        }
        if (window->isKeyDown(Keyboard::D)){
            d_position += speed * right;
        }
        if (window->isKeyDown(Keyboard::A)){
            d_position -= speed * right;
        }
        if (window->isKeyDown(Keyboard::S)){
            d_position -= speed * forwards;
        }
        if (window->isKeyDown(Keyboard::SPACE)){
            d_position += speed * up;
        }
        if (window->isKeyDown(Keyboard::LSHIFT)){
            d_position -= speed * up;
        }

        d_yaw += d_sensitivity * offset.x;
        d_pitch += d_sensitivity * offset.y;
        clamp(d_pitch, -89.0, 89.0);

        d_direction.x = sind(d_yaw) * cosd(d_pitch);
        d_direction.y = -sind(d_pitch);
        d_direction.z = -cosd(d_yaw) * cosd(d_pitch);
        normalise(d_direction);
    }

    prevMousePos = mousePos;
}

}