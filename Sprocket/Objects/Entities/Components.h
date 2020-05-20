#pragma once
#include "Maths.h"
#include "Model3D.h"
#include "Material.h"
#include "Colliders.h"

#include <cstddef>
#include <vector>

namespace Sprocket {

inline std::size_t counter()
{
    static std::size_t id = 0;
    return ++id;
}

template <typename T>
inline std::size_t getComponentTypeId() noexcept
{
    static std::size_t id = counter();
    return id;
}

struct Component {
    virtual ~Component() {}
};

struct ModelComponent : public Component
{
    Model3D  model;
    Material material;
    float    scale;  // Affects the size of the model.
};

struct PhysicsComponent : public Component
{
    bool        stationary          = false;
    Collider    collider            = EmptyCollider();
    float       mass                = 1.0f;
    
    bool        gravity             = true;
    Maths::vec3 velocity            = {0.0, 0.0, 0.0};
    float       bounciness          = 0.5f;
    float       frictionCoefficient = 0.3f;
    float       rollingResistance   = 0.0f;
};

struct PlayerComponent : public Component
{
    bool movingForwards = false;
    bool movingBackwards = false;
    bool movingLeft = false;
    bool movingRight = false;
    Maths::vec3 direction = {0.0, 0.0, 0.0};

    float yaw = 0.0f;  // Forwards direction
    float pitch = 0.0f;

    bool jumping = false;
    bool wallLeft = false;
};

struct SelectComponent : public Component
{
    bool selected = false;
    bool hovered = false;
};


}