#include "LuaCamera.h"
#include "LuaGlobals.h"
#include "Entity.h"

#include <lua.hpp>

namespace Sprocket {

void RegisterCameraFunctions(lua_State* L)
{
    lua_register(L, "GetPitch", &Lua::GetPitch);
    lua_register(L, "SetPitch", &Lua::SetPitch);
}

namespace Lua {

int GetPitch(lua_State* L)
{
    if (!CheckArgCount(L, 0)) { return luaL_error(L, "Bad number of args"); }

    auto c = GetEntity(L)->Get<CameraComponent>();
    lua_pushnumber(L, c.pitch);
    return 1;
}

int SetPitch(lua_State* L)
{
    if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }

    float x = (float)lua_tonumber(L, 1);
    auto& c = GetEntity(L)->Get<CameraComponent>();
    c.pitch = x;
    return 0;
}

}
}