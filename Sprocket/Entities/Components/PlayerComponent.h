#pragma once
#include "Maths.h"

namespace Sprocket {

// TODO: Replace with a script
struct PlayerComponent
{
    bool movingForwards  = false;
    bool movingBackwards = false;
    bool movingLeft      = false;
    bool movingRight     = false;
    bool jumping         = false;

    Maths::vec3 direction = {0.0, 0.0, 0.0};
    float yaw             = 0.0f;  // Forwards direction
    float pitch           = 0.0f;
};

}