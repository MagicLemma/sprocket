#pragma once
#include "Maths.h"

namespace Sprocket {

enum class Collider
{
    NONE,
    SPHERE,
    CAPSULE,
    BOX
};

struct PhysicsComponent
{
    Maths::vec3 velocity = {0.0, 0.0, 0.0};
    bool        gravity  = true;
    bool        frozen   = false;

    // Not all of these attributes (halfExtends, radius, height) are used
    // by all component types.
    Collider collider       = Collider::NONE;
    Maths::vec3 halfExtents = {0.0, 0.0, 0.0};
    float radius            = 1.0f;
    float height            = 1.0f;

    float mass                = 1.0f;
    float bounciness          = 0.5f;
    float frictionCoefficient = 0.3f;
    float rollingResistance   = 0.0f;
};

}