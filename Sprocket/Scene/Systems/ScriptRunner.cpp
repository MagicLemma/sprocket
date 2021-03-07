#include "ScriptRunner.h"
#include "Log.h"
#include "ECS.h"
#include "Scene.h"
#include "LuaScript.h"
#include "LuaLibrary.h"
#include "Components.h"

#include <utility>

namespace Sprocket {

ScriptRunner::ScriptRunner(Window* window)
    : d_window(window)
{
}

void ScriptRunner::OnStartup(Scene& scene)
{
    scene.Entities().OnAdd<ScriptComponent>([&](ecs::Entity entity) {
        lua::Script script(entity.Get<ScriptComponent>().script);
        lua::register_scene_functions(script, scene);
        lua::register_input_functions(script, d_input);
        lua::register_window_functions(script, *d_window);

        lua::register_entity_transformation_functions(script);
        lua::register_entity_component_functions(script);

        script.call_function("Init", entity);
        script.print_globals();
        d_engines.emplace(entity, std::make_pair(std::move(script), true));
    });

    scene.Entities().OnRemove<ScriptComponent>([&](ecs::Entity entity) {
        auto it = d_engines.find(entity);
        if (it != d_engines.end()) {
            it->second.second = false; // alive = false
        }
    });
}

void ScriptRunner::OnUpdate(Scene& scene, double dt)
{
    // We delete scripts here rather then with OnRemove otherwise we would segfault if
    // a script tries to delete its own entity, which is functionality that we want to
    // support.
    for (auto it = d_engines.begin(); it != d_engines.end();) {
        auto& entity = it->first;
        auto& [script, alive] = it->second;

        if (alive) {
            if (entity.Get<ScriptComponent>().active) {
                script.call_function("OnUpdate", entity, dt);
            }
            ++it;
        } else {
            it = d_engines.erase(it);
        }
    }
}

void ScriptRunner::OnEvent(Scene& scene, ev::Event& event)
{
    d_input.OnEvent(event);

    // WINDOW EVENTS
    for (auto& [entity, pair] : d_engines) {
        auto& [script, alive] = pair;
        if (!(alive && entity.Get<ScriptComponent>().active)) { continue; }
        script.on_event(event);
    }
}

}