#include "StaticCamera.h"

namespace Sprocket {

StaticCamera::StaticCamera(
    const Maths::vec3& position,
    float pitch,
    float yaw
)
    : d_position(position)
    , d_pitch(pitch)
    , d_yaw(yaw)
{   
}

Maths::mat4 StaticCamera::View() const
{
    Maths::quat orientation = Maths::identity;
    orientation = Maths::Rotate(orientation, {1, 0, 0}, d_pitch);
    orientation = Maths::Rotate(orientation, {0, 1, 0}, d_yaw);
    return Maths::Inverse(Maths::Transform(d_position, orientation));
}

}