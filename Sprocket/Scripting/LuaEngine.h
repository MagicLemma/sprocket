#pragma once
#include "Event.h"
#include "KeyboardProxy.h"
#include "MouseProxy.h"

#include <lua.hpp>

#include <string>

namespace Sprocket {

class Entity;

class LuaEngine
{
    lua_State* d_L;

public:
    LuaEngine();
    ~LuaEngine();

    void RunScript(const std::string& filename);
    
    void CallOnUpdateFunction(double dt);

    void SetEntity(Entity* e);
    void SetKeyboard(KeyboardProxy* k);
    void SetMouse(MouseProxy* m);
};

}