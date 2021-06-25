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

void ScriptRunner::on_event(spkt::registry& registry, ev::Event& event)
{
    if (auto e = event.get_if<spkt::added<ScriptComponent>>()) {
        lua::Script script(e->entity.get<ScriptComponent>().script);
        lua::load_vec3_functions(script);
        lua::load_vec2_functions(script);
        lua::load_registry_functions(script, registry);
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
}

}