// GENERATED FILE @ 2020-08-26 13:16:17.919688
#pragma once
#include "Maths.h"
#include <queue>
#include <string>

namespace Sprocket{

// Enums
enum class Collider
{
    NONE,
    SPHERE,
    CAPSULE,
    BOX,
};

// Components
struct TemporaryComponent
{
};

struct NameComponent
{
    std::string name;
};

struct TransformComponent
{
    Maths::vec3 position = {0.0f, 0.0f, 0.0f};
    Maths::quat orientation = {1.0f, 0.0f, 0.0f, 0.0f};
};

struct ModelComponent
{
    std::string model;
    float scale = 1.0f;
    std::string texture;
    float shineDamper = 1.0f;
    float reflectivity = 0.0f;
};

struct RigidBody3DComponent
{
    Maths::vec3 velocity = {0.0f, 0.0f, 0.0f};
    bool gravity = true;
    bool frozen = false;
    float bounciness = 0.5f;
    float frictionCoefficient = 0.3f;
    float rollingResistance = 0.0f;
    Maths::vec3 force = {0.0f, 0.0f, 0.0f};
    bool onFloor = false;
};

struct BoxCollider3DComponent
{
    float mass = 1.0f;
    Maths::vec3 halfExtents = {0.0f, 0.0f, 0.0f};
};

struct SphereCollider3DComponent
{
    float mass = 1.0f;
    float radius = 1.0f;
};

struct CapsuleCollider3DComponent
{
    float mass = 1.0f;
    float radius = 1.0f;
    float height = 1.0f;
};

struct ScriptComponent
{
    std::string script;
    bool active = true;
};

struct CameraComponent
{
    Maths::mat4 projection;
    float fov = 70.0f;
    float pitch = 0.0f;
};

struct SelectComponent
{
    bool selected = false;
    bool hovered = false;
};

struct PathComponent
{
    std::queue<Maths::vec3> markers;
    float speed;
};

struct GridComponent
{
    int x;
    int z;
};

struct LightComponent
{
    Maths::vec3 colour = {1.0f, 1.0f, 1.0f};
    Maths::vec3 attenuation = {1.0f, 0.0f, 0.0f};
    float brightness = 1.0f;
};

}
