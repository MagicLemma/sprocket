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

namespace Sprocket {

void ScriptRunner::on_update(spkt::registry& registry, double dt)
{
    std::vector<std::function<void()>> commands;

    registry.erase_if<ScriptComponent>([&](apx::entity entity) {
        auto& sc = registry.get<ScriptComponent>(entity);
        if (sc.requested_deletion) {
            return true;
        }
        else if (!sc.script_runtime) {
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
        
        return false;
    });

    for (auto& command : commands) {
        command();
    }
}

void ScriptRunner::on_event(spkt::registry& registry, ev::Event& event)
{
}

}