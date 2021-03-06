// GENERATED FILE
#include "LuaComponents.h"
#include "LuaGlobals.h"
#include "ECS.h"
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

int Push(lua_State* L, const glm::vec2& value)
{
    lua_pushnumber(L, value.x);
    lua_pushnumber(L, value.y);
    return 3;
}

int Push(lua_State* L, const glm::vec3& value)
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

template <> glm::vec3 Pull(lua_State* L, int& count)
{
    float x = (float)lua_tonumber(L, count++);
    float y = (float)lua_tonumber(L, count++);
    float z = (float)lua_tonumber(L, count++);
    return {x, y, z};
}

template <> glm::vec2 Pull(lua_State* L, int& count)
{
    float x = (float)lua_tonumber(L, count++);
    float y = (float)lua_tonumber(L, count++);
    return {x, y};
}

template <> glm::quat Pull(lua_State* L, int& count)
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
template <> constexpr int Dimension<glm::vec2>() { return 2; }
template <> constexpr int Dimension<glm::vec3>() { return 3; }
template <> constexpr int Dimension<glm::quat>() { return 4; }

constexpr int NameComponentDimension()
{
    int count = 0;
    count += Dimension<std::string>(); // name
    return count;
}

constexpr int Transform2DComponentDimension()
{
    int count = 0;
    count += Dimension<glm::vec2>(); // position
    count += Dimension<float>(); // rotation
    count += Dimension<glm::vec2>(); // scale
    return count;
}

constexpr int Transform3DComponentDimension()
{
    int count = 0;
    count += Dimension<glm::vec3>(); // position
    count += Dimension<glm::vec3>(); // scale
    return count;
}

constexpr int ModelComponentDimension()
{
    int count = 0;
    count += Dimension<std::string>(); // mesh
    count += Dimension<std::string>(); // material
    return count;
}

constexpr int RigidBody3DComponentDimension()
{
    int count = 0;
    count += Dimension<glm::vec3>(); // velocity
    count += Dimension<bool>(); // gravity
    count += Dimension<bool>(); // frozen
    count += Dimension<float>(); // bounciness
    count += Dimension<float>(); // frictionCoefficient
    count += Dimension<float>(); // rollingResistance
    count += Dimension<glm::vec3>(); // force
    count += Dimension<bool>(); // onFloor
    return count;
}

constexpr int BoxCollider3DComponentDimension()
{
    int count = 0;
    count += Dimension<glm::vec3>(); // position
    count += Dimension<float>(); // mass
    count += Dimension<glm::vec3>(); // halfExtents
    count += Dimension<bool>(); // applyScale
    return count;
}

constexpr int SphereCollider3DComponentDimension()
{
    int count = 0;
    count += Dimension<glm::vec3>(); // position
    count += Dimension<float>(); // mass
    count += Dimension<float>(); // radius
    return count;
}

constexpr int CapsuleCollider3DComponentDimension()
{
    int count = 0;
    count += Dimension<glm::vec3>(); // position
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

constexpr int Camera3DComponentDimension()
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
    count += Dimension<glm::vec3>(); // colour
    count += Dimension<float>(); // brightness
    return count;
}

constexpr int SunComponentDimension()
{
    int count = 0;
    count += Dimension<glm::vec3>(); // colour
    count += Dimension<float>(); // brightness
    count += Dimension<glm::vec3>(); // direction
    count += Dimension<bool>(); // shadows
    return count;
}

constexpr int AmbienceComponentDimension()
{
    int count = 0;
    count += Dimension<glm::vec3>(); // colour
    count += Dimension<float>(); // brightness
    return count;
}

constexpr int ParticleComponentDimension()
{
    int count = 0;
    count += Dimension<float>(); // interval
    count += Dimension<glm::vec3>(); // velocity
    count += Dimension<float>(); // velocityNoise
    count += Dimension<glm::vec3>(); // acceleration
    count += Dimension<glm::vec3>(); // scale
    count += Dimension<float>(); // life
    return count;
}

