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
        auto& [luaEngine, alive] = d_engines[entity.Id()];
        alive = true;
        luaEngine.SetScene(&scene);
        luaEngine.SetWindow(d_window);
        luaEngine.SetInput(&d_input);
        luaEngine.SetEntity(entity);
        luaEngine.RunScript(entity.Get<ScriptComponent>().script);
        luaEngine.CallInitFunction();
    });

    scene.Entities().OnRemove<ScriptComponent>([&](ecs::Entity entity) {
        d_engines[entity.Id()].second = false; // alive = false
    });
}

void ScriptRunner::OnUpdate(Scene& scene, double dt)
{
    // We delete scripts here rather then with OnRemove otherwise we would segfault if
    // a script tries to delete its own entity, which is functionality that we want to
    // support.
    for (auto it = d_engines.begin(); it != d_engines.end();) {
        auto& [script, alive] = it->second;
        if (alive) {
            script.CallOnUpdateFunction(dt);
            ++it;
        } else {
            it = d_engines.erase(it);
        }
    }
}

void ScriptRunner::OnEvent(Scene& scene, Event& event)
{
    d_input.OnEvent(event);

    // WINDOW EVENTS
    for (auto& [id, pair] : d_engines) {
        auto& [script, alive] = pair;
        if (!alive) { continue; }

        if (auto e = event.As<WindowResizeEvent>()) {
            script.CallOnWindowResizeEvent(e);
        }
        else if (auto e = event.As<WindowClosedEvent>()) {
            script.CallOnWindowClosedEvent(e);
        }
        else if (auto e = event.As<WindowGotFocusEvent>()) {
            script.CallOnWindowGotFocusEvent(e);
        }
        else if (auto e = event.As<WindowLostFocusEvent>()) {
            script.CallOnWindowLostFocusEvent(e);
        }
        else if (auto e = event.As<WindowMaximizeEvent>()) {
            script.CallOnWindowMaximizeEvent(e);
        }
        else if (auto e = event.As<WindowMinimizeEvent>()) {
            script.CallOnWindowMinimizeEvent(e);
        }

        // MOUSE EVENTS
        else if (auto e = event.As<MouseButtonPressedEvent>()) {
            script.CallOnMouseButtonPressedEvent(e);
        }
        else if (auto e = event.As<MouseButtonReleasedEvent>()) {
            script.CallOnMouseButtonReleasedEvent(e);
        }
        else if (auto e = event.As<MouseMovedEvent>()) {
            script.CallOnMouseMovedEvent(e);
        }
        else if (auto e = event.As<MouseScrolledEvent>()) {
            script.CallOnMouseScrolledEvent(e);
        }
            
        // KEYBOARD
        else if (auto e = event.As<KeyboardButtonPressedEvent>()) {
            script.CallOnKeyboardButtonPressedEvent(e);
        }
        else if (auto e = event.As<KeyboardButtonReleasedEvent>()) {
            script.CallOnKeyboardButtonReleasedEvent(e);
        }
        else if (auto e = event.As<KeyboardButtonHeldEvent>()) {
            script.CallOnKeyboardButtonHeldEvent(e);
        }
        else if (auto e = event.As<KeyboardKeyTypedEvent>()) {
            script.CallOnKeyboardKeyTypedEvent(e);
        }
    }
}

}