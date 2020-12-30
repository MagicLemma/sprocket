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
        luaEngine.SetScene(&scene);
        luaEngine.SetWindow(d_window);
        luaEngine.SetInput(&d_input);
        luaEngine.SetEntity(entity);
        luaEngine.RunScript(entity.Get<ScriptComponent>().script);
        luaEngine.CallInitFunction();
    });

    //scene.Entities().OnRemove<ScriptComponent>([&](ECS::Entity entity) {
    //    d_engines.erase(entity.Id());
    //});
}

void ScriptRunner::OnUpdate(Scene& scene, double dt)
{
    std::vector<u32> toDelete;

    for (auto& [id, script] : d_engines) {
        auto entity = scene.Entities().Get(id);
        if (entity.Valid() && entity.Has<ScriptComponent>()) {
            script.CallOnUpdateFunction(dt);
        } else {
            toDelete.push_back(id);
        }
    }
    for (auto id : toDelete) {
        d_engines.erase(id);
    }
}

void ScriptRunner::OnEvent(Scene& scene, Event& event)
{
    d_input.OnEvent(event);

    // WINDOW EVENTS
    for (auto& [id, luaEngine] : d_engines) {
        auto entity = scene.Entities().Get(id);
        if (!(entity.Valid() && entity.Has<ScriptComponent>())) {
            continue;
        }

        if (auto e = event.As<WindowResizeEvent>()) {
            luaEngine.CallOnWindowResizeEvent(e);
        }
        else if (auto e = event.As<WindowClosedEvent>()) {
            luaEngine.CallOnWindowClosedEvent(e);
        }
        else if (auto e = event.As<WindowGotFocusEvent>()) {
            luaEngine.CallOnWindowGotFocusEvent(e);
        }
        else if (auto e = event.As<WindowLostFocusEvent>()) {
            luaEngine.CallOnWindowLostFocusEvent(e);
        }
        else if (auto e = event.As<WindowMaximizeEvent>()) {
            luaEngine.CallOnWindowMaximizeEvent(e);
        }
        else if (auto e = event.As<WindowMinimizeEvent>()) {
            luaEngine.CallOnWindowMinimizeEvent(e);
        }

        // MOUSE EVENTS
        else if (auto e = event.As<MouseButtonPressedEvent>()) {
            luaEngine.CallOnMouseButtonPressedEvent(e);
        }
        else if (auto e = event.As<MouseButtonReleasedEvent>()) {
            luaEngine.CallOnMouseButtonReleasedEvent(e);
        }
        else if (auto e = event.As<MouseMovedEvent>()) {
            luaEngine.CallOnMouseMovedEvent(e);
        }
        else if (auto e = event.As<MouseScrolledEvent>()) {
            luaEngine.CallOnMouseScrolledEvent(e);
        }
            
        // KEYBOARD
        else if (auto e = event.As<KeyboardButtonPressedEvent>()) {
            luaEngine.CallOnKeyboardButtonPressedEvent(e);
        }
        else if (auto e = event.As<KeyboardButtonReleasedEvent>()) {
            luaEngine.CallOnKeyboardButtonReleasedEvent(e);
        }
        else if (auto e = event.As<KeyboardButtonHeldEvent>()) {
            luaEngine.CallOnKeyboardButtonHeldEvent(e);
        }
        else if (auto e = event.As<KeyboardKeyTypedEvent>()) {
            luaEngine.CallOnKeyboardKeyTypedEvent(e);
        }
    }
}

}