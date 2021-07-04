#include "basic_systems.h"
#include "Components.h"
#include "LuaScript.h"
#include "LuaLibrary.h"
#include "LuaMaths.h"

#include <glm/gtx/norm.hpp>
#include <utility>
#include <vector>
#include <functional>

namespace spkt {
namespace {

static constexpr const char* INIT_FUNCTION = "init";
static constexpr const char* UPDATE_FUNCTION = "on_update";

}

void script_system(spkt::registry& registry, double dt)
{
    std::vector<std::function<void()>> commands;

    for (apx::entity entity : registry.view<ScriptComponent>()) {
        auto& sc = registry.get<ScriptComponent>(entity);
        if (!sc.active) { continue; }

        if (!sc.script_runtime) {
            sc.script_runtime = std::make_shared<lua::Script>(sc.script);
            lua::Script& script = *sc.script_runtime;
            lua::load_vec3_functions(script);
            lua::load_vec2_functions(script);
            lua::load_registry_functions(script, registry);
            lua::load_entity_transformation_functions(script);
            lua::load_entity_component_functions(script);
            if (script.has_function(INIT_FUNCTION)) {
                script.set_value("__command_list__", &commands);
                script.call_function<void>(INIT_FUNCTION, apx::handle{registry, entity});
            }
        }
        else {
            lua::Script& script = *sc.script_runtime;
            script.set_value("__command_list__", &commands);
            script.call_function<void>(UPDATE_FUNCTION, apx::handle{registry, entity}, dt);
        }
    }

    for (auto& command : commands) {
        command();
    }
}

void animation_system(spkt::registry& registry, double dt)
{
    for (auto id : registry.view<MeshAnimationComponent>()) {
        auto& ac = registry.get<MeshAnimationComponent>(id);
        ac.time += (float)dt * ac.speed;
    }
}

void camera_system(spkt::registry& registry, double dt)
{
    auto singleton = registry.find<Singleton>();
    if (!registry.valid(singleton)) {
        return;
    }

    const auto& input = registry.get<InputSingleton>(singleton);
    float aspect_ratio = input.window_width / input.window_height;

    for (auto entity : registry.view<Camera3DComponent>()) {
        auto& camera = registry.get<Camera3DComponent>(entity);
        camera.projection = glm::perspective(
            camera.fov, aspect_ratio, 0.1f, 1000.0f
        );
    }
}

void path_follower_system(spkt::registry& registry, double dt)
{
    for (auto entity : registry.view<PathComponent>()) {
        auto& transform = registry.get<Transform3DComponent>(entity);
        auto& path = registry.get<PathComponent>(entity);
        if (path.markers.empty()) { return; }
        
        glm::vec3 direction = glm::normalize(path.markers.front() - transform.position);
        glm::vec3 advance = path.speed * (float)dt * direction;

        if (glm::length2(advance) < glm::length2(path.markers.front() - transform.position)) {
            transform.position += advance;
        }
        else {
            transform.position = path.markers.front();
            path.markers.pop();
        }
    }
}

void delete_below_50_system(spkt::registry& registry, double)
{
    registry.erase_if<Transform3DComponent>([&](apx::entity entity) {
        const auto& t = registry.get<Transform3DComponent>(entity);
        return t.position.y < -50.0f;
    });
}

void clear_events_system(spkt::registry& registry, double dt)
{
    registry.erase_if<Event>([](apx::entity) { return true; });
}

}