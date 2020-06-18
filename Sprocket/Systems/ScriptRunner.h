#pragma once
#include "EntitySystem.h"
#include "Lua.h"

namespace Sprocket {

class ScriptRunner : public EntitySystem
{
    LuaEngine d_luaEngine;

public:
    ScriptRunner();

    void UpdateEntity(double dt, Entity& entity) override;

    void UpdateSystem(double dt) override;
    void OnEvent(Event& event) override;
};

}