constexpr int MeshAnimationComponentDimension()
{
    int count = 0;
    count += Dimension<std::string>(); // name
    count += Dimension<float>(); // time
    count += Dimension<float>(); // speed
    return count;
}


template<typename T> int Lua_Has(lua_State* L)
{
    if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }

    ecs::Entity entity = *static_cast<ecs::Entity*>(lua_touserdata(L, 1));
    lua_pushboolean(L, entity.Has<T>());
    return 1;
}

}

namespace Lua {

int GetNameComponent(lua_State* L)
{
    if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }

    ecs::Entity e = *static_cast<ecs::Entity*>(lua_touserdata(L, 1));
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
    ecs::Entity e = *static_cast<ecs::Entity*>(lua_touserdata(L, 1));
    auto& c = e.Get<NameComponent>();
    c.name = Pull<std::string>(L, count);
    return 0;
}

int AddNameComponent(lua_State* L)
{
    if (!CheckArgCount(L, NameComponentDimension() + 1)) { return luaL_error(L, "Bad number of args"); }

    int count = 2;
    ecs::Entity e = *static_cast<ecs::Entity*>(lua_touserdata(L, 1));
    assert(!e.Has<NameComponent>());

    NameComponent c;
    c.name = Pull<std::string>(L, count);
    e.Add<NameComponent>(c);
    return 0;
}

int GetTransform2DComponent(lua_State* L)
{
    if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }

    ecs::Entity e = *static_cast<ecs::Entity*>(lua_touserdata(L, 1));
    assert(e.Has<Transform2DComponent>());

    int count = 0;
    const auto& c = e.Get<Transform2DComponent>();
    count += Push(L, c.position);
    count += Push(L, c.rotation);
    count += Push(L, c.scale);
    return count;
}

int SetTransform2DComponent(lua_State* L)
{
    if (!CheckArgCount(L, Transform2DComponentDimension() + 1)) { return luaL_error(L, "Bad number of args"); }

    int count = 2;
    ecs::Entity e = *static_cast<ecs::Entity*>(lua_touserdata(L, 1));
    auto& c = e.Get<Transform2DComponent>();
    c.position = Pull<glm::vec2>(L, count);
    c.rotation = Pull<float>(L, count);
    c.scale = Pull<glm::vec2>(L, count);
    return 0;
}

int AddTransform2DComponent(lua_State* L)
{
    if (!CheckArgCount(L, Transform2DComponentDimension() + 1)) { return luaL_error(L, "Bad number of args"); }

    int count = 2;
    ecs::Entity e = *static_cast<ecs::Entity*>(lua_touserdata(L, 1));
    assert(!e.Has<Transform2DComponent>());

    Transform2DComponent c;
    c.position = Pull<glm::vec2>(L, count);
    c.rotation = Pull<float>(L, count);
    c.scale = Pull<glm::vec2>(L, count);
    e.Add<Transform2DComponent>(c);
    return 0;
}

int GetTransform3DComponent(lua_State* L)
{
    if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }

    ecs::Entity e = *static_cast<ecs::Entity*>(lua_touserdata(L, 1));
    assert(e.Has<Transform3DComponent>());

    int count = 0;
    const auto& c = e.Get<Transform3DComponent>();
    count += Push(L, c.position);
    count += Push(L, c.scale);
    return count;
}

int SetTransform3DComponent(lua_State* L)
{
    if (!CheckArgCount(L, Transform3DComponentDimension() + 1)) { return luaL_error(L, "Bad number of args"); }

    int count = 2;
    ecs::Entity e = *static_cast<ecs::Entity*>(lua_touserdata(L, 1));
    auto& c = e.Get<Transform3DComponent>();
    c.position = Pull<glm::vec3>(L, count);
    c.scale = Pull<glm::vec3>(L, count);
    return 0;
}

