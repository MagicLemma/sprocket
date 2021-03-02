#include "LuaGlobals.h"
#include "Log.h"

#include <lua.hpp>

namespace Sprocket {

Scene* GetScene(lua_State* L)
{
    lua_getglobal(L, "__scene__");
    Scene* s = nullptr;
    if (!lua_isnil(L, -1)) {
        s = (Scene*)lua_touserdata(L, -1);
    }
    lua_pop(L, 1);
    return s;
}

Window* GetWindow(lua_State* L)
{
    lua_getglobal(L, "__window__");
    Window* w = nullptr;
    if (!lua_isnil(L, -1)) {
        w = (Window*)lua_touserdata(L, -1);
    }
    lua_pop(L, 1);
    return w;
}

InputProxy* GetInput(lua_State* L)
{
    lua_getglobal(L, "__input__");
    InputProxy* ip = nullptr;
    if (!lua_isnil(L, -1)) {
        ip = (InputProxy*)lua_touserdata(L, -1);
    }
    lua_pop(L, 1);
    return ip;
}

bool CheckReturnCode(lua_State* L, int rc)
{
    if (rc != LUA_OK) {
        const char* error = lua_tostring(L, -1);
        log::error("[Lua]: {}", error);
        return false;
    }
    return true;
}

bool CheckArgCount(lua_State* L, int argc)
{
    int args = lua_gettop(L);
    if (args != argc) {
        log::error("[Lua]: Expected {} args, got {}", argc, args);
        return false;
    }
    return true;
}

}