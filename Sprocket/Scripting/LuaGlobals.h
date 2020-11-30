#pragma once
#include "Entity.h"

#include <unordered_map>

class lua_State;

namespace Sprocket {

class Window;
class KeyboardProxy;
class MouseProxy;
class Entity;

// Functions for access core global values from a Lua state. These
// are used for implementing Lua C++ functions and are not callable
// directly within Lua.

Window* GetWindow(lua_State* L);
KeyboardProxy* GetKeyboard(lua_State* L);
MouseProxy* GetMouse(lua_State* L);
Entity* GetEntity(lua_State* L);

bool CheckReturnCode(lua_State* L, int rc);
bool CheckArgCount(lua_State* L, int argc);
    // Functions to help validate that the functions in Lua have been
    // used corrently.

}