int AddTransform3DComponent(lua_State* L)
{
    if (!CheckArgCount(L, Transform3DComponentDimension() + 1)) { return luaL_error(L, "Bad number of args"); }

    int count = 2;
    ecs::Entity e = *static_cast<ecs::Entity*>(lua_touserdata(L, 1));
    assert(!e.Has<Transform3DComponent>());

    Transform3DComponent c;
    c.position = Pull<glm::vec3>(L, count);
    c.scale = Pull<glm::vec3>(L, count);
    e.Add<Transform3DComponent>(c);
    return 0;
}

int GetModelComponent(lua_State* L)
{
    if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }

    ecs::Entity e = *static_cast<ecs::Entity*>(lua_touserdata(L, 1));
    assert(e.Has<ModelComponent>());

    int count = 0;
    const auto& c = e.Get<ModelComponent>();
    count += Push(L, c.mesh);
    count += Push(L, c.material);
    return count;
}

int SetModelComponent(lua_State* L)
{
    if (!CheckArgCount(L, ModelComponentDimension() + 1)) { return luaL_error(L, "Bad number of args"); }

    int count = 2;
    ecs::Entity e = *static_cast<ecs::Entity*>(lua_touserdata(L, 1));
    auto& c = e.Get<ModelComponent>();
    c.mesh = Pull<std::string>(L, count);
    c.material = Pull<std::string>(L, count);
    return 0;
}

int AddModelComponent(lua_State* L)
{
    if (!CheckArgCount(L, ModelComponentDimension() + 1)) { return luaL_error(L, "Bad number of args"); }

    int count = 2;
    ecs::Entity e = *static_cast<ecs::Entity*>(lua_touserdata(L, 1));
    assert(!e.Has<ModelComponent>());

    ModelComponent c;
    c.mesh = Pull<std::string>(L, count);
    c.material = Pull<std::string>(L, count);
    e.Add<ModelComponent>(c);
    return 0;
}

int GetRigidBody3DComponent(lua_State* L)
{
    if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }

    ecs::Entity e = *static_cast<ecs::Entity*>(lua_touserdata(L, 1));
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
    ecs::Entity e = *static_cast<ecs::Entity*>(lua_touserdata(L, 1));
    auto& c = e.Get<RigidBody3DComponent>();
    c.velocity = Pull<glm::vec3>(L, count);
    c.gravity = Pull<bool>(L, count);
    c.frozen = Pull<bool>(L, count);
    c.bounciness = Pull<float>(L, count);
    c.frictionCoefficient = Pull<float>(L, count);
    c.rollingResistance = Pull<float>(L, count);
    c.force = Pull<glm::vec3>(L, count);
    c.onFloor = Pull<bool>(L, count);
    return 0;
}

int AddRigidBody3DComponent(lua_State* L)
{
    if (!CheckArgCount(L, RigidBody3DComponentDimension() + 1)) { return luaL_error(L, "Bad number of args"); }

    int count = 2;
    ecs::Entity e = *static_cast<ecs::Entity*>(lua_touserdata(L, 1));
    assert(!e.Has<RigidBody3DComponent>());

    RigidBody3DComponent c;
    c.velocity = Pull<glm::vec3>(L, count);
    c.gravity = Pull<bool>(L, count);
    c.frozen = Pull<bool>(L, count);
    c.bounciness = Pull<float>(L, count);
    c.frictionCoefficient = Pull<float>(L, count);
    c.rollingResistance = Pull<float>(L, count);
    c.force = Pull<glm::vec3>(L, count);
    c.onFloor = Pull<bool>(L, count);
    e.Add<RigidBody3DComponent>(c);
    return 0;
}

