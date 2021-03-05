#include "ScriptRunner.h"
#include "Log.h"
#include "ECS.h"
#include "Scene.h"
#include "LuaEngine.h"
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
        auto& [engine, alive] = d_engines[entity];
        alive = true;

        engine.Set("__scene__", &scene);
        engine.Set("__window__", d_window);
        engine.Set("__input__", &d_input);

        engine.RunScript(entity.Get<ScriptComponent>().script);
        engine.Call("Init", entity);
        engine.PrintGlobals();
    });

    scene.Entities().OnRemove<ScriptComponent>([&](ecs::Entity entity) {
        d_engines[entity].second = false; // alive = false
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
                script.Call("OnUpdate", entity, dt);
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