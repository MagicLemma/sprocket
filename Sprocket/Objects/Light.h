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

struct AmbientLight
{
    Maths::vec3 colour     = {1.0, 1.0, 1.0};
    float       brightness = 0.0f;
};

struct Lights
// TODO: Allow for more than one DirectionalLight
{
    std::vector<PointLight> points;
    DirectionalLight        sun;
    AmbientLight            ambience;
};

}