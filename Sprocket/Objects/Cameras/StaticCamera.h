#pragma once
#include "Maths.h"
#include "Camera.h"

namespace Sprocket {

class StaticCamera : public Camera
// A simple implementation of the camera interface.
{
    Maths::vec3 d_position;
    float       d_pitch;
    float       d_yaw;

public:
    StaticCamera(const Maths::vec3& position, float pitch, float yaw);

    Maths::mat4 View() const override;
    Maths::vec3 Position() const override { return d_position; }

    void SetPosition(const Maths::vec3& p) { d_position = p; }
    void SetPitch(float theta) { d_pitch = theta; }
    void SetYaw(float theta) { d_yaw = theta; }

    float Pitch() const { return d_pitch; }
    float Yaw() const { return d_yaw; }
};

}