// GENERATED FILE
#include "LuaComponents.h"
#include "LuaGlobals.h"
#include "Entity.h"
#include "Maths.h"
#include "Components.h"
#include "Log.h"

#include <lua.hpp>
#include <cassert>

namespace Sprocket {
namespace {

// PUSH
int Push(lua_State* L, int value)
{
    lua_pushnumber(L, value);
    return 1;
}

int Push(lua_State* L, float value)
{
    lua_pushnumber(L, value);
    return 1;
}

int Push(lua_State* L, const std::string& value)
{
    lua_pushstring(L, value.c_str());
    return 1;
}

int Push(lua_State* L, const bool& value)
{
    lua_pushboolean(L, value);
    return 1;
}

int Push(lua_State* L, const Maths::vec3& value)
{
    lua_pushnumber(L, value.x);
    lua_pushnumber(L, value.y);
    lua_pushnumber(L, value.z);
    return 3;
}

// PULL
template <typename T> T Pull(lua_State* L, int& count)
{
    static_assert(sizeof(T) == -1);
    return T();
}

template <> int Pull(lua_State* L, int& count)
{
    return (int)lua_tonumber(L, count++);
}

template <> float Pull(lua_State* L, int& count)
{
    return (float)lua_tonumber(L, count++);
}

template <> std::string Pull(lua_State* L, int& count)
{
    return std::string(lua_tostring(L, count++));
}

template <> bool Pull(lua_State* L, int& count)
{
    return (bool)lua_toboolean(L, count++);
}

template <> Maths::vec3 Pull(lua_State* L, int& count)
{
    float x = (float)lua_tonumber(L, count++);
    float y = (float)lua_tonumber(L, count++);
    float z = (float)lua_tonumber(L, count++);
    return {x, y, z};
}

template <> Maths::quat Pull(lua_State* L, int& count)
{
    float x = (float)lua_tonumber(L, count++);
    float y = (float)lua_tonumber(L, count++);
    float z = (float)lua_tonumber(L, count++);
    float w = (float)lua_tonumber(L, count++);
    return {x, y, z, w};
}

// DIMENSION
template <typename T> constexpr int Dimension()
{
    static_assert(sizeof(T) == -1);
    return 0;
}

template <> constexpr int Dimension<int>() { return 1; }
template <> constexpr int Dimension<float>() { return 1; }
template <> constexpr int Dimension<bool>() { return 1; }
template <> constexpr int Dimension<std::string>() { return 1; }
template <> constexpr int Dimension<Maths::vec3>() { return 3; }
template <> constexpr int Dimension<Maths::quat>() { return 4; }

constexpr int TemporaryComponentDimension()
{
    int count = 0;
    return count;
}

constexpr int NameComponentDimension()
{
    int count = 0;
    count += Dimension<std::string>(); // name
    return count;
}

constexpr int TransformComponentDimension()
{
    int count = 0;
    count += Dimension<Maths::vec3>(); // position
    count += Dimension<Maths::vec3>(); // scale
    return count;
}

constexpr int ModelComponentDimension()
{
    int count = 0;
    count += Dimension<std::string>(); // model
    count += Dimension<std::string>(); // texture
    count += Dimension<float>(); // shineDamper
    count += Dimension<float>(); // reflectivity
    return count;
}

constexpr int RigidBody3DComponentDimension()
{
    int count = 0;
    count += Dimension<Maths::vec3>(); // velocity
    count += Dimension<bool>(); // gravity
    count += Dimension<bool>(); // frozen
    count += Dimension<float>(); // bounciness
    count += Dimension<float>(); // frictionCoefficient
    count += Dimension<float>(); // rollingResistance
    count += Dimension<Maths::vec3>(); // force
    count += Dimension<bool>(); // onFloor
    return count;
}

constexpr int BoxCollider3DComponentDimension()
{
    int count = 0;
    count += Dimension<Maths::vec3>(); // position
    count += Dimension<float>(); // mass
    count += Dimension<Maths::vec3>(); // halfExtents
    count += Dimension<bool>(); // applyScale
    return count;
}

constexpr int SphereCollider3DComponentDimension()
{
    int count = 0;
    count += Dimension<Maths::vec3>(); // position
    count += Dimension<float>(); // mass
    count += Dimension<float>(); // radius
    return count;
}

constexpr int CapsuleCollider3DComponentDimension()
{
    int count = 0;
    count += Dimension<Maths::vec3>(); // position
    count += Dimension<float>(); // mass
    count += Dimension<float>(); // radius
    count += Dimension<float>(); // height
    return count;
}

constexpr int ScriptComponentDimension()
{
    int count = 0;
    count += Dimension<std::string>(); // script
    count += Dimension<bool>(); // active
    return count;
}

constexpr int CameraComponentDimension()
{
    int count = 0;
    count += Dimension<float>(); // fov
    count += Dimension<float>(); // pitch
    return count;
}

constexpr int SelectComponentDimension()
{
    int count = 0;
    count += Dimension<bool>(); // selected
    count += Dimension<bool>(); // hovered
    return count;
}

constexpr int PathComponentDimension()
{
    int count = 0;
    count += Dimension<float>(); // speed
    return count;
}

constexpr int GridComponentDimension()
{
    int count = 0;
    count += Dimension<int>(); // x
    count += Dimension<int>(); // z
    return count;
}

constexpr int LightComponentDimension()
{
    int count = 0;
    count += Dimension<Maths::vec3>(); // colour
    count += Dimension<Maths::vec3>(); // attenuation
    count += Dimension<float>(); // brightness
    return count;
}

constexpr int ParticleComponentDimension()
{
    int count = 0;
    count += Dimension<float>(); // interval
    count += Dimension<Maths::vec3>(); // velocity
    count += Dimension<float>(); // velocityNoise
    count += Dimension<Maths::vec3>(); // acceleration
    count += Dimension<Maths::vec3>(); // scale
    count += Dimension<float>(); // life
    return count;
}


template<typename T> int Lua_Has(lua_State* L)
{
    if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }

