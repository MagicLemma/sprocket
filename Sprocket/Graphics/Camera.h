#pragma once
#include "Core/Window.h"
#include "Graphics/Camera.h"
#include "Events/Event.h"
#include "Utility/Maths.h"

namespace Sprocket {

class Camera
{
    Maths::vec3 d_position;
    float       d_pitch;
    float       d_yaw;
    float       d_roll;

    float       d_sensitivity; // Mouse sensitivity

public:
    Camera();

    void move(Window* window, bool active);

    Maths::vec3 position() const { return d_position; }
    float pitch() const { return d_pitch; }
    float yaw() const { return d_yaw; }
    float roll() const { return d_roll; }
};

}