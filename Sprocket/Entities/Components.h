#pragma once
#include "Maths.h"
#include <string>

namespace Sprocket {

struct TransformComponent
{
    Maths::vec3 position = {0, 0, 0};
    Maths::quat orientation = Maths::identity;
};

struct ModelComponent
{
    std::string model        = "";
    float       scale        = 1.0f;

    std::string texture      = "";    
    float       shineDamper  = 1.0f;
    float       reflectivity = 0.0f;
};

enum class Collider
{
    NONE,
    SPHERE,
    CAPSULE,
    BOX
};

struct PhysicsComponent
{
    Maths::vec3 velocity      = {0.0, 0.0, 0.0};
    bool gravity              = true;
    bool frozen               = false;
    
    Collider collider         = Collider::NONE;
    Maths::vec3 halfExtents   = {0.0, 0.0, 0.0};
    float radius              = 1.0f;
    float height              = 1.0f;

    float mass                = 1.0f;
    float bounciness          = 0.5f;
    float frictionCoefficient = 0.3f;
    float rollingResistance   = 0.0f;
};

struct NameComponent
{
    std::string name;
};

struct ScriptComponent
{
    std::string script;
    bool        active = true;
};

struct PlayerComponent
{
    bool movingForwards  = false;
    bool movingBackwards = false;
    bool movingLeft      = false;
    bool movingRight     = false;
    bool jumping         = false;

    Maths::vec3 direction = {0.0, 0.0, 0.0};
    float yaw             = 0.0f;
    float pitch           = 0.0f;
};

struct CameraComponent
{
    Maths::mat4 projection;
    float       pitch = 0.0f;
};

struct SelectComponent
{
    bool selected = false;
    bool hovered = false;
};

}