#include "ScriptRunner.h"
#include "Log.h"
#include "LuaEngine.h"

namespace Sprocket {

ScriptRunner::ScriptRunner()
{
}

void ScriptRunner::UpdateEntity(double dt, Entity& entity)
{
    if (!entity.Has<ScriptComponent>()) {
        return;
    }

    auto& sc = entity.Get<ScriptComponent>();
    sc.luaEngine.SetKeyboard(&d_keyboard);
    sc.luaEngine.SetMouse(&d_mouse);

    if (sc.active) {
        sc.luaEngine.RunOnUpdateScript(dt, entity);
    }
}

void ScriptRunner::UpdateSystem(double dt)
{
    d_mouse.OnUpdate();
}

void ScriptRunner::OnEvent(Event& event)
{
    d_keyboard.OnEvent(event);
    d_mouse.OnEvent(event);
}

}