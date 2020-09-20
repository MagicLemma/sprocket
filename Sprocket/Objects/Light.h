#pragma once
#include "Utility/Maths.h"

#include <vector>

namespace Sprocket {

struct AmbientLight
{
    Maths::vec3 colour     = {1.0, 1.0, 1.0};
    float       brightness = 0.0f;
};

struct Lights
// TODO: Allow for more than one DirectionalLight
{
    AmbientLight            ambience;
};

}