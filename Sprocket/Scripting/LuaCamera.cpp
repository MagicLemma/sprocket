#include "LuaCamera.h"
#include "LuaGlobals.h"
#include "Entity.h"
#include "Maths.h"

#include "CameraComponent.h"

#include <lua.hpp>

namespace Sprocket {

void RegisterCameraFunctions(lua_State* L)
{
    lua_register(L, "GetPitch", &Lua::GetPitch);
    lua_register(L, "SetPitch", &Lua::SetPitch);
    lua_register(L, "SetPerspectiveCamera", &Lua::SetPerspectiveCamera);
    lua_register(L, "SetOrthographicCamera", &Lua::SetOrthographicCamera);
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

int SetPerspectiveCamera(lua_State* L)
{
    if (!CheckArgCount(L, 4)) { return luaL_error(L, "Bad number of args"); }
    
    float aspectRatio = (float)lua_tonumber(L, 1);
    float fov = (float)lua_tonumber(L, 2);
    float nearPlane = (float)lua_tonumber(L, 3);
    float farPlane = (float)lua_tonumber(L, 4);
    auto& c = GetEntity(L)->Get<CameraComponent>();
    c.projection = Maths::Perspective(aspectRatio, fov, nearPlane, farPlane);
    return 0;
}

int SetOrthographicCamera(lua_State* L)
{
    if (!CheckArgCount(L, 6)) { return luaL_error(L, "Bad number of args"); }

    float leftEdge = (float)lua_tonumber(L, 1);
    float rightEdge = (float)lua_tonumber(L, 2);
    float bottomEdge = (float)lua_tonumber(L, 3);
    float topEdge = (float)lua_tonumber(L, 4);
    float nearEdge = (float)lua_tonumber(L, 5);
    float farEdge = (float)lua_tonumber(L, 6);
    auto& c = GetEntity(L)->Get<CameraComponent>();
    c.projection = Maths::Ortho(
        leftEdge, rightEdge, bottomEdge, topEdge, nearEdge, farEdge);
    return 0;
}

}
}