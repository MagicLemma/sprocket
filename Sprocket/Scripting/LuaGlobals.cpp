#include "LuaGlobals.h"
#include "Log.h"

#include <lua.hpp>

namespace Sprocket {

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

ECS::Entity* GetEntity(lua_State* L)
{
    lua_getglobal(L, "__entity__");
    ECS::Entity* e = (ECS::Entity*)lua_touserdata(L, -1);
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