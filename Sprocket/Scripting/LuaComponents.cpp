// GENERATED FILE @ 2020-08-28 23:10:37.950902

#include "LuaComponents.h"
#include "LuaGlobals.h"
#include "Entity.h"
#include "Maths.h"
#include "Components.h"

#include <lua.hpp>
#include <cassert>

namespace Sprocket {

void RegisterComponentFunctions(lua_State* L)
{
    lua_register(L, "Lua_GetNameComponent", &Lua::GetNameComponent);
    lua_register(L, "Lua_SetNameComponent", &Lua::SetNameComponent);
    lua_register(L, "Lua_AddNameComponent", &Lua::AddNameComponent);

    lua_register(L, "Lua_GetTransformComponent", &Lua::GetTransformComponent);
    lua_register(L, "Lua_SetTransformComponent", &Lua::SetTransformComponent);
    lua_register(L, "Lua_AddTransformComponent", &Lua::AddTransformComponent);

    lua_register(L, "Lua_GetModelComponent", &Lua::GetModelComponent);
    lua_register(L, "Lua_SetModelComponent", &Lua::SetModelComponent);
    lua_register(L, "Lua_AddModelComponent", &Lua::AddModelComponent);

    lua_register(L, "Lua_GetRigidBody3DComponent", &Lua::GetRigidBody3DComponent);
    lua_register(L, "Lua_SetRigidBody3DComponent", &Lua::SetRigidBody3DComponent);
    lua_register(L, "Lua_AddRigidBody3DComponent", &Lua::AddRigidBody3DComponent);

    lua_register(L, "Lua_GetBoxCollider3DComponent", &Lua::GetBoxCollider3DComponent);
    lua_register(L, "Lua_SetBoxCollider3DComponent", &Lua::SetBoxCollider3DComponent);
    lua_register(L, "Lua_AddBoxCollider3DComponent", &Lua::AddBoxCollider3DComponent);

    lua_register(L, "Lua_GetSphereCollider3DComponent", &Lua::GetSphereCollider3DComponent);
    lua_register(L, "Lua_SetSphereCollider3DComponent", &Lua::SetSphereCollider3DComponent);
    lua_register(L, "Lua_AddSphereCollider3DComponent", &Lua::AddSphereCollider3DComponent);

    lua_register(L, "Lua_GetCapsuleCollider3DComponent", &Lua::GetCapsuleCollider3DComponent);
    lua_register(L, "Lua_SetCapsuleCollider3DComponent", &Lua::SetCapsuleCollider3DComponent);
    lua_register(L, "Lua_AddCapsuleCollider3DComponent", &Lua::AddCapsuleCollider3DComponent);

    lua_register(L, "Lua_GetCameraComponent", &Lua::GetCameraComponent);
    lua_register(L, "Lua_SetCameraComponent", &Lua::SetCameraComponent);
    lua_register(L, "Lua_AddCameraComponent", &Lua::AddCameraComponent);

    lua_register(L, "Lua_GetSelectComponent", &Lua::GetSelectComponent);
    lua_register(L, "Lua_SetSelectComponent", &Lua::SetSelectComponent);
    lua_register(L, "Lua_AddSelectComponent", &Lua::AddSelectComponent);

    lua_register(L, "Lua_GetPathComponent", &Lua::GetPathComponent);
    lua_register(L, "Lua_SetPathComponent", &Lua::SetPathComponent);
    lua_register(L, "Lua_AddPathComponent", &Lua::AddPathComponent);

    lua_register(L, "Lua_GetGridComponent", &Lua::GetGridComponent);
    lua_register(L, "Lua_SetGridComponent", &Lua::SetGridComponent);
    lua_register(L, "Lua_AddGridComponent", &Lua::AddGridComponent);

    lua_register(L, "Lua_GetLightComponent", &Lua::GetLightComponent);
    lua_register(L, "Lua_SetLightComponent", &Lua::SetLightComponent);
    lua_register(L, "Lua_AddLightComponent", &Lua::AddLightComponent);

}

namespace Lua {

int GetNameComponent(lua_State* L)
{
    if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }
    Entity e = *static_cast<Entity*>(lua_touserdata(L, 1));
    assert(e.Has<NameComponent>());

