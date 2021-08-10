#pragma once
#include <apecs.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

#include <array>
#include <map>
#include <optional>
#include <queue>
#include <string>
#include <unordered_map>
#include <utility>
#include <memory>

namespace spkt {
namespace lua { class Script; }

using entity = apx::entity;
const auto null = apx::null;

struct physics_runtime;
struct rigid_body_runtime;
struct collider_runtime;

struct particle
{
    glm::vec3 position      = {0.0, 0.0, 0.0};
    glm::vec3 velocity      = {0.0, 0.0, 0.0};
    glm::vec3 acceleration  = {0.0, 0.0, 0.0};
    glm::vec3 scale         = {1.0, 1.0, 1.0};
    double life = -1.0;
};

static constexpr std::size_t NUM_PARTICLES = 10000;

// Components
struct Runtime
{
};

struct Singleton
{
};

struct Event
{
};

struct NameComponent
{
    std::string name = "Entity";
};

struct Transform2DComponent
{
    glm::vec2 position = {0.0f, 0.0f};
    float rotation = 0.0f;
    glm::vec2 scale = {1.0f, 1.0f};
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
    std::shared_ptr<lua::Script> script_runtime = nullptr;
};

struct Camera3DComponent
{
    glm::mat4 projection = {};
    float fov = 70.0f;
    float pitch = 0.0f;
};

struct PathComponent
{
    std::deque<glm::vec3> markers = {};
    float speed = 0.0f;
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
    spkt::entity entity_a = {};
    spkt::entity entity_b = {};
};

struct PhysicsSingleton
{
    std::shared_ptr<physics_runtime> physics_runtime = nullptr;
};

struct InputSingleton
{
    std::array<bool, 512> keyboard = {};
    std::array<bool, 5> mouse = {};
    std::array<bool, 5> mouse_click = {};
    std::array<bool, 5> mouse_unclick = {};
    glm::vec2 mouse_pos = {0.0, 0.0};
    glm::vec2 mouse_pos_last_frame = {0.0, 0.0};
    glm::vec2 mouse_offset = {0.0, 0.0};
    glm::vec2 mouse_scrolled = {0.0, 0.0};
    float window_width = 1280.0f;
    float window_height = 720.0f;
    bool window_resized = false;
};

struct GameGridSingleton
{
    spkt::entity hovered_square_entity = spkt::null;
    spkt::entity clicked_square_entity = spkt::null;
    glm::ivec2 hovered_square = {0, 0};
    std::optional<glm::ivec2> clicked_square = std::nullopt;
};

struct TileMapSingleton
{
    std::unordered_map<glm::ivec2, spkt::entity> tiles = {};
};

struct CameraSingleton
{
    spkt::entity camera_entity = spkt::null;
};

struct ParticleSingleton
{
    std::shared_ptr<std::array<particle, NUM_PARTICLES>> particles = nullptr;
    std::size_t next_slot = NUM_PARTICLES - 1;
};

using registry = apx::registry<
    Runtime,
    Singleton,
    Event,
    NameComponent,
    Transform2DComponent,
    Transform3DComponent,
    StaticModelComponent,
    AnimatedModelComponent,
    RigidBody3DComponent,
    BoxCollider3DComponent,
    SphereCollider3DComponent,
    CapsuleCollider3DComponent,
    ScriptComponent,
    Camera3DComponent,
    PathComponent,
    LightComponent,
    SunComponent,
    AmbienceComponent,
    ParticleComponent,
    CollisionEvent,
    PhysicsSingleton,
    InputSingleton,
    GameGridSingleton,
    TileMapSingleton,
    CameraSingleton,
    ParticleSingleton
>;

}
