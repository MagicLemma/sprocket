#include "LuaGlobals.h"
#include "Log.h"

#include <lua.hpp>

namespace Sprocket {

KeyboardProxy* GetKeyboard(lua_State* L)
{
    lua_getglobal(L, "__keyboard__");
    KeyboardProxy* k = nullptr;
    if (!lua_isnil(L, -1)) {
        k = (KeyboardProxy*)lua_touserdata(L, -1);
    }
    lua_pop(L, 1);
    return k;
}

MouseProxy* GetMouse(lua_State* L)
{
    lua_getglobal(L, "__mouse__");
    MouseProxy* m = nullptr;
    if (!lua_isnil(L, -1)) {
        m = (MouseProxy*)lua_touserdata(L, -1);
    }
    lua_pop(L, 1);
    return m;
}

Entity* GetEntity(lua_State* L)
{
    lua_getglobal(L, "__entity__");
    Entity* e = (Entity*)lua_touserdata(L, -1);
    lua_pop(L, 1);
    return e;
}

bool CheckReturnCode(lua_State* L, int rc)
{
    if (rc != LUA_OK) {
        const char* error = lua_tostring(L, -1);
        SPKT_LOG_ERROR("[Lua]: {}", error);
        return false;
    }
    return true;
}

bool CheckArgCount(lua_State* L, int argc)
{
    int args = lua_gettop(L);
    if (args != argc) {
        SPKT_LOG_ERROR("[Lua]: Expected {} args, got {}", argc, args);
        return false;
    }
    return true;
}

}