    const auto& c = e.Get<NameComponent>();
    lua_pushstring(L, c.name.c_str());
    return 1;
}

int SetNameComponent(lua_State* L)
{
    if (!CheckArgCount(L, 2)) { return luaL_error(L, "Bad number of args"); }

    Entity e = *static_cast<Entity*>(lua_touserdata(L, 1));
    auto& c = e.Get<NameComponent>();
    c.name = std::string(lua_tostring(L, 2));
    return 0;
}

int AddNameComponent(lua_State* L)
{
    if (!CheckArgCount(L, 2)) { return luaL_error(L, "Bad number of args"); }

    Entity e = *static_cast<Entity*>(lua_touserdata(L, 1));
    assert(!e.Has<NameComponent>());

    NameComponent c;
    c.name = std::string(lua_tostring(L, 2));
    e.Add(c);
    return 0;
}

int GetTransformComponent(lua_State* L)
{
    if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }
    Entity e = *static_cast<Entity*>(lua_touserdata(L, 1));
    assert(e.Has<TransformComponent>());

    const auto& c = e.Get<TransformComponent>();
    lua_pushnumber(L, c.position.x);
    lua_pushnumber(L, c.position.y);
    lua_pushnumber(L, c.position.z);
    lua_pushnumber(L, c.scale.x);
    lua_pushnumber(L, c.scale.y);
    lua_pushnumber(L, c.scale.z);
    return 6;
}

int SetTransformComponent(lua_State* L)
{
    if (!CheckArgCount(L, 7)) { return luaL_error(L, "Bad number of args"); }

    Entity e = *static_cast<Entity*>(lua_touserdata(L, 1));
    auto& c = e.Get<TransformComponent>();
    c.position.x = (float)lua_tonumber(L, 2);
    c.position.y = (float)lua_tonumber(L, 3);
    c.position.z = (float)lua_tonumber(L, 4);
    c.scale.x = (float)lua_tonumber(L, 5);
    c.scale.y = (float)lua_tonumber(L, 6);
    c.scale.z = (float)lua_tonumber(L, 7);
    return 0;
}

int AddTransformComponent(lua_State* L)
{
    if (!CheckArgCount(L, 7)) { return luaL_error(L, "Bad number of args"); }

    Entity e = *static_cast<Entity*>(lua_touserdata(L, 1));
    assert(!e.Has<TransformComponent>());

    TransformComponent c;
    c.position.x = (float)lua_tonumber(L, 2);
    c.position.y = (float)lua_tonumber(L, 3);
    c.position.z = (float)lua_tonumber(L, 4);
    c.scale.x = (float)lua_tonumber(L, 5);
    c.scale.y = (float)lua_tonumber(L, 6);
    c.scale.z = (float)lua_tonumber(L, 7);
    e.Add(c);
    return 0;
}

int GetModelComponent(lua_State* L)
{
    if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }
    Entity e = *static_cast<Entity*>(lua_touserdata(L, 1));
    assert(e.Has<ModelComponent>());

    const auto& c = e.Get<ModelComponent>();
    lua_pushstring(L, c.model.c_str());
    lua_pushstring(L, c.texture.c_str());
    lua_pushnumber(L, c.shineDamper);
    lua_pushnumber(L, c.reflectivity);
    return 4;
}

int SetModelComponent(lua_State* L)
{
    if (!CheckArgCount(L, 5)) { return luaL_error(L, "Bad number of args"); }

    Entity e = *static_cast<Entity*>(lua_touserdata(L, 1));
    auto& c = e.Get<ModelComponent>();
    c.model = std::string(lua_tostring(L, 2));
    c.texture = std::string(lua_tostring(L, 3));
    c.shineDamper = (float)lua_tonumber(L, 4);
    c.reflectivity = (float)lua_tonumber(L, 5);
    return 0;
}

int AddModelComponent(lua_State* L)
{
    if (!CheckArgCount(L, 5)) { return luaL_error(L, "Bad number of args"); }

    Entity e = *static_cast<Entity*>(lua_touserdata(L, 1));
    assert(!e.Has<ModelComponent>());

    ModelComponent c;
    c.model = std::string(lua_tostring(L, 2));
    c.texture = std::string(lua_tostring(L, 3));
    c.shineDamper = (float)lua_tonumber(L, 4);
    c.reflectivity = (float)lua_tonumber(L, 5);
    e.Add(c);
    return 0;
}

