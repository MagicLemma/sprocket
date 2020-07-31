#pragma once
#include "Maths.h"

namespace Sprocket {

struct TransformComponent
{
    Maths::vec3 position = {0, 0, 0};
    Maths::quat orientation = Maths::identity;
};

}