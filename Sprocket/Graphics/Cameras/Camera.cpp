#include "Camera.h"
#include "Maths.h"

namespace Sprocket {

Camera::Camera()
    : d_position(Maths::vec3(0.0f, 0.0f, 0.0f))
    , d_direction(Maths::vec3(0.0f, 0.0f, -1.0f))
    , d_pitch(0)
    , d_yaw(0)
    , d_roll(0)
{}

}