int GetRigidBody3DComponent(lua_State* L)
{
    if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }
    Entity e = *static_cast<Entity*>(lua_touserdata(L, 1));
    assert(e.Has<RigidBody3DComponent>());

    const auto& c = e.Get<RigidBody3DComponent>();
    lua_pushnumber(L, c.velocity.x);
    lua_pushnumber(L, c.velocity.y);
    lua_pushnumber(L, c.velocity.z);
    lua_pushboolean(L, c.gravity);
    lua_pushboolean(L, c.frozen);
    lua_pushnumber(L, c.bounciness);
    lua_pushnumber(L, c.frictionCoefficient);
    lua_pushnumber(L, c.rollingResistance);
    lua_pushnumber(L, c.force.x);
    lua_pushnumber(L, c.force.y);
    lua_pushnumber(L, c.force.z);
    lua_pushboolean(L, c.onFloor);
    return 12;
}

int SetRigidBody3DComponent(lua_State* L)
{
    if (!CheckArgCount(L, 13)) { return luaL_error(L, "Bad number of args"); }

    Entity e = *static_cast<Entity*>(lua_touserdata(L, 1));
    auto& c = e.Get<RigidBody3DComponent>();
    c.velocity.x = (float)lua_tonumber(L, 2);
    c.velocity.y = (float)lua_tonumber(L, 3);
    c.velocity.z = (float)lua_tonumber(L, 4);
    c.gravity = (bool)lua_toboolean(L, 5);
    c.frozen = (bool)lua_toboolean(L, 6);
    c.bounciness = (float)lua_tonumber(L, 7);
    c.frictionCoefficient = (float)lua_tonumber(L, 8);
    c.rollingResistance = (float)lua_tonumber(L, 9);
    c.force.x = (float)lua_tonumber(L, 10);
    c.force.y = (float)lua_tonumber(L, 11);
    c.force.z = (float)lua_tonumber(L, 12);
    c.onFloor = (bool)lua_toboolean(L, 13);
    return 0;
}

int AddRigidBody3DComponent(lua_State* L)
{
    if (!CheckArgCount(L, 13)) { return luaL_error(L, "Bad number of args"); }

    Entity e = *static_cast<Entity*>(lua_touserdata(L, 1));
    assert(!e.Has<RigidBody3DComponent>());

    RigidBody3DComponent c;
    c.velocity.x = (float)lua_tonumber(L, 2);
    c.velocity.y = (float)lua_tonumber(L, 3);
    c.velocity.z = (float)lua_tonumber(L, 4);
    c.gravity = (bool)lua_toboolean(L, 5);
    c.frozen = (bool)lua_toboolean(L, 6);
    c.bounciness = (float)lua_tonumber(L, 7);
    c.frictionCoefficient = (float)lua_tonumber(L, 8);
    c.rollingResistance = (float)lua_tonumber(L, 9);
    c.force.x = (float)lua_tonumber(L, 10);
    c.force.y = (float)lua_tonumber(L, 11);
    c.force.z = (float)lua_tonumber(L, 12);
    c.onFloor = (bool)lua_toboolean(L, 13);
    e.Add(c);
    return 0;
}

int GetBoxCollider3DComponent(lua_State* L)
{
    if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }
    Entity e = *static_cast<Entity*>(lua_touserdata(L, 1));
    assert(e.Has<BoxCollider3DComponent>());

    const auto& c = e.Get<BoxCollider3DComponent>();
    lua_pushnumber(L, c.mass);
    return 1;
}

int SetBoxCollider3DComponent(lua_State* L)
{
    if (!CheckArgCount(L, 2)) { return luaL_error(L, "Bad number of args"); }

    Entity e = *static_cast<Entity*>(lua_touserdata(L, 1));
    auto& c = e.Get<BoxCollider3DComponent>();
    c.mass = (float)lua_tonumber(L, 2);
    return 0;
}

int AddBoxCollider3DComponent(lua_State* L)
{
    if (!CheckArgCount(L, 2)) { return luaL_error(L, "Bad number of args"); }

    Entity e = *static_cast<Entity*>(lua_touserdata(L, 1));
    assert(!e.Has<BoxCollider3DComponent>());

    BoxCollider3DComponent c;
    c.mass = (float)lua_tonumber(L, 2);
    e.Add(c);
    return 0;
}

int GetSphereCollider3DComponent(lua_State* L)
{
    if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }
    Entity e = *static_cast<Entity*>(lua_touserdata(L, 1));
    assert(e.Has<SphereCollider3DComponent>());

    const auto& c = e.Get<SphereCollider3DComponent>();
    lua_pushnumber(L, c.mass);
    return 1;
}

