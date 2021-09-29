#include "basic_systems.h"

#include <Sprocket/Scene/lua_ecs.h>
#include <Sprocket/Scene/scene.h>
#include <Sprocket/Scripting/lua_input.h>
#include <Sprocket/Scripting/lua_maths.h>
#include <Sprocket/Scripting/lua_script.h>
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
            auto input_singleton = registry.find<InputSingleton>();
            input_store& input = *registry.get<InputSingleton>(input_singleton).input_store;
            
            sc.script_runtime = std::make_shared<lua::script>(sc.script);
            lua::script& script = *sc.script_runtime;

            lua::load_maths(script);
            lua::load_registry(script, registry);
            lua::load_input_store(script, input);

            if (script.has_function(INIT_FUNCTION)) {
                script.set_value("__command_list__", &commands);
                script.call_function<void>(INIT_FUNCTION, entity);
            }
        }
        else {
            lua::script& script = *sc.script_runtime;
            script.set_value("__command_list__", &commands);
            script.call_function<void>(UPDATE_FUNCTION, entity, dt);
        }
    }

    for (auto& command : commands) {
        command();
    }
}

}