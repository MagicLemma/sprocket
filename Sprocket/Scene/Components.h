// GENERATED - DO NOT EDIT
#pragma once
#include "Maths.h"
#include <vector>
#include <string>
#include <queue>

namespace Sprocket {

// A flag component to say to the serialiser that an entity with this should not get saved.
struct TemporaryComponent
{
};

struct NameComponent
{
    std::string name;
};

struct TransformComponent
{
    Maths::vec3 position = {0, 0, 0};
    Maths::quat orientation = {0, 0, 0, 1};
};

enum class Collider { NONE, SPHERE, CAPSULE, BOX };
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

    // Runtime info
    Maths::vec3 force         = {0.0, 0.0, 0.0};
    bool onFloor              = false;
};

struct ModelComponent {
    std::string model = "";
    float scale = 1.0f;
    std::string texture = "";
    float shineDamper = 1.0f;
    float reflectivity = 0.0f;
};

struct ScriptComponent {
    std::string script;
    bool active = true;
};

struct CameraComponent
{
    Maths::mat4 projection;
    float       fov = 70.0f;
    float       pitch = 0.0f;
};

struct SelectComponent {
    bool selected = false;
    bool hovered = false;
};

struct PathComponent
{
    std::queue<Maths::vec3> markers;
    float                   speed;
};

struct GridComponent
{
    int x;
    int z;
};

}