int SetSphereCollider3DComponent(lua_State* L)
{
    if (!CheckArgCount(L, 2)) { return luaL_error(L, "Bad number of args"); }

    Entity e = *static_cast<Entity*>(lua_touserdata(L, 1));
    auto& c = e.Get<SphereCollider3DComponent>();
    c.mass = (float)lua_tonumber(L, 2);
    return 0;
}

int AddSphereCollider3DComponent(lua_State* L)
{
    if (!CheckArgCount(L, 2)) { return luaL_error(L, "Bad number of args"); }

    Entity e = *static_cast<Entity*>(lua_touserdata(L, 1));
    assert(!e.Has<SphereCollider3DComponent>());

    SphereCollider3DComponent c;
    c.mass = (float)lua_tonumber(L, 2);
    e.Add(c);
    return 0;
}

int GetCapsuleCollider3DComponent(lua_State* L)
{
    if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }
    Entity e = *static_cast<Entity*>(lua_touserdata(L, 1));
    assert(e.Has<CapsuleCollider3DComponent>());

    const auto& c = e.Get<CapsuleCollider3DComponent>();
    lua_pushnumber(L, c.mass);
    return 1;
}

int SetCapsuleCollider3DComponent(lua_State* L)
{
    if (!CheckArgCount(L, 2)) { return luaL_error(L, "Bad number of args"); }

    Entity e = *static_cast<Entity*>(lua_touserdata(L, 1));
    auto& c = e.Get<CapsuleCollider3DComponent>();
    c.mass = (float)lua_tonumber(L, 2);
    return 0;
}

int AddCapsuleCollider3DComponent(lua_State* L)
{
    if (!CheckArgCount(L, 2)) { return luaL_error(L, "Bad number of args"); }

    Entity e = *static_cast<Entity*>(lua_touserdata(L, 1));
    assert(!e.Has<CapsuleCollider3DComponent>());

    CapsuleCollider3DComponent c;
    c.mass = (float)lua_tonumber(L, 2);
    e.Add(c);
    return 0;
}

int GetCameraComponent(lua_State* L)
{
    if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }
    Entity e = *static_cast<Entity*>(lua_touserdata(L, 1));
    assert(e.Has<CameraComponent>());

    const auto& c = e.Get<CameraComponent>();
    lua_pushnumber(L, c.fov);
    lua_pushnumber(L, c.pitch);
    return 2;
}

int SetCameraComponent(lua_State* L)
{
    if (!CheckArgCount(L, 3)) { return luaL_error(L, "Bad number of args"); }

    Entity e = *static_cast<Entity*>(lua_touserdata(L, 1));
    auto& c = e.Get<CameraComponent>();
    c.fov = (float)lua_tonumber(L, 2);
    c.pitch = (float)lua_tonumber(L, 3);
    return 0;
}

int AddCameraComponent(lua_State* L)
{
    if (!CheckArgCount(L, 3)) { return luaL_error(L, "Bad number of args"); }

    Entity e = *static_cast<Entity*>(lua_touserdata(L, 1));
    assert(!e.Has<CameraComponent>());

    CameraComponent c;
    c.fov = (float)lua_tonumber(L, 2);
    c.pitch = (float)lua_tonumber(L, 3);
    e.Add(c);
    return 0;
}

int GetSelectComponent(lua_State* L)
{
    if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }
    Entity e = *static_cast<Entity*>(lua_touserdata(L, 1));
    assert(e.Has<SelectComponent>());

    const auto& c = e.Get<SelectComponent>();
    lua_pushboolean(L, c.selected);
    lua_pushboolean(L, c.hovered);
    return 2;
}

int SetSelectComponent(lua_State* L)
{
    if (!CheckArgCount(L, 3)) { return luaL_error(L, "Bad number of args"); }

    Entity e = *static_cast<Entity*>(lua_touserdata(L, 1));
    auto& c = e.Get<SelectComponent>();
    c.selected = (bool)lua_toboolean(L, 2);
    c.hovered = (bool)lua_toboolean(L, 3);
    return 0;
}

int AddSelectComponent(lua_State* L)
{
    if (!CheckArgCount(L, 3)) { return luaL_error(L, "Bad number of args"); }

    Entity e = *static_cast<Entity*>(lua_touserdata(L, 1));
    assert(!e.Has<SelectComponent>());

    SelectComponent c;
    c.selected = (bool)lua_toboolean(L, 2);
    c.hovered = (bool)lua_toboolean(L, 3);
    e.Add(c);
    return 0;
}