int GetBoxCollider3DComponent(lua_State* L)
{
    if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }

    ecs::Entity e = *static_cast<ecs::Entity*>(lua_touserdata(L, 1));
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
    ecs::Entity e = *static_cast<ecs::Entity*>(lua_touserdata(L, 1));
    auto& c = e.Get<BoxCollider3DComponent>();
    c.position = Pull<glm::vec3>(L, count);
    c.mass = Pull<float>(L, count);
    c.halfExtents = Pull<glm::vec3>(L, count);
    c.applyScale = Pull<bool>(L, count);
    return 0;
}

int AddBoxCollider3DComponent(lua_State* L)
{
    if (!CheckArgCount(L, BoxCollider3DComponentDimension() + 1)) { return luaL_error(L, "Bad number of args"); }

    int count = 2;
    ecs::Entity e = *static_cast<ecs::Entity*>(lua_touserdata(L, 1));
    assert(!e.Has<BoxCollider3DComponent>());

    BoxCollider3DComponent c;
    c.position = Pull<glm::vec3>(L, count);
    c.mass = Pull<float>(L, count);
    c.halfExtents = Pull<glm::vec3>(L, count);
    c.applyScale = Pull<bool>(L, count);
    e.Add<BoxCollider3DComponent>(c);
    return 0;
}

int GetSphereCollider3DComponent(lua_State* L)
{
    if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }

    ecs::Entity e = *static_cast<ecs::Entity*>(lua_touserdata(L, 1));
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
    ecs::Entity e = *static_cast<ecs::Entity*>(lua_touserdata(L, 1));
    auto& c = e.Get<SphereCollider3DComponent>();
    c.position = Pull<glm::vec3>(L, count);
    c.mass = Pull<float>(L, count);
    c.radius = Pull<float>(L, count);
    return 0;
}

int AddSphereCollider3DComponent(lua_State* L)
{
    if (!CheckArgCount(L, SphereCollider3DComponentDimension() + 1)) { return luaL_error(L, "Bad number of args"); }

    int count = 2;
    ecs::Entity e = *static_cast<ecs::Entity*>(lua_touserdata(L, 1));
    assert(!e.Has<SphereCollider3DComponent>());

    SphereCollider3DComponent c;
    c.position = Pull<glm::vec3>(L, count);
    c.mass = Pull<float>(L, count);
    c.radius = Pull<float>(L, count);
    e.Add<SphereCollider3DComponent>(c);
    return 0;
}

int GetCapsuleCollider3DComponent(lua_State* L)
{
    if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }

    ecs::Entity e = *static_cast<ecs::Entity*>(lua_touserdata(L, 1));
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
    ecs::Entity e = *static_cast<ecs::Entity*>(lua_touserdata(L, 1));
    auto& c = e.Get<CapsuleCollider3DComponent>();
    c.position = Pull<glm::vec3>(L, count);
    c.mass = Pull<float>(L, count);
    c.radius = Pull<float>(L, count);
    c.height = Pull<float>(L, count);
    return 0;
}

int AddCapsuleCollider3DComponent(lua_State* L)
{
    if (!CheckArgCount(L, CapsuleCollider3DComponentDimension() + 1)) { return luaL_error(L, "Bad number of args"); }

    int count = 2;
    ecs::Entity e = *static_cast<ecs::Entity*>(lua_touserdata(L, 1));
    assert(!e.Has<CapsuleCollider3DComponent>());

    CapsuleCollider3DComponent c;
    c.position = Pull<glm::vec3>(L, count);
    c.mass = Pull<float>(L, count);
    c.radius = Pull<float>(L, count);
    c.height = Pull<float>(L, count);
    e.Add<CapsuleCollider3DComponent>(c);
    return 0;
}

int GetScriptComponent(lua_State* L)
{
    if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }

    ecs::Entity e = *static_cast<ecs::Entity*>(lua_touserdata(L, 1));
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
    ecs::Entity e = *static_cast<ecs::Entity*>(lua_touserdata(L, 1));
    auto& c = e.Get<ScriptComponent>();
    c.script = Pull<std::string>(L, count);
    c.active = Pull<bool>(L, count);
    return 0;
}

