#include "LuaInput.h"
#include "LuaGlobals.h"
#include "KeyboardProxy.h"
#include "MouseProxy.h"
#include "Window.h"

#include <lua.hpp>

namespace Sprocket {

void RegisterInputFunctions(lua_State* L)
{
    lua_register(L, "IsKeyDown", &Lua::IsKeyDown);
    lua_register(L, "IsMouseDown", &Lua::IsMouseDown);
    lua_register(L, "GetMousePos", &Lua::GetMousePos);
    lua_register(L, "GetMouseOffset", &Lua::GetMouseOffset);
}

namespace Lua {

int IsKeyDown(lua_State* L)
{
    if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }

    auto k = GetKeyboard(L);
    if (k == nullptr) {
        lua_pushboolean(L, false);
    }
    else {
        int x = (int)lua_tointeger(L, 1);
        lua_pushboolean(L, k->IsKeyDown(x));
    }
    
    return 1;
}

int IsMouseDown(lua_State* L)
{
    if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }

    auto m = GetMouse(L);
    if (m == nullptr) {
        lua_pushboolean(L, false);
    }
    else {
        int x = (int)lua_tointeger(L, 1);
        lua_pushboolean(L, m->IsButtonDown(x));
    }

    return 1;
}

int GetMousePos(lua_State* L)
{
    if (!CheckArgCount(L, 0)) { return luaL_error(L, "Bad number of args"); }

    auto w = GetWindow(L);
    if (w == nullptr) {
        lua_pushnumber(L, 0);
        lua_pushnumber(L, 0);
    }
    else {
        auto offset = w->GetMousePos();
        lua_pushnumber(L, offset.x);
        lua_pushnumber(L, offset.y);
    }
    return 2;
}

int GetMouseOffset(lua_State* L)
{
    if (!CheckArgCount(L, 0)) { return luaL_error(L, "Bad number of args"); }

    auto w = GetWindow(L);
    if (w == nullptr) {
        lua_pushnumber(L, 0);
        lua_pushnumber(L, 0);
    }
    else {
        auto offset = w->GetMouseOffset();
        lua_pushnumber(L, offset.x);
        lua_pushnumber(L, offset.y);
    }
    return 2;
}


}
}