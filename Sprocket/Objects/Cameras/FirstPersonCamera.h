#pragma once
#include "Core/Window.h"
#include "Events/Event.h"
#include "Utility/Maths.h"
#include "Camera.h"

namespace Sprocket {

class FirstPersonCamera : public Camera
{
    Maths::vec3 d_position;
        // The position of the camera in the world.

    Maths::vec3 d_direction;
        // Vector that points in the direction of the camera. This
        // should ideally be a unit vector.

    float d_pitch;
    float d_yaw;
    float d_roll;
        // Euler angles. These could be derived from the direction
        // and so should be kepy in sync.

    float d_sensitivity;
        // Mouse sensitivity

public:
    FirstPersonCamera();

    Maths::mat4 view() const override;
    void update(Window* window, float timeDelta) override;

    Maths::vec3 position() const { return d_position; }
    Maths::vec3 direction() const { return d_direction; }

    float pitch() const { return d_pitch; }
    float yaw() const { return d_yaw; }
    float roll() const { return d_roll; }
};

}