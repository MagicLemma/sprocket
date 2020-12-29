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
    scene.Entities().OnAdd<ScriptComponent>([&](ECS::Entity entity) {
        auto& luaEngine = d_engines[entity.Id()];
        luaEngine.SetWindow(d_window);
        luaEngine.SetInput(&d_input);
        luaEngine.SetEntity(entity);
        luaEngine.RunScript(entity.Get<ScriptComponent>().script);
        luaEngine.CallInitFunction();
    });

    scene.Entities().OnRemove<ScriptComponent>([&](ECS::Entity entity) {
        d_engines.erase(entity.Id());
    });
}

void ScriptRunner::OnUpdate(Scene& scene, double dt)
{
    for (auto entity : scene.Entities().Safe()) {
        if (entity.Has<ScriptComponent>()) {
            auto& luaEngine = d_engines[entity.Id()];
            luaEngine.CallOnUpdateFunction(dt);
        }
    }
}

void ScriptRunner::OnEvent(Scene& scene, Event& event)
{
    d_input.OnEvent(event);

    // WINDOW EVENTS
    if (auto e = event.As<WindowResizeEvent>()) {
        for (auto& [id, luaEngine] : d_engines) {
            luaEngine.CallOnWindowResizeEvent(e);
        }
    }
    else if (auto e = event.As<WindowClosedEvent>()) {
        for (auto& [id, luaEngine] : d_engines) {
            luaEngine.CallOnWindowClosedEvent(e);
        }
    }
    else if (auto e = event.As<WindowGotFocusEvent>()) {
        for (auto& [id, luaEngine] : d_engines) {
            luaEngine.CallOnWindowGotFocusEvent(e);
        }
    }
    else if (auto e = event.As<WindowLostFocusEvent>()) {
        for (auto& [id, luaEngine] : d_engines) {
            luaEngine.CallOnWindowLostFocusEvent(e);
        }
    }
    else if (auto e = event.As<WindowMaximizeEvent>()) {
        for (auto& [id, luaEngine] : d_engines) {
            luaEngine.CallOnWindowMaximizeEvent(e);
        }
    }
    else if (auto e = event.As<WindowMinimizeEvent>()) {
        for (auto& [id, luaEngine] : d_engines) {
            luaEngine.CallOnWindowMinimizeEvent(e);
        }
    }

    // MOUSE EVENTS
    else if (auto e = event.As<MouseButtonPressedEvent>()) {
        for (auto& [id, luaEngine] : d_engines) {
            luaEngine.CallOnMouseButtonPressedEvent(e);
        }
    }
    else if (auto e = event.As<MouseButtonReleasedEvent>()) {
        for (auto& [id, luaEngine] : d_engines) {
            luaEngine.CallOnMouseButtonReleasedEvent(e);
        }
    }
    else if (auto e = event.As<MouseMovedEvent>()) {
        for (auto& [id, luaEngine] : d_engines) {
            luaEngine.CallOnMouseMovedEvent(e);
        }
    }
    else if (auto e = event.As<MouseScrolledEvent>()) {
        for (auto& [id, luaEngine] : d_engines) {
            luaEngine.CallOnMouseScrolledEvent(e);
        }
    }
        
    // KEYBOARD
    else if (auto e = event.As<KeyboardButtonPressedEvent>()) {
        for (auto& [id, luaEngine] : d_engines) {
            luaEngine.CallOnKeyboardButtonPressedEvent(e);
        }
    }
    else if (auto e = event.As<KeyboardButtonReleasedEvent>()) {
        for (auto& [id, luaEngine] : d_engines) {
            luaEngine.CallOnKeyboardButtonReleasedEvent(e);
        }
    }
    else if (auto e = event.As<KeyboardButtonHeldEvent>()) {
        for (auto& [id, luaEngine] : d_engines) {
            luaEngine.CallOnKeyboardButtonHeldEvent(e);
        }
    }
    else if (auto e = event.As<KeyboardKeyTypedEvent>()) {
        for (auto& [id, luaEngine] : d_engines) {
            luaEngine.CallOnKeyboardKeyTypedEvent(e);
        }
    }
}

}