    Entity entity = *static_cast<Entity*>(lua_touserdata(L, 1));
    lua_pushboolean(L, entity.Has<T>());
    return 1;
}

}

void RegisterComponentFunctions(lua_State* L)
{
    lua_register(L, "Lua_GetTemporaryComponent", &Lua::GetTemporaryComponent);
    lua_register(L, "Lua_SetTemporaryComponent", &Lua::SetTemporaryComponent);
    lua_register(L, "Lua_AddTemporaryComponent", &Lua::AddTemporaryComponent);
    lua_register(L, "HasTemporaryComponent", &Lua_Has<TemporaryComponent>);

    lua_register(L, "Lua_GetNameComponent", &Lua::GetNameComponent);
    lua_register(L, "Lua_SetNameComponent", &Lua::SetNameComponent);
    lua_register(L, "Lua_AddNameComponent", &Lua::AddNameComponent);
    lua_register(L, "HasNameComponent", &Lua_Has<NameComponent>);

    lua_register(L, "Lua_GetTransformComponent", &Lua::GetTransformComponent);
    lua_register(L, "Lua_SetTransformComponent", &Lua::SetTransformComponent);
    lua_register(L, "Lua_AddTransformComponent", &Lua::AddTransformComponent);
    lua_register(L, "HasTransformComponent", &Lua_Has<TransformComponent>);

    lua_register(L, "Lua_GetModelComponent", &Lua::GetModelComponent);
    lua_register(L, "Lua_SetModelComponent", &Lua::SetModelComponent);
    lua_register(L, "Lua_AddModelComponent", &Lua::AddModelComponent);
    lua_register(L, "HasModelComponent", &Lua_Has<ModelComponent>);

    lua_register(L, "Lua_GetRigidBody3DComponent", &Lua::GetRigidBody3DComponent);
    lua_register(L, "Lua_SetRigidBody3DComponent", &Lua::SetRigidBody3DComponent);
    lua_register(L, "Lua_AddRigidBody3DComponent", &Lua::AddRigidBody3DComponent);
    lua_register(L, "HasRigidBody3DComponent", &Lua_Has<RigidBody3DComponent>);

    lua_register(L, "Lua_GetBoxCollider3DComponent", &Lua::GetBoxCollider3DComponent);
    lua_register(L, "Lua_SetBoxCollider3DComponent", &Lua::SetBoxCollider3DComponent);
    lua_register(L, "Lua_AddBoxCollider3DComponent", &Lua::AddBoxCollider3DComponent);
    lua_register(L, "HasBoxCollider3DComponent", &Lua_Has<BoxCollider3DComponent>);

    lua_register(L, "Lua_GetSphereCollider3DComponent", &Lua::GetSphereCollider3DComponent);
    lua_register(L, "Lua_SetSphereCollider3DComponent", &Lua::SetSphereCollider3DComponent);
    lua_register(L, "Lua_AddSphereCollider3DComponent", &Lua::AddSphereCollider3DComponent);
    lua_register(L, "HasSphereCollider3DComponent", &Lua_Has<SphereCollider3DComponent>);

    lua_register(L, "Lua_GetCapsuleCollider3DComponent", &Lua::GetCapsuleCollider3DComponent);
    lua_register(L, "Lua_SetCapsuleCollider3DComponent", &Lua::SetCapsuleCollider3DComponent);
    lua_register(L, "Lua_AddCapsuleCollider3DComponent", &Lua::AddCapsuleCollider3DComponent);
    lua_register(L, "HasCapsuleCollider3DComponent", &Lua_Has<CapsuleCollider3DComponent>);

    lua_register(L, "Lua_GetScriptComponent", &Lua::GetScriptComponent);
    lua_register(L, "Lua_SetScriptComponent", &Lua::SetScriptComponent);
    lua_register(L, "Lua_AddScriptComponent", &Lua::AddScriptComponent);
    lua_register(L, "HasScriptComponent", &Lua_Has<ScriptComponent>);

    lua_register(L, "Lua_GetCameraComponent", &Lua::GetCameraComponent);
    lua_register(L, "Lua_SetCameraComponent", &Lua::SetCameraComponent);
    lua_register(L, "Lua_AddCameraComponent", &Lua::AddCameraComponent);
    lua_register(L, "HasCameraComponent", &Lua_Has<CameraComponent>);

    lua_register(L, "Lua_GetSelectComponent", &Lua::GetSelectComponent);
    lua_register(L, "Lua_SetSelectComponent", &Lua::SetSelectComponent);
    lua_register(L, "Lua_AddSelectComponent", &Lua::AddSelectComponent);
    lua_register(L, "HasSelectComponent", &Lua_Has<SelectComponent>);

    lua_register(L, "Lua_GetPathComponent", &Lua::GetPathComponent);
    lua_register(L, "Lua_SetPathComponent", &Lua::SetPathComponent);
    lua_register(L, "Lua_AddPathComponent", &Lua::AddPathComponent);
    lua_register(L, "HasPathComponent", &Lua_Has<PathComponent>);

    lua_register(L, "Lua_GetGridComponent", &Lua::GetGridComponent);
    lua_register(L, "Lua_SetGridComponent", &Lua::SetGridComponent);
    lua_register(L, "Lua_AddGridComponent", &Lua::AddGridComponent);
    lua_register(L, "HasGridComponent", &Lua_Has<GridComponent>);

    lua_register(L, "Lua_GetLightComponent", &Lua::GetLightComponent);
    lua_register(L, "Lua_SetLightComponent", &Lua::SetLightComponent);
    lua_register(L, "Lua_AddLightComponent", &Lua::AddLightComponent);
    lua_register(L, "HasLightComponent", &Lua_Has<LightComponent>);

    lua_register(L, "Lua_GetParticleComponent", &Lua::GetParticleComponent);
    lua_register(L, "Lua_SetParticleComponent", &Lua::SetParticleComponent);
    lua_register(L, "Lua_AddParticleComponent", &Lua::AddParticleComponent);
    lua_register(L, "HasParticleComponent", &Lua_Has<ParticleComponent>);

}

