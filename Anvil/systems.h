#pragma once
#include <Anvil/ecs/ecs.h>
#include <Anvil/ecs/lua_ecs.h>

#include <Sprocket/Core/events.h>
#include <Sprocket/Core/log.h>
#include <Sprocket/Scripting/lua_input.h>
#include <Sprocket/Scripting/lua_maths.h>
#include <Sprocket/Scripting/lua_script.h>
#include <Sprocket/Utility/input_store.h>

namespace anvil {

inline void delete_below_50_system(spkt::registry& registry, double)
{
    registry.destroy_if<spkt::Transform3DComponent>([&](spkt::entity entity) {
        const auto& t = registry.get<spkt::Transform3DComponent>(entity);
        return t.position.y < -50.0f;
    });
}

inline void animation_system(spkt::registry& registry, double dt)
{
    for (auto [ac] : registry.view_get<spkt::AnimatedModelComponent>()) {
        ac.animation_time += (float)dt * ac.animation_speed;
    }
}

inline void clear_events_system(spkt::registry& registry, double dt)
{
    registry.destroy_if<spkt::Event>([](spkt::entity) { return true; });
}

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

inline void input_system_init(spkt::registry& registry, spkt::window* window)
{
    assert(window);
    auto singleton = registry.create();
    registry.emplace<spkt::Runtime>(singleton);
    auto& is = registry.emplace<spkt::InputSingleton>(singleton);
    is.input_store = std::make_shared<spkt::input_store>(window);
}

inline void input_system_on_event(spkt::registry& registry, spkt::event& event)
{
    auto singleton = registry.find<spkt::InputSingleton>();
    registry.get<spkt::InputSingleton>(singleton).input_store->on_event(event);
}

inline void input_system_end(spkt::registry& registry, double dt)
{
    auto singleton = registry.find<spkt::InputSingleton>();
    registry.get<spkt::InputSingleton>(singleton).input_store->end_frame();
}

}