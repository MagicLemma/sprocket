#pragma once
#include "Utility/Maths.h"

#include <vector>

namespace Sprocket {

struct Light
{
    Maths::vec3 position;
    Maths::vec3 colour;
    Maths::vec3 attenuation;
};

using Lights = std::vector<Light>;

}