namespace Lua {

int GetTemporaryComponent(lua_State* L)
{
    if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }

    Entity e = *static_cast<Entity*>(lua_touserdata(L, 1));
    assert(e.Has<TemporaryComponent>());

    int count = 0;
    const auto& c = e.Get<TemporaryComponent>();
    return count;
}

int SetTemporaryComponent(lua_State* L)
{
    if (!CheckArgCount(L, TemporaryComponentDimension() + 1)) { return luaL_error(L, "Bad number of args"); }

    int count = 2;
    Entity e = *static_cast<Entity*>(lua_touserdata(L, 1));
    auto& c = e.Get<TemporaryComponent>();
    return 0;
}

int AddTemporaryComponent(lua_State* L)
{
    if (!CheckArgCount(L, TemporaryComponentDimension() + 1)) { return luaL_error(L, "Bad number of args"); }

    int count = 2;
    Entity e = *static_cast<Entity*>(lua_touserdata(L, 1));
    assert(!e.Has<TemporaryComponent>());

    TemporaryComponent c;
    e.Add(c);
    return 0;
}

int GetNameComponent(lua_State* L)
{
    if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }

    Entity e = *static_cast<Entity*>(lua_touserdata(L, 1));
    assert(e.Has<NameComponent>());

    int count = 0;
    const auto& c = e.Get<NameComponent>();
    count += Push(L, c.name);
    return count;
}