int AddScriptComponent(lua_State* L)
{
    if (!CheckArgCount(L, ScriptComponentDimension() + 1)) { return luaL_error(L, "Bad number of args"); }

    int count = 2;
    ecs::Entity e = *static_cast<ecs::Entity*>(lua_touserdata(L, 1));
    assert(!e.Has<ScriptComponent>());

    ScriptComponent c;
    c.script = Pull<std::string>(L, count);
    c.active = Pull<bool>(L, count);
    e.Add<ScriptComponent>(c);
    return 0;
}

int GetCamera3DComponent(lua_State* L)
{
    if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }

    ecs::Entity e = *static_cast<ecs::Entity*>(lua_touserdata(L, 1));
    assert(e.Has<Camera3DComponent>());

    int count = 0;
    const auto& c = e.Get<Camera3DComponent>();
    count += Push(L, c.fov);
    count += Push(L, c.pitch);
    return count;
}

int SetCamera3DComponent(lua_State* L)
{
    if (!CheckArgCount(L, Camera3DComponentDimension() + 1)) { return luaL_error(L, "Bad number of args"); }

    int count = 2;
    ecs::Entity e = *static_cast<ecs::Entity*>(lua_touserdata(L, 1));
    auto& c = e.Get<Camera3DComponent>();
    c.fov = Pull<float>(L, count);
    c.pitch = Pull<float>(L, count);
    return 0;
}

int AddCamera3DComponent(lua_State* L)
{
    if (!CheckArgCount(L, Camera3DComponentDimension() + 1)) { return luaL_error(L, "Bad number of args"); }

    int count = 2;
    ecs::Entity e = *static_cast<ecs::Entity*>(lua_touserdata(L, 1));
    assert(!e.Has<Camera3DComponent>());

    Camera3DComponent c;
    c.fov = Pull<float>(L, count);
    c.pitch = Pull<float>(L, count);
    e.Add<Camera3DComponent>(c);
    return 0;
}

int GetSelectComponent(lua_State* L)
{
    if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }

    ecs::Entity e = *static_cast<ecs::Entity*>(lua_touserdata(L, 1));
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
    ecs::Entity e = *static_cast<ecs::Entity*>(lua_touserdata(L, 1));
    auto& c = e.Get<SelectComponent>();
    c.selected = Pull<bool>(L, count);
    c.hovered = Pull<bool>(L, count);
    return 0;
}

int AddSelectComponent(lua_State* L)
{
    if (!CheckArgCount(L, SelectComponentDimension() + 1)) { return luaL_error(L, "Bad number of args"); }

    int count = 2;
    ecs::Entity e = *static_cast<ecs::Entity*>(lua_touserdata(L, 1));
    assert(!e.Has<SelectComponent>());

    SelectComponent c;
    c.selected = Pull<bool>(L, count);
    c.hovered = Pull<bool>(L, count);
    e.Add<SelectComponent>(c);
    return 0;
}

int GetPathComponent(lua_State* L)
{
    if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }

    ecs::Entity e = *static_cast<ecs::Entity*>(lua_touserdata(L, 1));
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
    ecs::Entity e = *static_cast<ecs::Entity*>(lua_touserdata(L, 1));
    auto& c = e.Get<PathComponent>();
    c.speed = Pull<float>(L, count);
    return 0;
}

int AddPathComponent(lua_State* L)
{
    if (!CheckArgCount(L, PathComponentDimension() + 1)) { return luaL_error(L, "Bad number of args"); }

    int count = 2;
    ecs::Entity e = *static_cast<ecs::Entity*>(lua_touserdata(L, 1));
    assert(!e.Has<PathComponent>());

    PathComponent c;
    c.speed = Pull<float>(L, count);
    e.Add<PathComponent>(c);
    return 0;
}

