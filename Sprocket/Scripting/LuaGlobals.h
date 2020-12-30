#pragma once
#include "ECS.h"

#include <unordered_map>

class lua_State;

namespace Sprocket {

class Scene;
class Window;
class InputProxy;
class ECS::Entity;

// Functions for access core global values from a Lua state. These
// are used for implementing Lua C++ functions and are not callable
// directly within Lua.

Scene* GetScene(lua_State* L);
Window* GetWindow(lua_State* L);
InputProxy* GetInput(lua_State* L);
ECS::Entity* GetEntity(lua_State* L);

bool CheckReturnCode(lua_State* L, int rc);
bool CheckArgCount(lua_State* L, int argc);
    // Functions to help validate that the functions in Lua have been
    // used corrently.

}