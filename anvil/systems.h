#pragma once
#include <anvil/ecs/ecs.h>
#include <anvil/ecs/lua_ecs.h>

#include <Sprocket/Core/events.h>
#include <Sprocket/Core/log.h>
#include <Sprocket/Scripting/lua_input.h>
#include <Sprocket/Scripting/lua_maths.h>
#include <Sprocket/Scripting/lua_script.h>
#include <Sprocket/Utility/input_store.h>

namespace anvil {

inline void delete_below_50_system(anvil::registry& registry, double)
{
    registry.destroy_if<anvil::Transform3DComponent>([&](anvil::entity entity) {
        const auto& t = registry.get<anvil::Transform3DComponent>(entity);
        return t.position.y < -50.0f;
    });
}

inline void animation_system(anvil::registry& registry, double dt)
{
    for (auto [ac] : registry.view_get<anvil::AnimatedModelComponent>()) {
        ac.animation_time += (float)dt * ac.animation_speed;
    }
}

inline void clear_events_system(anvil::registry& registry, double dt)
{
    registry.destroy_if<anvil::Event>([](anvil::entity) { return true; });
}

inline void script_system(anvil::registry& registry, double dt)
{
    static constexpr const char* INIT_FUNCTION = "init";
    static constexpr const char* UPDATE_FUNCTION = "on_update";
    
    std::vector<std::function<void()>> commands;

    for (auto entity : registry.view<anvil::ScriptComponent>()) {
        auto& sc = registry.get<anvil::ScriptComponent>(entity);
        if (!sc.active) { continue; }

        if (!sc.script_runtime) {
            auto input_singleton = registry.find<anvil::InputSingleton>();
            auto& input = *registry.get<anvil::InputSingleton>(input_singleton).input_store;
            
            sc.script_runtime = std::make_shared<spkt::lua::script>(sc.script);
            spkt::lua::script& script = *sc.script_runtime;

            spkt::lua::load_maths(script);
            spkt::lua::load_input_store(script, input);
            anvil::load_registry(script, registry);

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

inline void input_system_init(anvil::registry& registry, spkt::window* window)
{
    assert(window);
    auto singleton = registry.create();
    registry.emplace<anvil::Runtime>(singleton);
    auto& is = registry.emplace<anvil::InputSingleton>(singleton);
    is.input_store = std::make_shared<spkt::input_store>(window);
}

inline void input_system_on_event(anvil::registry& registry, spkt::event& event)
{
    auto singleton = registry.find<anvil::InputSingleton>();
    registry.get<anvil::InputSingleton>(singleton).input_store->on_event(event);
}

inline void input_system_end(anvil::registry& registry, double dt)
{
    auto singleton = registry.find<anvil::InputSingleton>();
    registry.get<anvil::InputSingleton>(singleton).input_store->end_frame();
}

}