int SetNameComponent(lua_State* L)
{
    if (!CheckArgCount(L, NameComponentDimension() + 1)) { return luaL_error(L, "Bad number of args"); }

    int count = 2;
    Entity e = *static_cast<Entity*>(lua_touserdata(L, 1));
    auto& c = e.Get<NameComponent>();
    c.name = Pull<std::string>(L, count);
    return 0;
}

int AddNameComponent(lua_State* L)
{
    if (!CheckArgCount(L, NameComponentDimension() + 1)) { return luaL_error(L, "Bad number of args"); }

    int count = 2;
    Entity e = *static_cast<Entity*>(lua_touserdata(L, 1));
    assert(!e.Has<NameComponent>());

    NameComponent c;
    c.name = Pull<std::string>(L, count);
    e.Add(c);
    return 0;
}

int GetTransformComponent(lua_State* L)
{
    if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }

    Entity e = *static_cast<Entity*>(lua_touserdata(L, 1));
    assert(e.Has<TransformComponent>());

    int count = 0;
    const auto& c = e.Get<TransformComponent>();
    count += Push(L, c.position);
    count += Push(L, c.scale);
    return count;
}

int SetTransformComponent(lua_State* L)
{
    if (!CheckArgCount(L, TransformComponentDimension() + 1)) { return luaL_error(L, "Bad number of args"); }

    int count = 2;
    Entity e = *static_cast<Entity*>(lua_touserdata(L, 1));
    auto& c = e.Get<TransformComponent>();
    c.position = Pull<Maths::vec3>(L, count);
    c.scale = Pull<Maths::vec3>(L, count);
    return 0;
}

int AddTransformComponent(lua_State* L)
{
    if (!CheckArgCount(L, TransformComponentDimension() + 1)) { return luaL_error(L, "Bad number of args"); }

    int count = 2;
    Entity e = *static_cast<Entity*>(lua_touserdata(L, 1));
    assert(!e.Has<TransformComponent>());

    TransformComponent c;
    c.position = Pull<Maths::vec3>(L, count);
    c.scale = Pull<Maths::vec3>(L, count);
    e.Add(c);
    return 0;
}

int GetModelComponent(lua_State* L)
{
    if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }

    Entity e = *static_cast<Entity*>(lua_touserdata(L, 1));
    assert(e.Has<ModelComponent>());

    int count = 0;
    const auto& c = e.Get<ModelComponent>();
    count += Push(L, c.model);
    count += Push(L, c.texture);
    count += Push(L, c.shineDamper);
    count += Push(L, c.reflectivity);
    return count;
}

int SetModelComponent(lua_State* L)
{
    if (!CheckArgCount(L, ModelComponentDimension() + 1)) { return luaL_error(L, "Bad number of args"); }

    int count = 2;
    Entity e = *static_cast<Entity*>(lua_touserdata(L, 1));
    auto& c = e.Get<ModelComponent>();
    c.model = Pull<std::string>(L, count);
    c.texture = Pull<std::string>(L, count);
    c.shineDamper = Pull<float>(L, count);
    c.reflectivity = Pull<float>(L, count);
    return 0;
}

int AddModelComponent(lua_State* L)
{
    if (!CheckArgCount(L, ModelComponentDimension() + 1)) { return luaL_error(L, "Bad number of args"); }

    int count = 2;
    Entity e = *static_cast<Entity*>(lua_touserdata(L, 1));
    assert(!e.Has<ModelComponent>());

    ModelComponent c;
    c.model = Pull<std::string>(L, count);
    c.texture = Pull<std::string>(L, count);
    c.shineDamper = Pull<float>(L, count);
    c.reflectivity = Pull<float>(L, count);
    e.Add(c);
    return 0;
}

