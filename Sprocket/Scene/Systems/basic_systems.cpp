#include "basic_systems.h"

#include <Sprocket/Scene/scene.h>
#include <Sprocket/Scripting/lua_events.h>
#include <Sprocket/Scripting/lua_registry.h>
#include <Sprocket/Scripting/lua_maths.h>
#include <Sprocket/Scripting/LuaScript.h>
#include <Sprocket/Utility/input_store.h>

#include <glm/gtx/norm.hpp>

#include <functional>
#include <memory>
#include <utility>
#include <vector>

namespace spkt {
namespace {

static constexpr const char* INIT_FUNCTION = "init";
static constexpr const char* UPDATE_FUNCTION = "on_update";

}

void script_system(spkt::registry& registry, double dt)
{
    std::vector<std::function<void()>> commands;

    for (auto entity : registry.view<ScriptComponent>()) {
        auto& sc = registry.get<ScriptComponent>(entity);
        if (!sc.active) { continue; }

        if (!sc.script_runtime) {
            input_store& input = *get_singleton<InputSingleton>(registry).input_store;
            
            sc.script_runtime = std::make_shared<lua::Script>(sc.script);
            lua::Script& script = *sc.script_runtime;

            lua::load_maths(script);
            lua::load_registry(script, registry);
            lua::load_input_store(script, input);

            if (script.has_function(INIT_FUNCTION)) {
                script.set_value("__command_list__", &commands);
                script.call_function<void>(INIT_FUNCTION, entity);
            }
        }
        else {
            lua::Script& script = *sc.script_runtime;
            script.set_value("__command_list__", &commands);
            script.call_function<void>(UPDATE_FUNCTION, entity, dt);
        }
    }

    for (auto& command : commands) {
        command();
    }
}

void animation_system(spkt::registry& registry, double dt)
{
    for (auto [ac] : registry.view_get<AnimatedModelComponent>()) {
        ac.animation_time += (float)dt * ac.animation_speed;
    }
}

void camera_system(spkt::registry& registry, double dt)
{
    const auto& input = *get_singleton<InputSingleton>(registry).input_store;
    float aspect_ratio = input.window_width() / input.window_height();

    for (auto [cam] : registry.view_get<Camera3DComponent>()) {
        cam.projection = glm::perspective(cam.fov, aspect_ratio, 0.1f, 1000.0f);
    }
}

void path_follower_system(spkt::registry& registry, double dt)
{
    for (auto [path, transform] : registry.view_get<PathComponent, Transform3DComponent>()) {
        if (path.markers.empty()) { continue; }
        
        glm::vec3 to_dest = path.markers.front() - transform.position;
        glm::vec3 direction = glm::normalize(to_dest);
        glm::vec3 advance = path.speed * (float)dt * direction;

        if (glm::length2(advance) < glm::length2(to_dest)) {
            transform.position += advance;
        } else {
            transform.position = path.markers.front();
            path.markers.pop_front();
        }
    }
}

void delete_below_50_system(spkt::registry& registry, double)
{
    registry.destroy_if<Transform3DComponent>([&](spkt::entity entity) {
        const auto& t = registry.get<Transform3DComponent>(entity);
        return t.position.y < -50.0f;
    });
}

void clear_events_system(spkt::registry& registry, double dt)
{
    registry.destroy_if<Event>([](spkt::entity) { return true; });
}

}