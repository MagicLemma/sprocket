#include "ScriptRunner.h"
#include "Log.h"
#include "ECS.h"
#include "Scene.h"
#include "LuaScript.h"
#include "LuaLibrary.h"
#include "LuaMaths.h"
#include "Components.h"

#include <utility>
#include <vector>
#include <functional>

namespace spkt {

void ScriptRunner::on_update(spkt::registry& registry, double dt)
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
            script.set_value("__command_list__", &commands);
            script.call_function<void>("Init", spkt::entity{registry, entity});
        }
        else {
            lua::Script& script = *sc.script_runtime;
            script.set_value("__command_list__", &commands);
            script.call_function<void>("OnUpdate", spkt::entity{registry, entity}, dt);
        }
    }

    for (auto& command : commands) {
        command();
    }
}

}