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
        auto& [luaEngine, alive] = d_engines[entity];
        alive = true;
        luaEngine.SetScene(&scene);
        luaEngine.SetWindow(d_window);
        luaEngine.SetInput(&d_input);
        luaEngine.RunScript(entity.Get<ScriptComponent>().script);
        luaEngine.Call("Init", entity);
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
        if (alive && entity.Get<ScriptComponent>().active) {
            script.Call("OnUpdate", entity, dt);
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
    for (auto& [entity, pair] : d_engines) {
        auto& [script, alive] = pair;
        if (!(alive && entity.Get<ScriptComponent>().active)) { continue; }

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