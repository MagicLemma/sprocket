#include "LuaPhysics.h"
#include "LuaGlobals.h"
#include "Entity.h"
#include "Maths.h"
#include "Components.h"

#include <lua.hpp>
#include <cassert>

namespace Sprocket {

void RegisterPhysicsFunctions(lua_State* L)
{
    lua_register(L, "Lua_GetPhysicsComponent", &Lua::GetPhysicsComponent);
    lua_register(L, "Lua_SetPhysicsComponent", &Lua::SetPhysicsComponent);
}

namespace Lua {

int GetPhysicsComponent(lua_State* L)
{
    if (!CheckArgCount(L, 0)) { return luaL_error(L, "Bad number of args"); }
    assert(GetEntity(L)->Has<PhysicsComponent>());

    const auto& c = GetEntity(L)->Get<PhysicsComponent>();
    lua_pushnumber(L, c.velocity.x);
    lua_pushnumber(L, c.velocity.y);
    lua_pushnumber(L, c.velocity.z);
    lua_pushboolean(L, c.gravity);
    lua_pushboolean(L, c.frozen);
    lua_pushnumber(L, c.mass);
    lua_pushnumber(L, c.bounciness);
    lua_pushnumber(L, c.frictionCoefficient);
    lua_pushnumber(L, c.rollingResistance);
    lua_pushnumber(L, c.force.x);
    lua_pushnumber(L, c.force.y);
    lua_pushnumber(L, c.force.z);
    lua_pushboolean(L, c.onFloor);
    return 13;
}

int SetPhysicsComponent(lua_State* L)
{
    if (!CheckArgCount(L, 13)) { return luaL_error(L, "Bad number of args"); }

    auto& c = GetEntity(L)->Get<PhysicsComponent>();
    c.velocity.x = (float)lua_tonumber(L, 1);
    c.velocity.y = (float)lua_tonumber(L, 2);
    c.velocity.z = (float)lua_tonumber(L, 3);
    c.gravity = (bool)lua_toboolean(L, 4);
    c.frozen = (bool)lua_toboolean(L, 5);
    c.mass = (float)lua_tonumber(L, 6);
    c.bounciness = (float)lua_tonumber(L, 7);
    c.frictionCoefficient = (float)lua_tonumber(L, 8);
    c.rollingResistance = (float)lua_tonumber(L, 9);
    c.force.x = (float)lua_tonumber(L, 10);
    c.force.y = (float)lua_tonumber(L, 11);
    c.force.z = (float)lua_tonumber(L, 12);
    c.onFloor = (bool)lua_toboolean(L, 13);
    return 0;
}

}
    
}