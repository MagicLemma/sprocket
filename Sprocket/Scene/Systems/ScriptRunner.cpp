#include "ScriptRunner.h"
#include "Log.h"
#include "ECS.h"
#include "Scene.h"
#include "LuaScript.h"
#include "LuaLibrary.h"
#include "Components.h"
#include "PhysicsEngine3D.h"

#include <utility>

namespace Sprocket {

ScriptRunner::ScriptRunner(Window* window)
    : d_window(window)
{
}

void ScriptRunner::on_update(ecs::Registry&, const ev::Dispatcher&, double dt)
{
    // We delete scripts here rather then with OnRemove otherwise we would segfault if
    // a script tries to delete its own entity, which is functionality that we want to
    // support.
    for (auto it = d_engines.begin(); it != d_engines.end();) {
        auto& entity = it->first;
        auto& [script, alive] = it->second;

        if (alive) {
            if (entity.get<ScriptComponent>().active) {
                script.call_function<void>("OnUpdate", entity, dt);
            }
            ++it;
        } else {
            it = d_engines.erase(it);
        }
    }
}

cppcoro::generator<lua::Script&> ScriptRunner::active_scripts()
{
     for (auto& [entity, pair] : d_engines) {
        auto& [script, alive] = pair;
        if (alive && entity.get<ScriptComponent>().active) {
            co_yield script;
        }
    }
}

// TODO: Add more event handlers
void ScriptRunner::on_startup(ecs::Registry& registry, ev::Dispatcher& dispatcher)
{
    d_input.on_startup(dispatcher);

    dispatcher.subscribe<ecs::Added<ScriptComponent>>([&](ev::Event& event, auto&& data) {
        lua::Script script(data.entity.get<ScriptComponent>().script);
        lua::load_registry_functions(script, registry);
        lua::load_input_functions(script, d_input);
        lua::load_window_functions(script, *d_window);

        lua::load_entity_transformation_functions(script);
        lua::load_entity_component_functions(script);

        script.call_function<void>("Init", data.entity);
        script.print_globals();
        d_engines.emplace(data.entity, std::make_pair(std::move(script), true));
    });

    dispatcher.subscribe<ecs::Removed<ScriptComponent>>([&](ev::Event& event, auto&& data) {
        auto it = d_engines.find(data.entity);
        if (it != d_engines.end()) {
            it->second.second = false; // alive = false
        }
    });

    const auto handler = [&](ev::Event& event, const char* f, auto&&... args) {
        for (auto& script : active_scripts()) {
            if (script.has_function(f) && script.call_function<bool>(f, std::forward<decltype(args)>(args)...)) {
                event.consume();
            }
        }
    };

    dispatcher.subscribe<ev::WindowResize>([&](ev::Event& event, auto&& data) {
        handler(event, "OnWindowResizeEvent", data.width, data.height);
    });

    dispatcher.subscribe<ev::MouseButtonPressed>([&](ev::Event& event, auto&& data) {
        handler(event, "OnMouseButtonPressedEvent", data.button, data.action, data.mods);
    });

    dispatcher.subscribe<ev::MouseScrolled>([&](ev::Event& event, auto&& data) {
        handler(event, "OnMouseScrolledEvent", data.x_offset, data.y_offset);
    });

    dispatcher.subscribe<CollisionEvent>([&](ev::Event& event, auto&& data) {
        handler(event, "OnCollisionEvent", data.entity1, data.entity2);
    });
}

}