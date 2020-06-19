#pragma once
#include "Entity.h"
#include "Event.h"
#include "KeyboardProxy.h"
#include "MouseProxy.h"

#include <lua.hpp>

#include <string>

namespace Sprocket {

class LuaEngine
{
    lua_State* d_L;

    KeyboardProxy d_keyboard;
    MouseProxy d_mouse;

public:
    LuaEngine();
    ~LuaEngine();

    void RunScript(const std::string& filename);
    void RunOnUpdateScript(double dt, Entity& entity);

    void OnUpdate(double dt);
    void OnEvent(Event& event);
};

}