#include "FirstPersonCamera.h"

#include <cmath>
#include <algorithm>

namespace Sprocket {

FirstPersonCamera::FirstPersonCamera()
    : Camera()
    , d_position(Maths::vec3(0.0f, 0.0f, 0.0f))
    , d_direction(Maths::vec3(0.0f, 0.0f, -1.0f))
    , d_pitch(0)
    , d_yaw(0)
    , d_roll(0)
    , d_sensitivity(0.15f)
{
}

Maths::mat4 FirstPersonCamera::View() const
{
    return Maths::View(d_position, d_pitch, d_yaw, d_roll);
}

void FirstPersonCamera::OnUpdate(float timeDelta)
{
    d_mouse.OnUpdate();

    float speed = 10.0f * timeDelta;

    Maths::vec3 forwards = d_direction;
    forwards.y = 0;
    Maths::Normalise(forwards);

    Maths::vec3 up = {0.0f, 1.0f, 0.0f};
    Maths::vec3 right = cross(forwards, up);
    
    if (d_keyboard.IsKeyDown(Keyboard::W)){
        d_position += speed * forwards;
    }
    if (d_keyboard.IsKeyDown(Keyboard::S)){
        d_position -= speed * forwards;
    }
    if (d_keyboard.IsKeyDown(Keyboard::D)){
        d_position += speed * right;
    }
    if (d_keyboard.IsKeyDown(Keyboard::A)){
        d_position -= speed * right;
    }
    
    if (d_keyboard.IsKeyDown(Keyboard::SPACE)){
        d_position += speed * up;
    }
    if (d_keyboard.IsKeyDown(Keyboard::LSHIFT)){
        d_position -= speed * up;
    }

    Maths::vec2 offset = d_mouse.GetMouseOffset();
    if (offset != Maths::vec2{0.0, 0.0}) {
        d_yaw += d_sensitivity * offset.x;
        d_pitch += d_sensitivity * offset.y;
        Maths::Clamp(d_pitch, -89.0, 89.0);
    }

    d_direction.x = Maths::Sind(d_yaw) * Maths::Cosd(d_pitch);
    d_direction.y = -Maths::Sind(d_pitch);
    d_direction.z = -Maths::Cosd(d_yaw) * Maths::Cosd(d_pitch);
    Maths::Normalise(d_direction);
}

void FirstPersonCamera::OnEvent(Event& event)
{
    d_keyboard.OnEvent(event);
    d_mouse.OnEvent(event);
}

}