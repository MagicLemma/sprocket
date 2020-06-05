#pragma once
#include "Utility/Maths.h"

#include <vector>

namespace Sprocket {

struct PointLight
{
    Maths::vec3 position;
    Maths::vec3 colour;
    Maths::vec3 attenuation;
};

struct DirectionalLight
{
    Maths::vec3 direction;
    Maths::vec3 colour;
    float       brightness;
};

using PointLights = std::vector<PointLight>;

}