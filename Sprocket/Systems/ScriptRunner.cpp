#include "ScriptRunner.h"
#include "Log.h"
#include "LuaEngine.h"

namespace Sprocket {

ScriptRunner::ScriptRunner()
{
    d_keyboard.ConsumeAll(false);
}

void ScriptRunner::UpdateEntity(double dt, Entity& entity)
{
    if (!entity.Has<ScriptComponent>()) {
        return;
    }

    auto& sc = entity.Get<ScriptComponent>();

    // TODO: Move these so they only get called once.
    sc.luaEngine.SetKeyboard(&d_keyboard);
    sc.luaEngine.SetMouse(&d_mouse);
    sc.luaEngine.SetEntity(&entity);

    if (sc.active) {
        sc.luaEngine.CallOnUpdateFunction(dt);
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