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

struct TransformComponent : public Component
{
    Maths::mat4 transform;
        // Contains the translation and rotation data for
        // this entity.
};

struct ModelComponent : public Component
{
    Model3D               model;
    float                 scale;  // Affects the size of the model.
    std::vector<Material> materials;
};

struct PhysicsComponent : public Component
{
    bool     stationary = false;
    float    mass       = 1.0f;
    Collider collider   = EmptyCollider();

    float       velocity;
    Maths::vec3 velocityDirection;
};

struct PlayerComponent : public Component
{
    bool movingForwards = false;
    bool movingBackwards = false;
    bool movingLeft = false;
    bool movingRight = false;

    float yaw = 0.0f;  // Forwards direction

    bool jumping = false;
};


}