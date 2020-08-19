#include "LuaCamera.h"
#include "LuaGlobals.h"
#include "Entity.h"
#include "Maths.h"
#include "Components.h"

#include <lua.hpp>

namespace Sprocket {

void RegisterCameraFunctions(lua_State* L)
{
    lua_register(L, "Lua_GetCameraComponent", &Lua::GetCameraComponent);
    lua_register(L, "Lua_SetCameraComponent", &Lua::SetCameraComponent);
}

namespace Lua {

int GetCameraComponent(lua_State* L)
{
    if (!CheckArgCount(L, 0)) { return luaL_error(L, "Bad number of args"); }
    assert(GetEntity(L)->Has<CameraComponent>());

    const auto& c = GetEntity(L)->Get<CameraComponent>();
    lua_pushnumber(L, c.fov);
    lua_pushnumber(L, c.pitch);
    return 2;
}

int SetCameraComponent(lua_State* L)
{
    if (!CheckArgCount(L, 2)) { return luaL_error(L, "Bad number of args"); }

    auto& c = GetEntity(L)->Get<CameraComponent>();
    c.fov = (float)lua_tonumber(L, 1);
    c.pitch = (float)lua_tonumber(L, 2);
    return 0;
}

}
}