#pragma once
#include "Maths.h"
#include "Camera.h"

namespace Sprocket {

class StaticCamera : public Camera
// A simple implementation of the camera interface.
{
    Maths::vec3 d_position;
    Maths::quat d_orientation;

public:
    StaticCamera(const Maths::vec3& position);

    Maths::mat4 View() const override;
    Maths::vec3 Position() const override { return d_position; }

    void SetPosition(const Maths::vec3& p) { d_position = p; }
    void SetOrientation(const Maths::quat& q) { d_orientation = q; }

};

}