int GetRigidBody3DComponent(lua_State* L)
{
    if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }

    Entity e = *static_cast<Entity*>(lua_touserdata(L, 1));
    assert(e.Has<RigidBody3DComponent>());

    int count = 0;
    const auto& c = e.Get<RigidBody3DComponent>();
    count += Push(L, c.velocity);
    count += Push(L, c.gravity);
    count += Push(L, c.frozen);
    count += Push(L, c.bounciness);
    count += Push(L, c.frictionCoefficient);
    count += Push(L, c.rollingResistance);
    count += Push(L, c.force);
    count += Push(L, c.onFloor);
    return count;
}

int SetRigidBody3DComponent(lua_State* L)
{
    if (!CheckArgCount(L, RigidBody3DComponentDimension() + 1)) { return luaL_error(L, "Bad number of args"); }

    int count = 2;
    Entity e = *static_cast<Entity*>(lua_touserdata(L, 1));
    auto& c = e.Get<RigidBody3DComponent>();
    c.velocity = Pull<Maths::vec3>(L, count);
    c.gravity = Pull<bool>(L, count);
    c.frozen = Pull<bool>(L, count);
    c.bounciness = Pull<float>(L, count);
    c.frictionCoefficient = Pull<float>(L, count);
    c.rollingResistance = Pull<float>(L, count);
    c.force = Pull<Maths::vec3>(L, count);
    c.onFloor = Pull<bool>(L, count);
    return 0;
}

int AddRigidBody3DComponent(lua_State* L)
{
    if (!CheckArgCount(L, RigidBody3DComponentDimension() + 1)) { return luaL_error(L, "Bad number of args"); }

    int count = 2;
    Entity e = *static_cast<Entity*>(lua_touserdata(L, 1));
    assert(!e.Has<RigidBody3DComponent>());

    RigidBody3DComponent c;
    c.velocity = Pull<Maths::vec3>(L, count);
    c.gravity = Pull<bool>(L, count);
    c.frozen = Pull<bool>(L, count);
    c.bounciness = Pull<float>(L, count);
    c.frictionCoefficient = Pull<float>(L, count);
    c.rollingResistance = Pull<float>(L, count);
    c.force = Pull<Maths::vec3>(L, count);
    c.onFloor = Pull<bool>(L, count);
    e.Add(c);
    return 0;
}

int GetBoxCollider3DComponent(lua_State* L)
{
    if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }

    Entity e = *static_cast<Entity*>(lua_touserdata(L, 1));
    assert(e.Has<BoxCollider3DComponent>());

    int count = 0;
    const auto& c = e.Get<BoxCollider3DComponent>();
    count += Push(L, c.position);
    count += Push(L, c.mass);
    count += Push(L, c.halfExtents);
    count += Push(L, c.applyScale);
    return count;
}

int SetBoxCollider3DComponent(lua_State* L)
{
    if (!CheckArgCount(L, BoxCollider3DComponentDimension() + 1)) { return luaL_error(L, "Bad number of args"); }

    int count = 2;
    Entity e = *static_cast<Entity*>(lua_touserdata(L, 1));
    auto& c = e.Get<BoxCollider3DComponent>();
    c.position = Pull<Maths::vec3>(L, count);
    c.mass = Pull<float>(L, count);
    c.halfExtents = Pull<Maths::vec3>(L, count);
    c.applyScale = Pull<bool>(L, count);
    return 0;
}

int AddBoxCollider3DComponent(lua_State* L)
{
    if (!CheckArgCount(L, BoxCollider3DComponentDimension() + 1)) { return luaL_error(L, "Bad number of args"); }

    int count = 2;
    Entity e = *static_cast<Entity*>(lua_touserdata(L, 1));
    assert(!e.Has<BoxCollider3DComponent>());

    BoxCollider3DComponent c;
    c.position = Pull<Maths::vec3>(L, count);
    c.mass = Pull<float>(L, count);
    c.halfExtents = Pull<Maths::vec3>(L, count);
    c.applyScale = Pull<bool>(L, count);
    e.Add(c);
    return 0;
}