int GetGridComponent(lua_State* L)
{
    if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }

    ecs::Entity e = *static_cast<ecs::Entity*>(lua_touserdata(L, 1));
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
    ecs::Entity e = *static_cast<ecs::Entity*>(lua_touserdata(L, 1));
    auto& c = e.Get<GridComponent>();
    c.x = Pull<int>(L, count);
    c.z = Pull<int>(L, count);
    return 0;
}

int AddGridComponent(lua_State* L)
{
    if (!CheckArgCount(L, GridComponentDimension() + 1)) { return luaL_error(L, "Bad number of args"); }

    int count = 2;
    ecs::Entity e = *static_cast<ecs::Entity*>(lua_touserdata(L, 1));
    assert(!e.Has<GridComponent>());

    GridComponent c;
    c.x = Pull<int>(L, count);
    c.z = Pull<int>(L, count);
    e.Add<GridComponent>(c);
    return 0;
}

int GetLightComponent(lua_State* L)
{
    if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }

    ecs::Entity e = *static_cast<ecs::Entity*>(lua_touserdata(L, 1));
    assert(e.Has<LightComponent>());

    int count = 0;
    const auto& c = e.Get<LightComponent>();
    count += Push(L, c.colour);
    count += Push(L, c.brightness);
    return count;
}

int SetLightComponent(lua_State* L)
{
    if (!CheckArgCount(L, LightComponentDimension() + 1)) { return luaL_error(L, "Bad number of args"); }

    int count = 2;
    ecs::Entity e = *static_cast<ecs::Entity*>(lua_touserdata(L, 1));
    auto& c = e.Get<LightComponent>();
    c.colour = Pull<glm::vec3>(L, count);
    c.brightness = Pull<float>(L, count);
    return 0;
}

int AddLightComponent(lua_State* L)
{
    if (!CheckArgCount(L, LightComponentDimension() + 1)) { return luaL_error(L, "Bad number of args"); }

    int count = 2;
    ecs::Entity e = *static_cast<ecs::Entity*>(lua_touserdata(L, 1));
    assert(!e.Has<LightComponent>());

    LightComponent c;
    c.colour = Pull<glm::vec3>(L, count);
    c.brightness = Pull<float>(L, count);
    e.Add<LightComponent>(c);
    return 0;
}

int GetSunComponent(lua_State* L)
{
    if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }

    ecs::Entity e = *static_cast<ecs::Entity*>(lua_touserdata(L, 1));
    assert(e.Has<SunComponent>());

    int count = 0;
    const auto& c = e.Get<SunComponent>();
    count += Push(L, c.colour);
    count += Push(L, c.brightness);
    count += Push(L, c.direction);
    count += Push(L, c.shadows);
    return count;
}

int SetSunComponent(lua_State* L)
{
    if (!CheckArgCount(L, SunComponentDimension() + 1)) { return luaL_error(L, "Bad number of args"); }

    int count = 2;
    ecs::Entity e = *static_cast<ecs::Entity*>(lua_touserdata(L, 1));
    auto& c = e.Get<SunComponent>();
    c.colour = Pull<glm::vec3>(L, count);
    c.brightness = Pull<float>(L, count);
    c.direction = Pull<glm::vec3>(L, count);
    c.shadows = Pull<bool>(L, count);
    return 0;
}

int AddSunComponent(lua_State* L)
{
    if (!CheckArgCount(L, SunComponentDimension() + 1)) { return luaL_error(L, "Bad number of args"); }

    int count = 2;
    ecs::Entity e = *static_cast<ecs::Entity*>(lua_touserdata(L, 1));
    assert(!e.Has<SunComponent>());

    SunComponent c;
    c.colour = Pull<glm::vec3>(L, count);
    c.brightness = Pull<float>(L, count);
    c.direction = Pull<glm::vec3>(L, count);
    c.shadows = Pull<bool>(L, count);
    e.Add<SunComponent>(c);
    return 0;
}

