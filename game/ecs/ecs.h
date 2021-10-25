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
#include <deque>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>

struct physics_runtime;
struct rigid_body_runtime;
struct collider_runtime;

namespace game {

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

struct InputSingleton
{
    std::shared_ptr<spkt::input_store> input_store = nullptr;
};

struct GameGridSingleton
{
    game::entity hovered_square_entity = game::null;
    game::entity clicked_square_entity = game::null;
    glm::ivec2 hovered_square = {0, 0};
    std::optional<glm::ivec2> clicked_square = std::nullopt;
};

struct TileMapSingleton
{
    std::unordered_map<glm::ivec2, game::entity> tiles = {};
};

struct CameraSingleton
{
    game::entity camera_entity = game::null;
};

using registry = apx::registry<
    Runtime,
    Event,
    NameComponent,
    Transform3DComponent,
    StaticModelComponent,
    AnimatedModelComponent,
    ScriptComponent,
    Camera3DComponent,
    PathComponent,
    LightComponent,
    SunComponent,
    AmbienceComponent,
    InputSingleton,
    GameGridSingleton,
    TileMapSingleton,
    CameraSingleton
>;

}
