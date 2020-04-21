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

}