#pragma once
#include "Maths.h"
#include <queue>
#include <string>
#include <utility>
#include <array>
#include <unordered_map>
#include <map>
#include <optional>
#include <memory>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

#include "apecs.hpp"

namespace spkt {
namespace lua { class Script; }
struct physics_runtime;
struct rigid_body_runtime;
struct collider_runtime;

// Components
struct TemporaryComponent
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
    glm::quat orientation = {0.0f, 0.0f, 0.0f, 1.0f};
    glm::vec3 scale = {1.0f, 1.0f, 1.0f};
};

struct ModelComponent
{
    std::string mesh = "";
    std::string material = "";
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
    glm::quat orientation = {0.0f, 0.0f, 0.0f, 1.0f};
    float mass = 1.0f;
    glm::vec3 halfExtents = {0.0f, 0.0f, 0.0f};
    bool applyScale = true;
    std::shared_ptr<collider_runtime> runtime = nullptr;
};

struct SphereCollider3DComponent
{
    glm::vec3 position = {0.0f, 0.0f, 0.0f};
    glm::quat orientation = {0.0f, 0.0f, 0.0f, 1.0f};
    float mass = 1.0f;
    float radius = 1.0f;
    std::shared_ptr<collider_runtime> runtime = nullptr;
};

struct CapsuleCollider3DComponent
{
    glm::vec3 position = {0.0f, 0.0f, 0.0f};
    glm::quat orientation = {0.0f, 0.0f, 0.0f, 1.0f};
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
    std::queue<glm::vec3> markers = {};
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

struct MeshAnimationComponent
{
    std::string name = "";
    float time = 0.0f;
    float speed = 1.0f;
};

struct PhysicsSingleton
{
    std::shared_ptr<physics_runtime> physics_runtime = nullptr;
    std::vector<std::pair<apx::entity, apx::entity>> collisions = {};
};

struct InputSingleton
{
    std::array<bool, 512> keyboard = {};
    std::array<bool, 5> mouse = {};
    std::array<bool, 5> mouse_click = {};
    std::array<bool, 5> mouse_unclick = {};
    glm::vec2 mouse_pos = {0.0, 0.0};
    glm::vec2 mouse_offset = {0.0, 0.0};
    glm::vec2 mouse_scrolled = {0.0, 0.0};
    float window_width = 1280.0f;
    float window_height = 720.0f;
    bool window_resized = false;
};

struct GameGridSingleton
{
    apx::entity hovered_square_entity = apx::null;
    apx::entity clicked_square_entity = apx::null;
    glm::ivec2 hovered_square = {0, 0};
    std::optional<glm::ivec2> clicked_square = std::nullopt;
};

struct TileMapSingleton
{
    std::unordered_map<glm::ivec2, apx::entity> tiles = {};
};

struct CameraSingleton
{
    apx::entity camera_entity = apx::null;
};


}
