#include "LuaTransform.h"
#include "LuaGlobals.h"
#include "Entity.h"

#include <lua.hpp>

namespace Sprocket {

void RegisterTransformFunctions(lua_State* L)
{
    // Most of these are prefixed with Lua_ as they get wrapped in the
    // Lua world so the args and return values are Vec3 objects. See
    // Sprocket.lua for the wrappings.
    lua_register(L, "Lua_GetPosition", &Lua::GetPosition);
    lua_register(L, "Lua_SetPosition", &Lua::SetPosition);
    lua_register(L, "Lua_SetLookAt", &Lua::SetLookAt);
    
    lua_register(L, "Lua_GetForwardsDir", &Lua::GetForwardsDir);
    lua_register(L, "Lua_GetRightDir", &Lua::GetRightDir);

    lua_register(L, "RotateY", &Lua::RotateY);
}

namespace Lua {

int GetPosition(lua_State* L)
{
    if (!CheckArgCount(L, 0)) { return luaL_error(L, "Bad number of args"); }

    Entity* entity = GetEntity(L);
    auto& tr = entity->Get<TransformComponent>();
    lua_pushnumber(L, tr.position.x);
    lua_pushnumber(L, tr.position.y);
    lua_pushnumber(L, tr.position.z);
    return 3;
}

int SetPosition(lua_State* L)
{
    if (!CheckArgCount(L, 3)) { return luaL_error(L, "Bad number of args"); }

    Entity* entity = GetEntity(L);
    auto& tr = entity->Get<TransformComponent>();
    float x = (float)lua_tonumber(L, 1);
    float y = (float)lua_tonumber(L, 2);
    float z = (float)lua_tonumber(L, 3);
    tr.position = {x, y, z};
    return 0;
}

int SetLookAt(lua_State* L)
{
    if (!CheckArgCount(L, 6)) { return luaL_error(L, "Bad number of args"); }

    Entity* entity = GetEntity(L);
    float px = (float)lua_tonumber(L, 1);
    float py = (float)lua_tonumber(L, 2);
    float pz = (float)lua_tonumber(L, 3);

    float tx = (float)lua_tonumber(L, 4);
    float ty = (float)lua_tonumber(L, 5);
    float tz = (float)lua_tonumber(L, 6);
    Maths::quat q = Maths::LookAtQuat({px, py, pz}, {tx, ty, tz});

    auto& tr = entity->Get<TransformComponent>();
    tr.position = {px, py, pz};
    tr.orientation = q;
    return 0;
}

int RotateY(lua_State* L)
{
    if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); };

    Entity* entity = GetEntity(L);
    auto& tr = entity->Get<TransformComponent>();

    float yaw = (float)lua_tonumber(L, 1);
    tr.orientation = Maths::Rotate(tr.orientation, {0, 1, 0}, Maths::Radians(yaw));
    return 0;
}

int GetForwardsDir(lua_State* L)
{
    if (!CheckArgCount(L, 0)) { return luaL_error(L, "Bad number of args"); }

    auto& tr = GetEntity(L)->Get<TransformComponent>();
    auto forwards = Maths::Forwards(tr.orientation);
    lua_pushnumber(L, forwards.x);
    lua_pushnumber(L, forwards.y);
    lua_pushnumber(L, forwards.z);
    return 3;
}

int GetRightDir(lua_State* L)
{
    if (!CheckArgCount(L, 0)) { return luaL_error(L, "Bad number of args"); }

    Entity* e = GetEntity(L);
    SPKT_LOG_INFO("{}", e->Name());

    auto& tr = GetEntity(L)->Get<TransformComponent>();
    auto right = Maths::Right(tr.orientation);
    lua_pushnumber(L, right.x);
    lua_pushnumber(L, right.y);
    lua_pushnumber(L, right.z);
    return 3;
}

}
}