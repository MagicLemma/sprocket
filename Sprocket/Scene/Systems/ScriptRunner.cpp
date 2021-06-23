#include "ScriptRunner.h"
#include "Log.h"
#include "ECS.h"
#include "Scene.h"
#include "LuaScript.h"
#include "LuaLibrary.h"
#include "LuaMaths.h"
#include "Components.h"

#include <utility>

namespace Sprocket {

ScriptRunner::ScriptRunner(Window* window)
    : d_window(window)
{
}

void ScriptRunner::on_update(spkt::registry&, double dt)
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

void ScriptRunner::on_event(spkt::registry& registry, ev::Event& event)
{
    d_input.on_event(event);

    if (auto e = event.get_if<spkt::added<ScriptComponent>>()) {
        lua::Script script(e->entity.get<ScriptComponent>().script);
        lua::load_vec3_functions(script);
        lua::load_vec2_functions(script);
        lua::load_registry_functions(script, registry);
        lua::load_input_functions(script, d_input);
        lua::load_window_functions(script, *d_window);

        lua::load_entity_transformation_functions(script);
        lua::load_entity_component_functions(script);

        script.call_function<void>("Init", e->entity);
        d_engines.emplace(e->entity, std::make_pair(std::move(script), true));
    }
    else if (auto e = event.get_if<spkt::removed<ScriptComponent>>()) {
        auto it = d_engines.find(e->entity);
        if (it != d_engines.end()) {
            it->second.second = false; // alive = false
        }
    }
    else if (auto e = event.get_if<ev::WindowResize>()) {
        for (auto& script : active_scripts()) {
            if (script.has_function("OnWindowResizeEvent") &&
                script.call_function<bool>("OnWindowResizeEvent", e->width, e->height)) {
                event.consume();
            }
        }
    }
    else if (auto e = event.get_if<ev::MouseButtonPressed>()) {
        for (auto& script : active_scripts()) {
            if (script.has_function("OnMouseButtonPressedEvent") &&
                script.call_function<bool>("OnMouseButtonPressedEvent", e->button, e->action, e->mods)) {
                event.consume();
            }
        }
    }
    else if (auto e = event.get_if<ev::MouseScrolled>()) {
        for (auto& script : active_scripts()) {
            if (script.has_function("OnMouseScrolledEvent") &&
                script.call_function<bool>("OnMouseScrolledEvent", e->x_offset, e->y_offset)) {
                event.consume();
            }
        }
    }
}

}