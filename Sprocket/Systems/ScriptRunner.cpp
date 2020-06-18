#include "ScriptRunner.h"
#include "Log.h"

namespace Sprocket {

ScriptRunner::ScriptRunner()
    : d_luaEngine()
{
}

void ScriptRunner::UpdateEntity(double dt, Entity& entity)
{
    if (!entity.Has<ScriptComponent>()) {
        return;
    }
    d_luaEngine.RunOnUpdateScript(dt, entity);
}

void ScriptRunner::UpdateSystem(double dt)
{
    d_luaEngine.OnUpdate(dt);
}

void ScriptRunner::OnEvent(Event& event)
{
    d_luaEngine.OnEvent(event);
}

}