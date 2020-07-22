#pragma once
#include "Maths.h"
#include "Model3D.h"
#include "Material.h"
#include "Colliders.h"

#include <cstddef>
#include <vector>
#include <memory>

namespace Sprocket {

struct TransformComponent
{
    Maths::vec3 position = {0, 0, 0};
    Maths::quat orientation = Maths::identity;

    Maths::mat4 Transform() const { return Maths::Transform(position, orientation); }
};

struct NameComponent
{
    std::string name;
};

struct DeletableComponent
{
    bool alive = true;
};

struct ModelComponent
{
    Model3D  model;
    Material material;
    float    scale = 1.0f;  // Affects the size of the model.
};

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

struct SelectComponent
{
    bool selected = false;
    bool hovered = false;
};

struct ScriptComponent
{
    std::string script;
    bool active = true;
};

struct CameraComponent
{
    Maths::mat4 projection;
    float       pitch = 0.0f; // Relative to the entities orientation.
        // TODO: Generalise this.
};

}