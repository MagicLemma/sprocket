#pragma once
#include "Maths.h"
#include "Model3D.h"
#include "Material.h"
#include "Colliders.h"
#include "Lens.h"

#include <cstddef>
#include <vector>
#include <memory>

namespace Sprocket {

inline std::size_t Counter()
{
    static std::size_t id = 0;
    return ++id;
}

template <typename T>
inline std::size_t GetComponentTypeId() noexcept
{
    static std::size_t id = Counter();
    return id;
}

struct Component {
    virtual ~Component() {}
};

struct ModelComponent : public Component
{
    Model3D  model;
    Material material;
    float    scale = 1.0f;  // Affects the size of the model.
};

struct PhysicsComponent : public Component
{
    Maths::vec3 velocity = {0.0, 0.0, 0.0};
    bool        gravity  = true;
    bool        frozen   = false;
};

struct ColliderComponent : public Component
{
    Collider collider            = EmptyCollider();
    float    mass                = 1.0f;
    float    bounciness          = 0.5f;
    float    frictionCoefficient = 0.3f;
    float    rollingResistance   = 0.0f;
};

struct PlayerComponent : public Component
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

struct SelectComponent : public Component
{
    bool selected = false;
    bool hovered = false;
};

struct ScriptComponent : public Component
{
    std::string script;
    bool active = true;
};

struct CameraComponent : public Component
{
    std::shared_ptr<Lens> lens;
    float pitch = 0.0f; // Relative to the entities orientation.
        // TODO: Generalise this.
};

}