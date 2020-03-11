#pragma once
#include "Maths.h"
#include "Window.h"
#include "Event.h"

namespace Sprocket {

class Camera
{
protected:
    Maths::vec3 d_position;

    Maths::vec3 d_direction;
        // Vector that points in the direction of the camera. This
        // should ideally be a unit vector.

    float d_pitch;
    float d_yaw;
    float d_roll;
        // Euler angles. These could be derived from the direction
        // and so should be kepy in sync.

public:
    Camera();

    virtual void update(Window* window, float timeDelta) = 0;
        // To be implemented, how this camera should update every tick.

    virtual void handleEvent(Window* window, const Event& event) {}
        // Optional override to handle events.

    virtual Maths::mat4 projectionMatrix() const = 0;
        // To be implemented, should the return the projection matrix.

    Maths::vec3 position() const { return d_position; }
    Maths::vec3 direction() const { return d_direction; }

    float pitch() const { return d_pitch; }
    float yaw() const { return d_yaw; }
    float roll() const { return d_roll; }
};

}