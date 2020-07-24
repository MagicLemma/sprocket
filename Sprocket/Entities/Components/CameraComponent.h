#pragma once
#include "Maths.h"

namespace Sprocket {

struct CameraComponent
{
    Maths::mat4 projection;
    float       pitch = 0.0f; // Relative to the entities orientation.
        // TODO: Generalise this.
};

}