int GetSphereCollider3DComponent(lua_State* L)
{
    if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }

    Entity e = *static_cast<Entity*>(lua_touserdata(L, 1));
    assert(e.Has<SphereCollider3DComponent>());

    int count = 0;
    const auto& c = e.Get<SphereCollider3DComponent>();
    count += Push(L, c.position);
    count += Push(L, c.mass);
    count += Push(L, c.radius);
    return count;
}

int SetSphereCollider3DComponent(lua_State* L)
{
    if (!CheckArgCount(L, SphereCollider3DComponentDimension() + 1)) { return luaL_error(L, "Bad number of args"); }

    int count = 2;
    Entity e = *static_cast<Entity*>(lua_touserdata(L, 1));
    auto& c = e.Get<SphereCollider3DComponent>();
    c.position = Pull<Maths::vec3>(L, count);
    c.mass = Pull<float>(L, count);
    c.radius = Pull<float>(L, count);
    return 0;
}

int AddSphereCollider3DComponent(lua_State* L)
{
    if (!CheckArgCount(L, SphereCollider3DComponentDimension() + 1)) { return luaL_error(L, "Bad number of args"); }

    int count = 2;
    Entity e = *static_cast<Entity*>(lua_touserdata(L, 1));
    assert(!e.Has<SphereCollider3DComponent>());

    SphereCollider3DComponent c;
    c.position = Pull<Maths::vec3>(L, count);
    c.mass = Pull<float>(L, count);
    c.radius = Pull<float>(L, count);
    e.Add(c);
    return 0;
}

int GetCapsuleCollider3DComponent(lua_State* L)
{
    if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }

    Entity e = *static_cast<Entity*>(lua_touserdata(L, 1));
    assert(e.Has<CapsuleCollider3DComponent>());

    int count = 0;
    const auto& c = e.Get<CapsuleCollider3DComponent>();
    count += Push(L, c.position);
    count += Push(L, c.mass);
    count += Push(L, c.radius);
    count += Push(L, c.height);
    return count;
}

int SetCapsuleCollider3DComponent(lua_State* L)
{
    if (!CheckArgCount(L, CapsuleCollider3DComponentDimension() + 1)) { return luaL_error(L, "Bad number of args"); }

    int count = 2;
    Entity e = *static_cast<Entity*>(lua_touserdata(L, 1));
    auto& c = e.Get<CapsuleCollider3DComponent>();
    c.position = Pull<Maths::vec3>(L, count);
    c.mass = Pull<float>(L, count);
    c.radius = Pull<float>(L, count);
    c.height = Pull<float>(L, count);
    return 0;
}

int AddCapsuleCollider3DComponent(lua_State* L)
{
    if (!CheckArgCount(L, CapsuleCollider3DComponentDimension() + 1)) { return luaL_error(L, "Bad number of args"); }

    int count = 2;
    Entity e = *static_cast<Entity*>(lua_touserdata(L, 1));
    assert(!e.Has<CapsuleCollider3DComponent>());

    CapsuleCollider3DComponent c;
    c.position = Pull<Maths::vec3>(L, count);
    c.mass = Pull<float>(L, count);
    c.radius = Pull<float>(L, count);
    c.height = Pull<float>(L, count);
    e.Add(c);
    return 0;
}

int GetScriptComponent(lua_State* L)
{
    if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }

    Entity e = *static_cast<Entity*>(lua_touserdata(L, 1));
    assert(e.Has<ScriptComponent>());

    int count = 0;
    const auto& c = e.Get<ScriptComponent>();
    count += Push(L, c.script);
    count += Push(L, c.active);
    return count;
}

int SetScriptComponent(lua_State* L)
{
    if (!CheckArgCount(L, ScriptComponentDimension() + 1)) { return luaL_error(L, "Bad number of args"); }

    int count = 2;
    Entity e = *static_cast<Entity*>(lua_touserdata(L, 1));
    auto& c = e.Get<ScriptComponent>();
    c.script = Pull<std::string>(L, count);
    c.active = Pull<bool>(L, count);
    return 0;
}

int AddScriptComponent(lua_State* L)
{
    if (!CheckArgCount(L, ScriptComponentDimension() + 1)) { return luaL_error(L, "Bad number of args"); }

    int count = 2;
    Entity e = *static_cast<Entity*>(lua_touserdata(L, 1));
    assert(!e.Has<ScriptComponent>());

    ScriptComponent c;
    c.script = Pull<std::string>(L, count);
    c.active = Pull<bool>(L, count);
    e.Add(c);
    return 0;
}

