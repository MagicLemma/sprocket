#pragma once
#include "Camera.h"
#include "Event.h"

#include <glm/glm.hpp>

namespace Sprocket {

class Camera
{
    glm::vec3 d_position;
    float     d_pitch;
    float     d_yaw;
    float     d_roll;

public:
    Camera();

    void move();

    glm::vec3 position() const { return d_position; }
    float pitch() const { return d_pitch; }
    float yaw() const { return d_yaw; }
    float roll() const { return d_roll; }
};

}