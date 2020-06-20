#include "LuaTransform.h"
#include "LuaGlobals.h"
#include "Entity.h"

#include <lua.hpp>

namespace Sprocket {

void RegisterTransformFunctions(lua_State* L)
{
    lua_register(L, "GetPosition", &Lua::GetPosition);
    lua_register(L, "SetPosition", &Lua::SetPosition);
    lua_register(L, "SetLookAt", &Lua::SetLookAt);
    lua_register(L, "RotateY", &Lua::RotateY);
    lua_register(L, "GetForwardsDir", &Lua::GetForwardsDir);
    lua_register(L, "GetRightDir", &Lua::GetRightDir);
}

namespace Lua {

int GetPosition(lua_State* L)
{
    if (!CheckArgCount(L, 0)) { return luaL_error(L, "Bad number of args"); }

    Entity* entity = GetEntity(L);
    lua_pushnumber(L, entity->Position().x);
    lua_pushnumber(L, entity->Position().y);
    lua_pushnumber(L, entity->Position().z);
    return 3;
}

int SetPosition(lua_State* L)
{
    if (!CheckArgCount(L, 3)) { return luaL_error(L, "Bad number of args"); }

    Entity* entity = GetEntity(L);
    float x = (float)lua_tonumber(L, 1);
    float y = (float)lua_tonumber(L, 2);
    float z = (float)lua_tonumber(L, 3);
    entity->SetPosition({x, y, z});
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

    entity->Position() = {px, py, pz};
    entity->Orientation() = q;
    return 0;
}

int RotateY(lua_State* L)
{
    if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); };

    Entity* entity = GetEntity(L);
    float yaw = (float)lua_tonumber(L, 1);
    auto orientation = entity->Orientation();
    orientation = Maths::Rotate(orientation, {0, 1, 0}, Maths::Radians(yaw));
    entity->Orientation() = orientation;
    return 0;
}

int GetForwardsDir(lua_State* L)
{
    if (!CheckArgCount(L, 0)) { return luaL_error(L, "Bad number of args"); }

    auto forwards = Maths::Forwards(GetEntity(L)->Orientation());
    lua_pushnumber(L, forwards.x);
    lua_pushnumber(L, forwards.y);
    lua_pushnumber(L, forwards.z);
    return 3;
}

int GetRightDir(lua_State* L)
{
    if (!CheckArgCount(L, 0)) { return luaL_error(L, "Bad number of args"); }

    auto right = Maths::Right(GetEntity(L)->Orientation());
    lua_pushnumber(L, right.x);
    lua_pushnumber(L, right.y);
    lua_pushnumber(L, right.z);
    return 3;
}

}
}