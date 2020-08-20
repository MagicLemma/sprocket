// GENERATED FILE @ 2020-08-20 23:53:20.357268

#include "LuaComponents.h"
#include "LuaGlobals.h"
#include "Entity.h"
#include "Maths.h"
#include "Components.h"

#include <lua.hpp>

namespace Sprocket {

void RegisterComponentFunctions(lua_State* L)
{
    lua_register(L, "Lua_GetNameComponent", &Lua::GetNameComponent);
    lua_register(L, "Lua_SetNameComponent", &Lua::SetNameComponent);

    lua_register(L, "Lua_GetTransformComponent", &Lua::GetTransformComponent);
    lua_register(L, "Lua_SetTransformComponent", &Lua::SetTransformComponent);

    lua_register(L, "Lua_GetModelComponent", &Lua::GetModelComponent);
    lua_register(L, "Lua_SetModelComponent", &Lua::SetModelComponent);

    lua_register(L, "Lua_GetPhysicsComponent", &Lua::GetPhysicsComponent);
    lua_register(L, "Lua_SetPhysicsComponent", &Lua::SetPhysicsComponent);

    lua_register(L, "Lua_GetCameraComponent", &Lua::GetCameraComponent);
    lua_register(L, "Lua_SetCameraComponent", &Lua::SetCameraComponent);

    lua_register(L, "Lua_GetSelectComponent", &Lua::GetSelectComponent);
    lua_register(L, "Lua_SetSelectComponent", &Lua::SetSelectComponent);

    lua_register(L, "Lua_GetPathComponent", &Lua::GetPathComponent);
    lua_register(L, "Lua_SetPathComponent", &Lua::SetPathComponent);

    lua_register(L, "Lua_GetGridComponent", &Lua::GetGridComponent);
    lua_register(L, "Lua_SetGridComponent", &Lua::SetGridComponent);

}

namespace Lua {

int GetNameComponent(lua_State* L)
{
    if (!CheckArgCount(L, 0)) { return luaL_error(L, "Bad number of args"); }
    assert(GetEntity(L)->Has<NameComponent>());

    const auto& c = GetEntity(L)->Get<NameComponent>();
    lua_pushstring(L, c.name.c_str());
    return 1;
}

int SetNameComponent(lua_State* L)
{
    if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }

    auto& c = GetEntity(L)->Get<NameComponent>();
    c.name = std::string(lua_tostring(L, 1));
    return 0;
}

int GetTransformComponent(lua_State* L)
{
    if (!CheckArgCount(L, 0)) { return luaL_error(L, "Bad number of args"); }
    assert(GetEntity(L)->Has<TransformComponent>());

    const auto& c = GetEntity(L)->Get<TransformComponent>();
    lua_pushnumber(L, c.position.x);
    lua_pushnumber(L, c.position.y);
    lua_pushnumber(L, c.position.z);
    return 3;
}

int SetTransformComponent(lua_State* L)
{
    if (!CheckArgCount(L, 3)) { return luaL_error(L, "Bad number of args"); }

    auto& c = GetEntity(L)->Get<TransformComponent>();
    c.position.x = (float)lua_tonumber(L, 1);
    c.position.y = (float)lua_tonumber(L, 2);
    c.position.z = (float)lua_tonumber(L, 3);
    return 0;
}

int GetModelComponent(lua_State* L)
{
    if (!CheckArgCount(L, 0)) { return luaL_error(L, "Bad number of args"); }
    assert(GetEntity(L)->Has<ModelComponent>());

    const auto& c = GetEntity(L)->Get<ModelComponent>();
    lua_pushstring(L, c.model.c_str());
    lua_pushnumber(L, c.scale);
    lua_pushstring(L, c.texture.c_str());
    lua_pushnumber(L, c.shineDamper);
    lua_pushnumber(L, c.reflectivity);
    return 5;
}

int SetModelComponent(lua_State* L)
{
    if (!CheckArgCount(L, 5)) { return luaL_error(L, "Bad number of args"); }

    auto& c = GetEntity(L)->Get<ModelComponent>();
    c.model = std::string(lua_tostring(L, 1));
    c.scale = (float)lua_tonumber(L, 2);
    c.texture = std::string(lua_tostring(L, 3));
    c.shineDamper = (float)lua_tonumber(L, 4);
    c.reflectivity = (float)lua_tonumber(L, 5);
    return 0;
}

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

int GetSelectComponent(lua_State* L)
{
    if (!CheckArgCount(L, 0)) { return luaL_error(L, "Bad number of args"); }
    assert(GetEntity(L)->Has<SelectComponent>());

    const auto& c = GetEntity(L)->Get<SelectComponent>();
    lua_pushboolean(L, c.selected);
    lua_pushboolean(L, c.hovered);
    return 2;
}

int SetSelectComponent(lua_State* L)
{
    if (!CheckArgCount(L, 2)) { return luaL_error(L, "Bad number of args"); }

    auto& c = GetEntity(L)->Get<SelectComponent>();
    c.selected = (bool)lua_toboolean(L, 1);
    c.hovered = (bool)lua_toboolean(L, 2);
    return 0;
}

int GetPathComponent(lua_State* L)
{
    if (!CheckArgCount(L, 0)) { return luaL_error(L, "Bad number of args"); }
    assert(GetEntity(L)->Has<PathComponent>());

    const auto& c = GetEntity(L)->Get<PathComponent>();
    lua_pushnumber(L, c.speed);
    return 1;
}

int SetPathComponent(lua_State* L)
{
    if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }

    auto& c = GetEntity(L)->Get<PathComponent>();
    c.speed = (float)lua_tonumber(L, 1);
    return 0;
}

int GetGridComponent(lua_State* L)
{
    if (!CheckArgCount(L, 0)) { return luaL_error(L, "Bad number of args"); }
    assert(GetEntity(L)->Has<GridComponent>());

    const auto& c = GetEntity(L)->Get<GridComponent>();
    lua_pushnumber(L, c.x);
    lua_pushnumber(L, c.z);
    return 2;
}

int SetGridComponent(lua_State* L)
{
    if (!CheckArgCount(L, 2)) { return luaL_error(L, "Bad number of args"); }

    auto& c = GetEntity(L)->Get<GridComponent>();
    c.x = (int)lua_tonumber(L, 1);
    c.z = (int)lua_tonumber(L, 2);
    return 0;
}

}
}