int GetPathComponent(lua_State* L)
{
    if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }
    Entity e = *static_cast<Entity*>(lua_touserdata(L, 1));
    assert(e.Has<PathComponent>());

    const auto& c = e.Get<PathComponent>();
    lua_pushnumber(L, c.speed);
    return 1;
}

int SetPathComponent(lua_State* L)
{
    if (!CheckArgCount(L, 2)) { return luaL_error(L, "Bad number of args"); }

    Entity e = *static_cast<Entity*>(lua_touserdata(L, 1));
    auto& c = e.Get<PathComponent>();
    c.speed = (float)lua_tonumber(L, 2);
    return 0;
}

int AddPathComponent(lua_State* L)
{
    if (!CheckArgCount(L, 2)) { return luaL_error(L, "Bad number of args"); }

    Entity e = *static_cast<Entity*>(lua_touserdata(L, 1));
    assert(!e.Has<PathComponent>());

    PathComponent c;
    c.speed = (float)lua_tonumber(L, 2);
    e.Add(c);
    return 0;
}

int GetGridComponent(lua_State* L)
{
    if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }
    Entity e = *static_cast<Entity*>(lua_touserdata(L, 1));
    assert(e.Has<GridComponent>());

    const auto& c = e.Get<GridComponent>();
    lua_pushnumber(L, c.x);
    lua_pushnumber(L, c.z);
    return 2;
}

int SetGridComponent(lua_State* L)
{
    if (!CheckArgCount(L, 3)) { return luaL_error(L, "Bad number of args"); }

    Entity e = *static_cast<Entity*>(lua_touserdata(L, 1));
    auto& c = e.Get<GridComponent>();
    c.x = (int)lua_tonumber(L, 2);
    c.z = (int)lua_tonumber(L, 3);
    return 0;
}

int AddGridComponent(lua_State* L)
{
    if (!CheckArgCount(L, 3)) { return luaL_error(L, "Bad number of args"); }

    Entity e = *static_cast<Entity*>(lua_touserdata(L, 1));
    assert(!e.Has<GridComponent>());

    GridComponent c;
    c.x = (int)lua_tonumber(L, 2);
    c.z = (int)lua_tonumber(L, 3);
    e.Add(c);
    return 0;
}

int GetLightComponent(lua_State* L)
{
    if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }
    Entity e = *static_cast<Entity*>(lua_touserdata(L, 1));
    assert(e.Has<LightComponent>());

    const auto& c = e.Get<LightComponent>();
    lua_pushnumber(L, c.colour.x);
    lua_pushnumber(L, c.colour.y);
    lua_pushnumber(L, c.colour.z);
    lua_pushnumber(L, c.attenuation.x);
    lua_pushnumber(L, c.attenuation.y);
    lua_pushnumber(L, c.attenuation.z);
    lua_pushnumber(L, c.brightness);
    return 7;
}

int SetLightComponent(lua_State* L)
{
    if (!CheckArgCount(L, 8)) { return luaL_error(L, "Bad number of args"); }

    Entity e = *static_cast<Entity*>(lua_touserdata(L, 1));
    auto& c = e.Get<LightComponent>();
    c.colour.x = (float)lua_tonumber(L, 2);
    c.colour.y = (float)lua_tonumber(L, 3);
    c.colour.z = (float)lua_tonumber(L, 4);
    c.attenuation.x = (float)lua_tonumber(L, 5);
    c.attenuation.y = (float)lua_tonumber(L, 6);
    c.attenuation.z = (float)lua_tonumber(L, 7);
    c.brightness = (float)lua_tonumber(L, 8);
    return 0;
}

int AddLightComponent(lua_State* L)
{
    if (!CheckArgCount(L, 8)) { return luaL_error(L, "Bad number of args"); }

    Entity e = *static_cast<Entity*>(lua_touserdata(L, 1));
    assert(!e.Has<LightComponent>());

    LightComponent c;
    c.colour.x = (float)lua_tonumber(L, 2);
    c.colour.y = (float)lua_tonumber(L, 3);
    c.colour.z = (float)lua_tonumber(L, 4);
    c.attenuation.x = (float)lua_tonumber(L, 5);
    c.attenuation.y = (float)lua_tonumber(L, 6);
    c.attenuation.z = (float)lua_tonumber(L, 7);
    c.brightness = (float)lua_tonumber(L, 8);
    e.Add(c);
    return 0;
}

}
}
