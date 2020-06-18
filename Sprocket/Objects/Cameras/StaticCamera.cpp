#include "StaticCamera.h"

namespace Sprocket {

StaticCamera::StaticCamera(const Maths::vec3& position)
    : d_position(position)
    , d_orientation(Maths::identity)
{   
}

Maths::mat4 StaticCamera::View() const
{

    return Maths::Inverse(Maths::Transform(d_position, d_orientation));
}

}