int GetCameraComponent(lua_State* L)
{
    if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }

    Entity e = *static_cast<Entity*>(lua_touserdata(L, 1));
    assert(e.Has<CameraComponent>());

    int count = 0;
    const auto& c = e.Get<CameraComponent>();
    count += Push(L, c.fov);
    count += Push(L, c.pitch);
    return count;
}

int SetCameraComponent(lua_State* L)
{
    if (!CheckArgCount(L, CameraComponentDimension() + 1)) { return luaL_error(L, "Bad number of args"); }

    int count = 2;
    Entity e = *static_cast<Entity*>(lua_touserdata(L, 1));
    auto& c = e.Get<CameraComponent>();
    c.fov = Pull<float>(L, count);
    c.pitch = Pull<float>(L, count);
    return 0;
}

int AddCameraComponent(lua_State* L)
{
    if (!CheckArgCount(L, CameraComponentDimension() + 1)) { return luaL_error(L, "Bad number of args"); }

    int count = 2;
    Entity e = *static_cast<Entity*>(lua_touserdata(L, 1));
    assert(!e.Has<CameraComponent>());

    CameraComponent c;
    c.fov = Pull<float>(L, count);
    c.pitch = Pull<float>(L, count);
    e.Add(c);
    return 0;
}

int GetSelectComponent(lua_State* L)
{
    if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }

    Entity e = *static_cast<Entity*>(lua_touserdata(L, 1));
    assert(e.Has<SelectComponent>());

    int count = 0;
    const auto& c = e.Get<SelectComponent>();
    count += Push(L, c.selected);
    count += Push(L, c.hovered);
    return count;
}

int SetSelectComponent(lua_State* L)
{
    if (!CheckArgCount(L, SelectComponentDimension() + 1)) { return luaL_error(L, "Bad number of args"); }

    int count = 2;
    Entity e = *static_cast<Entity*>(lua_touserdata(L, 1));
    auto& c = e.Get<SelectComponent>();
    c.selected = Pull<bool>(L, count);
    c.hovered = Pull<bool>(L, count);
    return 0;
}

int AddSelectComponent(lua_State* L)
{
    if (!CheckArgCount(L, SelectComponentDimension() + 1)) { return luaL_error(L, "Bad number of args"); }

    int count = 2;
    Entity e = *static_cast<Entity*>(lua_touserdata(L, 1));
    assert(!e.Has<SelectComponent>());

    SelectComponent c;
    c.selected = Pull<bool>(L, count);
    c.hovered = Pull<bool>(L, count);
    e.Add(c);
    return 0;
}

int GetPathComponent(lua_State* L)
{
    if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }

    Entity e = *static_cast<Entity*>(lua_touserdata(L, 1));
    assert(e.Has<PathComponent>());

    int count = 0;
    const auto& c = e.Get<PathComponent>();
    count += Push(L, c.speed);
    return count;
}

int SetPathComponent(lua_State* L)
{
    if (!CheckArgCount(L, PathComponentDimension() + 1)) { return luaL_error(L, "Bad number of args"); }

    int count = 2;
    Entity e = *static_cast<Entity*>(lua_touserdata(L, 1));
    auto& c = e.Get<PathComponent>();
    c.speed = Pull<float>(L, count);
    return 0;
}

int AddPathComponent(lua_State* L)
{
    if (!CheckArgCount(L, PathComponentDimension() + 1)) { return luaL_error(L, "Bad number of args"); }

    int count = 2;
    Entity e = *static_cast<Entity*>(lua_touserdata(L, 1));
    assert(!e.Has<PathComponent>());

    PathComponent c;
    c.speed = Pull<float>(L, count);
    e.Add(c);
    return 0;
}

int GetGridComponent(lua_State* L)
{
    if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }

    Entity e = *static_cast<Entity*>(lua_touserdata(L, 1));
    assert(e.Has<GridComponent>());

    int count = 0;
    const auto& c = e.Get<GridComponent>();
    count += Push(L, c.x);
    count += Push(L, c.z);
    return count;
}

