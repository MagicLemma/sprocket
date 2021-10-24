#pragma once
#include <apecs.hpp>
#include <sprocket/graphics/particles.h>
#include <sprocket/Scripting/lua_script.h>
#include <sprocket/Utility/Hashing.h>
#include <sprocket/Utility/input_store.h>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <array>
#include <cstddef>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>

struct physics_runtime;
struct rigid_body_runtime;
struct collider_runtime;

namespace anvil {

using entity = apx::entity;
const auto null = apx::null;

// Components
struct Runtime
{
};

struct Event
{
};

struct NameComponent
{
    std::string name = "Entity";
};

struct Transform3DComponent
{
    glm::vec3 position = {0.0f, 0.0f, 0.0f};
    glm::quat orientation = glm::identity<glm::quat>();
    glm::vec3 scale = {1.0f, 1.0f, 1.0f};
};

struct StaticModelComponent
{
    std::string mesh = "";
    std::string material = "";
};

struct AnimatedModelComponent
{
    std::string mesh = "";
    std::string material = "";
    std::string animation_name = "";
    float animation_time = 0.0f;
    float animation_speed = 1.0f;
};

struct RigidBody3DComponent
{
    glm::vec3 velocity = {0.0f, 0.0f, 0.0f};
    bool gravity = true;
    bool frozen = false;
    float bounciness = 0.5f;
    float frictionCoefficient = 0.3f;
    float rollingResistance = 0.0f;
    glm::vec3 force = {0.0f, 0.0f, 0.0f};
    bool onFloor = false;
    std::shared_ptr<rigid_body_runtime> runtime = nullptr;
};

struct BoxCollider3DComponent
{
    glm::vec3 position = {0.0f, 0.0f, 0.0f};
    glm::quat orientation = glm::identity<glm::quat>();
    float mass = 1.0f;
    glm::vec3 halfExtents = {0.0f, 0.0f, 0.0f};
    bool applyScale = true;
    std::shared_ptr<collider_runtime> runtime = nullptr;
};

struct SphereCollider3DComponent
{
    glm::vec3 position = {0.0f, 0.0f, 0.0f};
    glm::quat orientation = glm::identity<glm::quat>();
    float mass = 1.0f;
    float radius = 1.0f;
    std::shared_ptr<collider_runtime> runtime = nullptr;
};

struct CapsuleCollider3DComponent
{
    glm::vec3 position = {0.0f, 0.0f, 0.0f};
    glm::quat orientation = glm::identity<glm::quat>();
    float mass = 1.0f;
    float radius = 1.0f;
    float height = 1.0f;
    std::shared_ptr<collider_runtime> runtime = nullptr;
};

struct ScriptComponent
{
    std::string script = "";
    bool active = true;
    std::shared_ptr<spkt::lua::script> script_runtime = nullptr;
};

struct Camera3DComponent
{
    float fov = 70.0f;
    float pitch = 0.0f;
};

struct LightComponent
{
    glm::vec3 colour = {1.0f, 1.0f, 1.0f};
    float brightness = 1.0f;
};

struct SunComponent
{
    glm::vec3 colour = {1.0f, 1.0f, 1.0f};
    float brightness = 1.0f;
    glm::vec3 direction = {0.0f, -1.0f, 0.0f};
    bool shadows = false;
};

struct AmbienceComponent
{
    glm::vec3 colour = {1.0f, 1.0f, 1.0f};
    float brightness = 1.0f;
};

struct ParticleComponent
{
    float interval = 1.0f;
    glm::vec3 velocity = {0.0f, 0.0f, 0.0f};
    float velocityNoise = 0.0f;
    glm::vec3 acceleration = {0.0f, -9.81f, 0.0f};
    glm::vec3 scale = {1.0f, 1.0f, 1.0f};
    float life = 1.0f;
    float accumulator = 0.0f;
};

struct CollisionEvent
{
    entity entity_a = {};
    entity entity_b = {};
};

struct PhysicsSingleton
{
    std::shared_ptr<physics_runtime> physics_runtime = nullptr;
};

struct InputSingleton
{
    std::shared_ptr<spkt::input_store> input_store = nullptr;
};

struct ParticleSingleton
{
    std::shared_ptr<std::array<spkt::particle, spkt::NUM_PARTICLES>> particles = nullptr;
    std::size_t next_slot = spkt::NUM_PARTICLES - 1;
};

using registry = apx::registry<
    Runtime,
    Event,
    NameComponent,
    Transform3DComponent,
    StaticModelComponent,
    AnimatedModelComponent,
    RigidBody3DComponent,
    BoxCollider3DComponent,
    SphereCollider3DComponent,
    CapsuleCollider3DComponent,
    ScriptComponent,
    Camera3DComponent,
    LightComponent,
    SunComponent,
    AmbienceComponent,
    ParticleComponent,
    CollisionEvent,
    PhysicsSingleton,
    InputSingleton,
    ParticleSingleton
>;

}
