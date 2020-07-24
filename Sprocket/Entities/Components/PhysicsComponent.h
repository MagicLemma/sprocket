#pragma once
#include "Maths.h"

#include <variant>

namespace Sprocket {

struct BoxCollider
{
    Maths::vec3 halfExtents;
};

struct SphereCollider
{
    float radius;
};

struct CapsuleCollider
{
    float radius;
    float height;
};

struct EmptyCollider
{
};

using Collider = std::variant<
    BoxCollider,
    SphereCollider,
    CapsuleCollider,
    EmptyCollider
>;

struct PhysicsComponent
{
    Maths::vec3 velocity = {0.0, 0.0, 0.0};
    bool        gravity  = true;
    bool        frozen   = false;

    Collider collider            = EmptyCollider();
    float    mass                = 1.0f;
    float    bounciness          = 0.5f;
    float    frictionCoefficient = 0.3f;
    float    rollingResistance   = 0.0f;
};

}