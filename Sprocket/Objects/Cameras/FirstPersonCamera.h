#pragma once
#include "Event.h"
#include "Maths.h"
#include "Camera.h"
#include "KeyboardProxy.h"
#include "MouseProxy.h"

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

    KeyboardProxy d_keyboard;
    MouseProxy d_mouse;

public:
    FirstPersonCamera();

    Maths::mat4 View() const override;
    Maths::vec3 Position() const override { return d_position; }

    void OnUpdate(double timeDelta) override;
    void OnEvent(Event& event) override;
    
    Maths::vec3 Direction() const { return d_direction; }

    float Pitch() const { return d_pitch; }
    float Yaw() const { return d_yaw; }
    float Roll() const { return d_roll; }
};

}