int GetAmbienceComponent(lua_State* L)
{
    if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }

    ecs::Entity e = *static_cast<ecs::Entity*>(lua_touserdata(L, 1));
    assert(e.Has<AmbienceComponent>());

    int count = 0;
    const auto& c = e.Get<AmbienceComponent>();
    count += Push(L, c.colour);
    count += Push(L, c.brightness);
    return count;
}

int SetAmbienceComponent(lua_State* L)
{
    if (!CheckArgCount(L, AmbienceComponentDimension() + 1)) { return luaL_error(L, "Bad number of args"); }

    int count = 2;
    ecs::Entity e = *static_cast<ecs::Entity*>(lua_touserdata(L, 1));
    auto& c = e.Get<AmbienceComponent>();
    c.colour = Pull<glm::vec3>(L, count);
    c.brightness = Pull<float>(L, count);
    return 0;
}

int AddAmbienceComponent(lua_State* L)
{
    if (!CheckArgCount(L, AmbienceComponentDimension() + 1)) { return luaL_error(L, "Bad number of args"); }

    int count = 2;
    ecs::Entity e = *static_cast<ecs::Entity*>(lua_touserdata(L, 1));
    assert(!e.Has<AmbienceComponent>());

    AmbienceComponent c;
    c.colour = Pull<glm::vec3>(L, count);
    c.brightness = Pull<float>(L, count);
    e.Add<AmbienceComponent>(c);
    return 0;
}

int GetParticleComponent(lua_State* L)
{
    if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }

    ecs::Entity e = *static_cast<ecs::Entity*>(lua_touserdata(L, 1));
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
    ecs::Entity e = *static_cast<ecs::Entity*>(lua_touserdata(L, 1));
    auto& c = e.Get<ParticleComponent>();
    c.interval = Pull<float>(L, count);
    c.velocity = Pull<glm::vec3>(L, count);
    c.velocityNoise = Pull<float>(L, count);
    c.acceleration = Pull<glm::vec3>(L, count);
    c.scale = Pull<glm::vec3>(L, count);
    c.life = Pull<float>(L, count);
    return 0;
}

int AddParticleComponent(lua_State* L)
{
    if (!CheckArgCount(L, ParticleComponentDimension() + 1)) { return luaL_error(L, "Bad number of args"); }

    int count = 2;
    ecs::Entity e = *static_cast<ecs::Entity*>(lua_touserdata(L, 1));
    assert(!e.Has<ParticleComponent>());

    ParticleComponent c;
    c.interval = Pull<float>(L, count);
    c.velocity = Pull<glm::vec3>(L, count);
    c.velocityNoise = Pull<float>(L, count);
    c.acceleration = Pull<glm::vec3>(L, count);
    c.scale = Pull<glm::vec3>(L, count);
    c.life = Pull<float>(L, count);
    e.Add<ParticleComponent>(c);
    return 0;
}

int GetMeshAnimationComponent(lua_State* L)
{
    if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }

    ecs::Entity e = *static_cast<ecs::Entity*>(lua_touserdata(L, 1));
    assert(e.Has<MeshAnimationComponent>());

    int count = 0;
    const auto& c = e.Get<MeshAnimationComponent>();
    count += Push(L, c.name);
    count += Push(L, c.time);
    count += Push(L, c.speed);
    return count;
}

int SetMeshAnimationComponent(lua_State* L)
{
    if (!CheckArgCount(L, MeshAnimationComponentDimension() + 1)) { return luaL_error(L, "Bad number of args"); }

    int count = 2;
    ecs::Entity e = *static_cast<ecs::Entity*>(lua_touserdata(L, 1));
    auto& c = e.Get<MeshAnimationComponent>();
    c.name = Pull<std::string>(L, count);
    c.time = Pull<float>(L, count);
    c.speed = Pull<float>(L, count);
    return 0;
}

