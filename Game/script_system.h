#pragma once
#include <Game/ecs/lua_ecs.h>
#include <Game/ecs/scene.h>

#include <Sprocket/Scripting/lua_input.h>
#include <Sprocket/Scripting/lua_maths.h>
#include <Sprocket/Scripting/lua_script.h>
#include <Sprocket/Utility/input_store.h>

#include <functional>
#include <memory>
#include <utility>
#include <vector>

inline void script_system(spkt::registry& registry, double dt)
{
    static constexpr const char* INIT_FUNCTION = "init";
    static constexpr const char* UPDATE_FUNCTION = "on_update";
    
    std::vector<std::function<void()>> commands;

    for (auto entity : registry.view<spkt::ScriptComponent>()) {
        auto& sc = registry.get<spkt::ScriptComponent>(entity);
        if (!sc.active) { continue; }

        if (!sc.script_runtime) {
            auto input_singleton = registry.find<spkt::InputSingleton>();
            auto& input = *registry.get<spkt::InputSingleton>(input_singleton).input_store;
            
            sc.script_runtime = std::make_shared<spkt::lua::script>(sc.script);
            spkt::lua::script& script = *sc.script_runtime;

            spkt::lua::load_maths(script);
            spkt::lua::load_input_store(script, input);
            spkt::load_registry(script, registry);

            if (script.has_function(INIT_FUNCTION)) {
                script.set_value("__command_list__", &commands);
                script.call_function<void>(INIT_FUNCTION, entity);
            }
        }
        else {
            spkt::lua::script& script = *sc.script_runtime;
            script.set_value("__command_list__", &commands);
            script.call_function<void>(UPDATE_FUNCTION, entity, dt);
        }
    }

    for (auto& command : commands) {
        command();
    }
}