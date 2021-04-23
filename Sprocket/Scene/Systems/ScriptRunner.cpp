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

apx::generator<lua::Script&> ScriptRunner::active_scripts()
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

    dispatcher.subscribe<ecs::Added<ScriptComponent>>([&](ev::Event& event) {
        auto data = event.get<ecs::Added<ScriptComponent>>();
        lua::Script script(data.entity.get<ScriptComponent>().script);
        lua::load_registry_functions(script, registry);
        lua::load_input_functions(script, d_input);
        lua::load_window_functions(script, *d_window);

        lua::load_entity_transformation_functions(script);
        lua::load_entity_component_functions(script);

        script.call_function<void>("Init", data.entity);
        d_engines.emplace(data.entity, std::make_pair(std::move(script), true));
    });

    dispatcher.subscribe<ecs::Removed<ScriptComponent>>([&](ev::Event& event) {
        auto data = event.get<ecs::Removed<ScriptComponent>>();
        auto it = d_engines.find(data.entity);
        if (it != d_engines.end()) {
            it->second.second = false; // alive = false
        }
    });

    dispatcher.subscribe<ev::WindowResize>([&](ev::Event& event) {
        auto data = event.get<ev::WindowResize>();
        for (auto& script : active_scripts()) {
            if (script.has_function("OnWindowResizeEvent") &&
                script.call_function<bool>("OnWindowResizeEvent", data.width, data.height)) {
                event.consume();
            }
        }
    });

    dispatcher.subscribe<ev::MouseButtonPressed>([&](ev::Event& event) {
        auto data = event.get<ev::MouseButtonPressed>();
        for (auto& script : active_scripts()) {
            if (script.has_function("OnMouseButtonPressedEvent") &&
                script.call_function<bool>("OnMouseButtonPressedEvent", data.button, data.action, data.mods)) {
                event.consume();
            }
        }
    });

    dispatcher.subscribe<ev::MouseScrolled>([&](ev::Event& event) {
        auto data = event.get<ev::MouseScrolled>();
        for (auto& script : active_scripts()) {
            if (script.has_function("OnMouseScrolledEvent") &&
                script.call_function<bool>("OnMouseScrolledEvent", data.x_offset, data.y_offset)) {
                event.consume();
            }
        }
    });

    dispatcher.subscribe<CollisionEvent>([&](ev::Event& event) {
        auto data = event.get<CollisionEvent>();
        for (auto& script : active_scripts()) {
            if (script.has_function("OnCollisionEvent") &&
                script.call_function<bool>("OnCollisionEvent", data.entity1, data.entity2)) {
                event.consume();
            }
        }
    });
}

}