int AddMeshAnimationComponent(lua_State* L)
{
    if (!CheckArgCount(L, MeshAnimationComponentDimension() + 1)) { return luaL_error(L, "Bad number of args"); }

    int count = 2;
    ecs::Entity e = *static_cast<ecs::Entity*>(lua_touserdata(L, 1));
    assert(!e.Has<MeshAnimationComponent>());

    MeshAnimationComponent c;
    c.name = Pull<std::string>(L, count);
    c.time = Pull<float>(L, count);
    c.speed = Pull<float>(L, count);
    e.Add<MeshAnimationComponent>(c);
    return 0;
}

}

void RegisterComponentFunctions(lua_State* L)
{
    lua_register(L, "Lua_GetNameComponent", &Lua::GetNameComponent);
    lua_register(L, "Lua_SetNameComponent", &Lua::SetNameComponent);
    lua_register(L, "Lua_AddNameComponent", &Lua::AddNameComponent);
    lua_register(L, "HasNameComponent", &Lua_Has<NameComponent>);

    lua_register(L, "Lua_GetTransform2DComponent", &Lua::GetTransform2DComponent);
    lua_register(L, "Lua_SetTransform2DComponent", &Lua::SetTransform2DComponent);
    lua_register(L, "Lua_AddTransform2DComponent", &Lua::AddTransform2DComponent);
    lua_register(L, "HasTransform2DComponent", &Lua_Has<Transform2DComponent>);

    lua_register(L, "Lua_GetTransform3DComponent", &Lua::GetTransform3DComponent);
    lua_register(L, "Lua_SetTransform3DComponent", &Lua::SetTransform3DComponent);
    lua_register(L, "Lua_AddTransform3DComponent", &Lua::AddTransform3DComponent);
    lua_register(L, "HasTransform3DComponent", &Lua_Has<Transform3DComponent>);

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

    lua_register(L, "Lua_GetCamera3DComponent", &Lua::GetCamera3DComponent);
    lua_register(L, "Lua_SetCamera3DComponent", &Lua::SetCamera3DComponent);
    lua_register(L, "Lua_AddCamera3DComponent", &Lua::AddCamera3DComponent);
    lua_register(L, "HasCamera3DComponent", &Lua_Has<Camera3DComponent>);

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

    lua_register(L, "Lua_GetSunComponent", &Lua::GetSunComponent);
    lua_register(L, "Lua_SetSunComponent", &Lua::SetSunComponent);
    lua_register(L, "Lua_AddSunComponent", &Lua::AddSunComponent);
    lua_register(L, "HasSunComponent", &Lua_Has<SunComponent>);

    lua_register(L, "Lua_GetAmbienceComponent", &Lua::GetAmbienceComponent);
    lua_register(L, "Lua_SetAmbienceComponent", &Lua::SetAmbienceComponent);
    lua_register(L, "Lua_AddAmbienceComponent", &Lua::AddAmbienceComponent);
    lua_register(L, "HasAmbienceComponent", &Lua_Has<AmbienceComponent>);

    lua_register(L, "Lua_GetParticleComponent", &Lua::GetParticleComponent);
    lua_register(L, "Lua_SetParticleComponent", &Lua::SetParticleComponent);
    lua_register(L, "Lua_AddParticleComponent", &Lua::AddParticleComponent);
    lua_register(L, "HasParticleComponent", &Lua_Has<ParticleComponent>);

    lua_register(L, "Lua_GetMeshAnimationComponent", &Lua::GetMeshAnimationComponent);
    lua_register(L, "Lua_SetMeshAnimationComponent", &Lua::SetMeshAnimationComponent);
    lua_register(L, "Lua_AddMeshAnimationComponent", &Lua::AddMeshAnimationComponent);
    lua_register(L, "HasMeshAnimationComponent", &Lua_Has<MeshAnimationComponent>);

}

}