int SetGridComponent(lua_State* L)
{
    if (!CheckArgCount(L, GridComponentDimension() + 1)) { return luaL_error(L, "Bad number of args"); }

    int count = 2;
    Entity e = *static_cast<Entity*>(lua_touserdata(L, 1));
    auto& c = e.Get<GridComponent>();
    c.x = Pull<int>(L, count);
    c.z = Pull<int>(L, count);
    return 0;
}

int AddGridComponent(lua_State* L)
{
    if (!CheckArgCount(L, GridComponentDimension() + 1)) { return luaL_error(L, "Bad number of args"); }

    int count = 2;
    Entity e = *static_cast<Entity*>(lua_touserdata(L, 1));
    assert(!e.Has<GridComponent>());

    GridComponent c;
    c.x = Pull<int>(L, count);
    c.z = Pull<int>(L, count);
    e.Add(c);
    return 0;
}

int GetLightComponent(lua_State* L)
{
    if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }

    Entity e = *static_cast<Entity*>(lua_touserdata(L, 1));
    assert(e.Has<LightComponent>());

    int count = 0;
    const auto& c = e.Get<LightComponent>();
    count += Push(L, c.colour);
    count += Push(L, c.attenuation);
    count += Push(L, c.brightness);
    return count;
}

int SetLightComponent(lua_State* L)
{
    if (!CheckArgCount(L, LightComponentDimension() + 1)) { return luaL_error(L, "Bad number of args"); }

    int count = 2;
    Entity e = *static_cast<Entity*>(lua_touserdata(L, 1));
    auto& c = e.Get<LightComponent>();
    c.colour = Pull<Maths::vec3>(L, count);
    c.attenuation = Pull<Maths::vec3>(L, count);
    c.brightness = Pull<float>(L, count);
    return 0;
}

int AddLightComponent(lua_State* L)
{
    if (!CheckArgCount(L, LightComponentDimension() + 1)) { return luaL_error(L, "Bad number of args"); }

    int count = 2;
    Entity e = *static_cast<Entity*>(lua_touserdata(L, 1));
    assert(!e.Has<LightComponent>());

    LightComponent c;
    c.colour = Pull<Maths::vec3>(L, count);
    c.attenuation = Pull<Maths::vec3>(L, count);
    c.brightness = Pull<float>(L, count);
    e.Add(c);
    return 0;
}

int GetParticleComponent(lua_State* L)
{
    if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }

    Entity e = *static_cast<Entity*>(lua_touserdata(L, 1));
    assert(e.Has<ParticleComponent>());

    int count = 0;
    const auto& c = e.Get<ParticleComponent>();
    count += Push(L, c.interval);
    count += Push(L, c.velocity);
    count += Push(L, c.velocityNoise);
    count += Push(L, c.acceleration);
    count += Push(L, c.scale);
    count += Push(L, c.life);
    return count;
}

int SetParticleComponent(lua_State* L)
{
    if (!CheckArgCount(L, ParticleComponentDimension() + 1)) { return luaL_error(L, "Bad number of args"); }

    int count = 2;
    Entity e = *static_cast<Entity*>(lua_touserdata(L, 1));
    auto& c = e.Get<ParticleComponent>();
    c.interval = Pull<float>(L, count);
    c.velocity = Pull<Maths::vec3>(L, count);
    c.velocityNoise = Pull<float>(L, count);
    c.acceleration = Pull<Maths::vec3>(L, count);
    c.scale = Pull<Maths::vec3>(L, count);
    c.life = Pull<float>(L, count);
    return 0;
}

int AddParticleComponent(lua_State* L)
{
    if (!CheckArgCount(L, ParticleComponentDimension() + 1)) { return luaL_error(L, "Bad number of args"); }

    int count = 2;
    Entity e = *static_cast<Entity*>(lua_touserdata(L, 1));
    assert(!e.Has<ParticleComponent>());

    ParticleComponent c;
    c.interval = Pull<float>(L, count);
    c.velocity = Pull<Maths::vec3>(L, count);
    c.velocityNoise = Pull<float>(L, count);
    c.acceleration = Pull<Maths::vec3>(L, count);
    c.scale = Pull<Maths::vec3>(L, count);
    c.life = Pull<float>(L, count);
    e.Add(c);
    return 0;
}


}
}
