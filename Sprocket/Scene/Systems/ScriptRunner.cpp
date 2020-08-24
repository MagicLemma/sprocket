#include "ScriptRunner.h"
#include "Log.h"
#include "Entity.h"
#include "Scene.h"
#include "LuaEngine.h"
#include "Components.h"

#include <utility>

namespace Sprocket {

ScriptRunner::ScriptRunner()
{
    d_keyboard.ConsumeAll(false);
}

void ScriptRunner::OnStartup(Scene& scene)
{
    auto AddScript = [&](Entity& entity) {
        auto& luaEngine = d_engines[entity.Id()];
        luaEngine.SetKeyboard(&d_keyboard);
        luaEngine.SetMouse(&d_mouse);
        luaEngine.SetEntity(entity);
        luaEngine.RunScript(entity.Get<ScriptComponent>().script);
        luaEngine.CallInitFunction();
    };

    scene.Each<ScriptComponent>(AddScript);
        // Register all existing scripts

    scene.OnAdd<ScriptComponent>(AddScript);
        // If a new script gets added to an entity, resiter that too.

    scene.OnRemove<ScriptComponent>([&](Entity& entity) {
        d_engines.erase(entity.Id());
    });
}

void ScriptRunner::OnUpdate(Scene& scene, double dt)
{
    d_mouse.OnUpdate();

    scene.Each<ScriptComponent>([&](Entity& entity) {
        auto& luaEngine = d_engines[entity.Id()];
        luaEngine.CallOnUpdateFunction(dt);
    });
}

void ScriptRunner::OnEvent(Scene& scene, Event& event)
{
    d_keyboard.OnEvent(event);
    d_mouse.OnEvent(event);

    if (auto e = event.As<MouseButtonPressedEvent>()) {
        for (auto& [id, luaEngine] : d_engines) {
            luaEngine.CallOnMouseButtonPressedEvent(e);
        }
    }
    else if (auto e = event.As<MouseScrolledEvent>()) {
        for (auto& [id, luaEngine] : d_engines) {
            luaEngine.CallOnMouseScrolledEvent(e);
        }
    }
    else if (auto e = event.As<WindowResizeEvent>()) {
        for (auto& [id, luaEngine] : d_engines) {
            luaEngine.CallOnWindowResizeEvent(e);
        }
    }
}

}