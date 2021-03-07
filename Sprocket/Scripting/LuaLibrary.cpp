// GENERATED FILE
#include "LuaLibrary.h"
#include "LuaScript.h"
#include "ECS.h"
#include "Scene.h"
#include "InputProxy.h"
#include "Window.h"
#include "Components.h"
#include "Log.h"

#include <lua.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/trigonometric.hpp>

namespace Sprocket {
namespace lua {
namespace {

void do_file(lua_State* L, const char* file)
{
    if (luaL_dofile(L, file)) {
        log::error("[Lua]: Could not load {}", lua_tostring(L, -1));
    }
}

template <typename T>
T* get_pointer(lua_State* L, const std::string& var_name)
{
    lua_getglobal(L, var_name.c_str());
    T* ret = nullptr;
    if (lua_islightuserdata(L, -1)) {
        ret = static_cast<T*>(lua_touserdata(L, -1));
    } else {
        log::error("Variable {} is not light user data", var_name);
    }
    lua_pop(L, 1);
    return ret;
}

bool CheckReturnCode(lua_State* L, int rc)
{
    if (rc != LUA_OK) {
        const char* error = lua_tostring(L, -1);
        log::error("[Lua]: {}", error);
        return false;
    }
    return true;
}

bool CheckArgCount(lua_State* L, int argc)
{
    int args = lua_gettop(L);
    if (args != argc) {
        log::error("[Lua]: Expected {} args, got {}", argc, args);
        return false;
    }
    return true;
}

}

void register_scene_functions(lua::Script& script, Scene& scene)
{
    lua_State* L = script.native_handle();
    script.set_value("__scene__", &scene);

    // Add functions for creating and destroying entities.
    lua_register(L, "NewEntity", [](lua_State* L) {
        if (!CheckArgCount(L, 0)) { return luaL_error(L, "Bad number of args"); }
        auto luaEntity = static_cast<ecs::Entity*>(lua_newuserdata(L, sizeof(ecs::Entity)));
        *luaEntity = get_pointer<Scene>(L, "__scene__")->Entities().New();
        return 1;
    });

    lua_register(L, "DeleteEntity", [](lua_State* L) {
        if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }
        auto luaEntity = static_cast<ecs::Entity*>(lua_touserdata(L, 1));
        luaEntity->Delete();
        return 0;
    });

    // Add functions for iterating over all entities in __scene__. The C++ functions
    // should not be used directly, instead they should be used via the Scene:Each
    // function implemented last in Lua.
    using Generator = cppcoro::generator<ecs::Entity>;
    using Iterator = typename Generator::iterator;
    static_assert(std::is_trivially_destructible_v<Iterator>);

    lua_register(L, "_Each_New", [](lua_State* L) {
        if (!CheckArgCount(L, 0)) { return luaL_error(L, "Bad number of args"); }
        auto gen = new Generator(get_pointer<Scene>(L, "__scene__")->Entities().Each());
        lua_pushlightuserdata(L, static_cast<void*>(gen));
        return 1;
    });

    lua_register(L, "_Each_Delete", [](lua_State* L) {
        if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }
        delete static_cast<Generator*>(lua_touserdata(L, 1));
        return 0;
    });

    lua_register(L, "_Each_Iter_Start", [](lua_State*L) {
        if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }
        auto gen = static_cast<Generator*>(lua_touserdata(L, 1));

        auto iter = static_cast<Iterator*>(lua_newuserdata(L, sizeof(Iterator)));
        *iter = gen->begin();
        return 1;
    });

    lua_register(L, "_Each_Iter_Valid", [](lua_State* L) {
        if (!CheckArgCount(L, 2)) { return luaL_error(L, "Bad number of args"); }
        auto gen = static_cast<Generator*>(lua_touserdata(L, 1));
        auto iter = static_cast<Iterator*>(lua_touserdata(L, 2));

        lua_pushboolean(L, *iter != gen->end());
        return 1;
    });

    lua_register(L, "_Each_Iter_Next", [](lua_State* L) {
        if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }
        auto iter = static_cast<Iterator*>(lua_touserdata(L, 1));
        ++(*iter);
        return 0;
    });

    lua_register(L, "_Each_Iter_Get", [](lua_State* L) {
        if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }
        auto iterator = static_cast<Iterator*>(lua_touserdata(L, 1));

        auto luaEntity = static_cast<ecs::Entity*>(lua_newuserdata(L, sizeof(ecs::Entity)));
        *luaEntity = **iterator;
        return 1;
    });

    // Hook all of the above functions into a single generator function.
    luaL_dostring(L, R"lua(
        Scene = Class(function(self)
        end)

        function Scene:Each()
            local generator = _Each_New()
            local iter = _Each_Iter_Start(generator)

            return function()
                if _Each_Iter_Valid(generator, iter) then
                    local entity = _Each_Iter_Get(iter)
                    _Each_Iter_Next(iter)
                    return entity
                else
                    _Each_Delete(generator)
                end
            end
        end
    )lua");
}

void register_input_functions(lua::Script& script, InputProxy& input)
{
    lua_State* L = script.native_handle();
    script.set_value("__input__", &input);

    lua_register(L, "IsKeyDown", [](lua_State* L) {
        if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }

        if (auto ip = get_pointer<InputProxy>(L, "__input__"); ip) {
            int x = (int)lua_tointeger(L, 1);
            lua_pushboolean(L, ip->IsKeyboardDown(x));
        }
        else {
            lua_pushboolean(L, false);
        }

        return 1;
    });

    lua_register(L, "IsMouseDown", [](lua_State* L) {
        if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }

        if (auto ip = get_pointer<InputProxy>(L, "__input__"); ip) {
            int x = (int)lua_tointeger(L, 1);
            lua_pushboolean(L, ip->IsMouseDown(x));
        }
        else {
            lua_pushboolean(L, false);
        }

        return 1;
    });
}

void register_window_functions(lua::Script& script, Window& window)
{
    lua_State* L = script.native_handle();
    script.set_value("__window__", &window);

    lua_register(L, "GetMousePos", [](lua_State* L) {
        if (!CheckArgCount(L, 0)) { return luaL_error(L, "Bad number of args"); }

        if (auto w = get_pointer<Window>(L, "__window__"); w) {
            auto offset = w->GetMousePos();
            lua_pushnumber(L, offset.x);
            lua_pushnumber(L, offset.y);
        } else {
            lua_pushnumber(L, 0);
            lua_pushnumber(L, 0);
        }
        return 2;
    });

    lua_register(L, "GetMouseOffset", [](lua_State* L) {
        if (!CheckArgCount(L, 0)) { return luaL_error(L, "Bad number of args"); }

        if (auto w = get_pointer<Window>(L, "__window__"); w) {
            auto offset = w->GetMouseOffset();
            lua_pushnumber(L, offset.x);
            lua_pushnumber(L, offset.y);
        } else {
            lua_pushnumber(L, 0);
            lua_pushnumber(L, 0);
        }
        return 2;
    });
}

void register_entity_transformation_functions(lua::Script& script)
{
    lua_State* L = script.native_handle();

    lua_register(L, "_SetLookAt", [](lua_State* L) {
        if (!CheckArgCount(L, 7)) { return luaL_error(L, "Bad number of args"); }

        ecs::Entity entity = *static_cast<ecs::Entity*>(lua_touserdata(L, 1));
        float px = (float)lua_tonumber(L, 2);
        float py = (float)lua_tonumber(L, 3);
        float pz = (float)lua_tonumber(L, 4);

        float tx = (float)lua_tonumber(L, 5);
        float ty = (float)lua_tonumber(L, 6);
        float tz = (float)lua_tonumber(L, 7);

        auto& tr = entity.Get<Transform3DComponent>();
        tr.position = glm::vec3(px, py, pz);
        tr.orientation = glm::conjugate(glm::quat_cast(glm::lookAt(tr.position, {tx, ty, tz}, {0.0, 1.0, 0.0})));
        return 0;
    });

    luaL_dostring(L, R"lua(
        function SetLookAt(entity, pos, target)
            _SetLookAt(entity, pos.x, pos.y, pos.z, target.x, target.y, target.z)
        end
    )lua");

    lua_register(L, "RotateY", [](lua_State* L) {
        if (!CheckArgCount(L, 2)) { return luaL_error(L, "Bad number of args"); };

        ecs::Entity entity = *static_cast<ecs::Entity*>(lua_touserdata(L, 1));
        auto& tr = entity.Get<Transform3DComponent>();

        float yaw = (float)lua_tonumber(L, 2);
        tr.orientation = glm::rotate(tr.orientation, yaw, {0, 1, 0});
        return 0;
    });

    lua_register(L, "_GetForwardsDir", [](lua_State* L) {
        if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }

        ecs::Entity entity = *static_cast<ecs::Entity*>(lua_touserdata(L, 1));
        auto& tr = entity.Get<Transform3DComponent>();
        auto o = tr.orientation;

        if (entity.Has<Camera3DComponent>()) {
            auto pitch = entity.Get<Camera3DComponent>().pitch;
            o = glm::rotate(o, pitch, {1, 0, 0});
        }

        auto forwards = Maths::Forwards(o);

        lua_pushnumber(L, forwards.x);
        lua_pushnumber(L, forwards.y);
        lua_pushnumber(L, forwards.z);
        return 3;
    });

    luaL_dostring(L, R"lua(
        function GetForwardsDir(entity)
            local x, y, z = _GetForwardsDir(entity)
            return Vec3(x, y, z)
        end
    )lua");

    lua_register(L, "_GetRightDir", [](lua_State* L) {
        if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }

        ecs::Entity entity = *static_cast<ecs::Entity*>(lua_touserdata(L, 1));
        auto& tr = entity.Get<Transform3DComponent>();
        auto right = Maths::Right(tr.orientation);
        lua_pushnumber(L, right.x);
        lua_pushnumber(L, right.y);
        lua_pushnumber(L, right.z);
        return 3;
    });

    luaL_dostring(L, R"lua(
        function GetRightDir(entity)
            local x, y, z = _GetRightDir(entity)
            return Vec3(x, y, z)
        end
    )lua");

    lua_register(L, "MakeUpright", [](lua_State* L) {
        if (!CheckArgCount(L, 2)) { return luaL_error(L, "Bad number of args"); }
        ecs::Entity entity = *static_cast<ecs::Entity*>(lua_touserdata(L, 1));
        auto& tr = entity.Get<Transform3DComponent>();
        float yaw = (float)lua_tonumber(L, 2);
        tr.orientation = glm::quat(glm::vec3(0, yaw, 0));
        return 0;
    });
}

// COMPONENT RELATED CODE - MOSTLY GENERATED

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

void register_entity_component_functions(lua::Script& script)
{
    lua_State* L = script.native_handle();

    // Functions for NameComponent =====================================================

    luaL_dostring(L, R"lua(
        NameComponent = Class(function(self, name)
            self.name = name
        end)
    )lua");

    lua_register(L, "_GetNameComponent", [](lua_State* L) {
        if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }

        ecs::Entity e = *static_cast<ecs::Entity*>(lua_touserdata(L, 1));
        assert(e.Has<NameComponent>());

        int count = 0;
        const auto& c = e.Get<NameComponent>();
        count += Push(L, c.name);
        return count;
    });

    luaL_dostring(L, R"lua(
        function GetNameComponent(entity)
            x0 = _GetNameComponent(entity)
            return NameComponent(x0)
        end
    )lua");

    lua_register(L, "_SetNameComponent", [](lua_State* L) {
        if (!CheckArgCount(L, NameComponentDimension() + 1)) { return luaL_error(L, "Bad number of args"); }

        int count = 2;
        ecs::Entity e = *static_cast<ecs::Entity*>(lua_touserdata(L, 1));
        auto& c = e.Get<NameComponent>();
        c.name = Pull<std::string>(L, count);
        return 0;
    });

    luaL_dostring(L, R"lua(
        function SetNameComponent(entity, c)
            _SetNameComponent(entity, c.name)
        end
    )lua");

    lua_register(L, "_AddNameComponent", [](lua_State* L) {
        if (!CheckArgCount(L, NameComponentDimension() + 1)) { return luaL_error(L, "Bad number of args"); }

        int count = 2;
        ecs::Entity e = *static_cast<ecs::Entity*>(lua_touserdata(L, 1));
        assert(!e.Has<NameComponent>());

        NameComponent c;
        c.name = Pull<std::string>(L, count);
        e.Add<NameComponent>(c);
        return 0;
    });

    luaL_dostring(L, R"lua(
        function AddNameComponent(entity, c)
            _AddNameComponent(entity, c.name)
        end
    )lua");

    lua_register(L, "HasNameComponent", &Lua_Has<NameComponent>);


    // Functions for Transform2DComponent =====================================================

    luaL_dostring(L, R"lua(
        Transform2DComponent = Class(function(self, position, rotation, scale)
            self.position = position
            self.rotation = rotation
            self.scale = scale
        end)
    )lua");

    lua_register(L, "_GetTransform2DComponent", [](lua_State* L) {
        if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }

        ecs::Entity e = *static_cast<ecs::Entity*>(lua_touserdata(L, 1));
        assert(e.Has<Transform2DComponent>());

        int count = 0;
        const auto& c = e.Get<Transform2DComponent>();
        count += Push(L, c.position);
        count += Push(L, c.rotation);
        count += Push(L, c.scale);
        return count;
    });

    luaL_dostring(L, R"lua(
        function GetTransform2DComponent(entity)
            x0, x1, x2, x3, x4 = _GetTransform2DComponent(entity)
            return Transform2DComponent(Vec3(x0, x1), x2, Vec3(x3, x4))
        end
    )lua");

    lua_register(L, "_SetTransform2DComponent", [](lua_State* L) {
        if (!CheckArgCount(L, Transform2DComponentDimension() + 1)) { return luaL_error(L, "Bad number of args"); }

        int count = 2;
        ecs::Entity e = *static_cast<ecs::Entity*>(lua_touserdata(L, 1));
        auto& c = e.Get<Transform2DComponent>();
        c.position = Pull<glm::vec2>(L, count);
        c.rotation = Pull<float>(L, count);
        c.scale = Pull<glm::vec2>(L, count);
        return 0;
    });

    luaL_dostring(L, R"lua(
        function SetTransform2DComponent(entity, c)
            _SetTransform2DComponent(entity, c.position, c.rotation, c.scale)
        end
    )lua");

    lua_register(L, "_AddTransform2DComponent", [](lua_State* L) {
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
    });

    luaL_dostring(L, R"lua(
        function AddTransform2DComponent(entity, c)
            _AddTransform2DComponent(entity, c.position, c.rotation, c.scale)
        end
    )lua");

    lua_register(L, "HasTransform2DComponent", &Lua_Has<Transform2DComponent>);


    // Functions for Transform3DComponent =====================================================

    luaL_dostring(L, R"lua(
        Transform3DComponent = Class(function(self, position, scale)
            self.position = position
            self.scale = scale
        end)
    )lua");

    lua_register(L, "_GetTransform3DComponent", [](lua_State* L) {
        if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }

        ecs::Entity e = *static_cast<ecs::Entity*>(lua_touserdata(L, 1));
        assert(e.Has<Transform3DComponent>());

        int count = 0;
        const auto& c = e.Get<Transform3DComponent>();
        count += Push(L, c.position);
        count += Push(L, c.scale);
        return count;
    });

    luaL_dostring(L, R"lua(
        function GetTransform3DComponent(entity)
            x0, x1, x2, x3, x4, x5 = _GetTransform3DComponent(entity)
            return Transform3DComponent(Vec3(x0, x1, x2), Vec3(x3, x4, x5))
        end
    )lua");

    lua_register(L, "_SetTransform3DComponent", [](lua_State* L) {
        if (!CheckArgCount(L, Transform3DComponentDimension() + 1)) { return luaL_error(L, "Bad number of args"); }

        int count = 2;
        ecs::Entity e = *static_cast<ecs::Entity*>(lua_touserdata(L, 1));
        auto& c = e.Get<Transform3DComponent>();
        c.position = Pull<glm::vec3>(L, count);
        c.scale = Pull<glm::vec3>(L, count);
        return 0;
    });

    luaL_dostring(L, R"lua(
        function SetTransform3DComponent(entity, c)
            _SetTransform3DComponent(entity, c.position.x, c.position.y, c.position.z, c.scale.x, c.scale.y, c.scale.z)
        end
    )lua");

    lua_register(L, "_AddTransform3DComponent", [](lua_State* L) {
        if (!CheckArgCount(L, Transform3DComponentDimension() + 1)) { return luaL_error(L, "Bad number of args"); }

        int count = 2;
        ecs::Entity e = *static_cast<ecs::Entity*>(lua_touserdata(L, 1));
        assert(!e.Has<Transform3DComponent>());

        Transform3DComponent c;
        c.position = Pull<glm::vec3>(L, count);
        c.scale = Pull<glm::vec3>(L, count);
        e.Add<Transform3DComponent>(c);
        return 0;
    });

    luaL_dostring(L, R"lua(
        function AddTransform3DComponent(entity, c)
            _AddTransform3DComponent(entity, c.position.x, c.position.y, c.position.z, c.scale.x, c.scale.y, c.scale.z)
        end
    )lua");

    lua_register(L, "HasTransform3DComponent", &Lua_Has<Transform3DComponent>);


    // Functions for ModelComponent =====================================================

    luaL_dostring(L, R"lua(
        ModelComponent = Class(function(self, mesh, material)
            self.mesh = mesh
            self.material = material
        end)
    )lua");

    lua_register(L, "_GetModelComponent", [](lua_State* L) {
        if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }

        ecs::Entity e = *static_cast<ecs::Entity*>(lua_touserdata(L, 1));
        assert(e.Has<ModelComponent>());

        int count = 0;
        const auto& c = e.Get<ModelComponent>();
        count += Push(L, c.mesh);
        count += Push(L, c.material);
        return count;
    });

    luaL_dostring(L, R"lua(
        function GetModelComponent(entity)
            x0, x1 = _GetModelComponent(entity)
            return ModelComponent(x0, x1)
        end
    )lua");

    lua_register(L, "_SetModelComponent", [](lua_State* L) {
        if (!CheckArgCount(L, ModelComponentDimension() + 1)) { return luaL_error(L, "Bad number of args"); }

        int count = 2;
        ecs::Entity e = *static_cast<ecs::Entity*>(lua_touserdata(L, 1));
        auto& c = e.Get<ModelComponent>();
        c.mesh = Pull<std::string>(L, count);
        c.material = Pull<std::string>(L, count);
        return 0;
    });

    luaL_dostring(L, R"lua(
        function SetModelComponent(entity, c)
            _SetModelComponent(entity, c.mesh, c.material)
        end
    )lua");

    lua_register(L, "_AddModelComponent", [](lua_State* L) {
        if (!CheckArgCount(L, ModelComponentDimension() + 1)) { return luaL_error(L, "Bad number of args"); }

        int count = 2;
        ecs::Entity e = *static_cast<ecs::Entity*>(lua_touserdata(L, 1));
        assert(!e.Has<ModelComponent>());

        ModelComponent c;
        c.mesh = Pull<std::string>(L, count);
        c.material = Pull<std::string>(L, count);
        e.Add<ModelComponent>(c);
        return 0;
    });

    luaL_dostring(L, R"lua(
        function AddModelComponent(entity, c)
            _AddModelComponent(entity, c.mesh, c.material)
        end
    )lua");

    lua_register(L, "HasModelComponent", &Lua_Has<ModelComponent>);


    // Functions for RigidBody3DComponent =====================================================

    luaL_dostring(L, R"lua(
        RigidBody3DComponent = Class(function(self, velocity, gravity, frozen, bounciness, frictionCoefficient, rollingResistance, force, onFloor)
            self.velocity = velocity
            self.gravity = gravity
            self.frozen = frozen
            self.bounciness = bounciness
            self.frictionCoefficient = frictionCoefficient
            self.rollingResistance = rollingResistance
            self.force = force
            self.onFloor = onFloor
        end)
    )lua");

    lua_register(L, "_GetRigidBody3DComponent", [](lua_State* L) {
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
    });

    luaL_dostring(L, R"lua(
        function GetRigidBody3DComponent(entity)
            x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11 = _GetRigidBody3DComponent(entity)
            return RigidBody3DComponent(Vec3(x0, x1, x2), x3, x4, x5, x6, x7, Vec3(x8, x9, x10), x11)
        end
    )lua");

    lua_register(L, "_SetRigidBody3DComponent", [](lua_State* L) {
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
    });

    luaL_dostring(L, R"lua(
        function SetRigidBody3DComponent(entity, c)
            _SetRigidBody3DComponent(entity, c.velocity.x, c.velocity.y, c.velocity.z, c.gravity, c.frozen, c.bounciness, c.frictionCoefficient, c.rollingResistance, c.force.x, c.force.y, c.force.z, c.onFloor)
        end
    )lua");

    lua_register(L, "_AddRigidBody3DComponent", [](lua_State* L) {
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
    });

    luaL_dostring(L, R"lua(
        function AddRigidBody3DComponent(entity, c)
            _AddRigidBody3DComponent(entity, c.velocity.x, c.velocity.y, c.velocity.z, c.gravity, c.frozen, c.bounciness, c.frictionCoefficient, c.rollingResistance, c.force.x, c.force.y, c.force.z, c.onFloor)
        end
    )lua");

    lua_register(L, "HasRigidBody3DComponent", &Lua_Has<RigidBody3DComponent>);


    // Functions for BoxCollider3DComponent =====================================================

    luaL_dostring(L, R"lua(
        BoxCollider3DComponent = Class(function(self, position, mass, halfExtents, applyScale)
            self.position = position
            self.mass = mass
            self.halfExtents = halfExtents
            self.applyScale = applyScale
        end)
    )lua");

    lua_register(L, "_GetBoxCollider3DComponent", [](lua_State* L) {
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
    });

    luaL_dostring(L, R"lua(
        function GetBoxCollider3DComponent(entity)
            x0, x1, x2, x3, x4, x5, x6, x7 = _GetBoxCollider3DComponent(entity)
            return BoxCollider3DComponent(Vec3(x0, x1, x2), x3, Vec3(x4, x5, x6), x7)
        end
    )lua");

    lua_register(L, "_SetBoxCollider3DComponent", [](lua_State* L) {
        if (!CheckArgCount(L, BoxCollider3DComponentDimension() + 1)) { return luaL_error(L, "Bad number of args"); }

        int count = 2;
        ecs::Entity e = *static_cast<ecs::Entity*>(lua_touserdata(L, 1));
        auto& c = e.Get<BoxCollider3DComponent>();
        c.position = Pull<glm::vec3>(L, count);
        c.mass = Pull<float>(L, count);
        c.halfExtents = Pull<glm::vec3>(L, count);
        c.applyScale = Pull<bool>(L, count);
        return 0;
    });

    luaL_dostring(L, R"lua(
        function SetBoxCollider3DComponent(entity, c)
            _SetBoxCollider3DComponent(entity, c.position.x, c.position.y, c.position.z, c.mass, c.halfExtents.x, c.halfExtents.y, c.halfExtents.z, c.applyScale)
        end
    )lua");

    lua_register(L, "_AddBoxCollider3DComponent", [](lua_State* L) {
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
    });

    luaL_dostring(L, R"lua(
        function AddBoxCollider3DComponent(entity, c)
            _AddBoxCollider3DComponent(entity, c.position.x, c.position.y, c.position.z, c.mass, c.halfExtents.x, c.halfExtents.y, c.halfExtents.z, c.applyScale)
        end
    )lua");

    lua_register(L, "HasBoxCollider3DComponent", &Lua_Has<BoxCollider3DComponent>);


    // Functions for SphereCollider3DComponent =====================================================

    luaL_dostring(L, R"lua(
        SphereCollider3DComponent = Class(function(self, position, mass, radius)
            self.position = position
            self.mass = mass
            self.radius = radius
        end)
    )lua");

    lua_register(L, "_GetSphereCollider3DComponent", [](lua_State* L) {
        if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }

        ecs::Entity e = *static_cast<ecs::Entity*>(lua_touserdata(L, 1));
        assert(e.Has<SphereCollider3DComponent>());

        int count = 0;
        const auto& c = e.Get<SphereCollider3DComponent>();
        count += Push(L, c.position);
        count += Push(L, c.mass);
        count += Push(L, c.radius);
        return count;
    });

    luaL_dostring(L, R"lua(
        function GetSphereCollider3DComponent(entity)
            x0, x1, x2, x3, x4 = _GetSphereCollider3DComponent(entity)
            return SphereCollider3DComponent(Vec3(x0, x1, x2), x3, x4)
        end
    )lua");

    lua_register(L, "_SetSphereCollider3DComponent", [](lua_State* L) {
        if (!CheckArgCount(L, SphereCollider3DComponentDimension() + 1)) { return luaL_error(L, "Bad number of args"); }

        int count = 2;
        ecs::Entity e = *static_cast<ecs::Entity*>(lua_touserdata(L, 1));
        auto& c = e.Get<SphereCollider3DComponent>();
        c.position = Pull<glm::vec3>(L, count);
        c.mass = Pull<float>(L, count);
        c.radius = Pull<float>(L, count);
        return 0;
    });

    luaL_dostring(L, R"lua(
        function SetSphereCollider3DComponent(entity, c)
            _SetSphereCollider3DComponent(entity, c.position.x, c.position.y, c.position.z, c.mass, c.radius)
        end
    )lua");

    lua_register(L, "_AddSphereCollider3DComponent", [](lua_State* L) {
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
    });

    luaL_dostring(L, R"lua(
        function AddSphereCollider3DComponent(entity, c)
            _AddSphereCollider3DComponent(entity, c.position.x, c.position.y, c.position.z, c.mass, c.radius)
        end
    )lua");

    lua_register(L, "HasSphereCollider3DComponent", &Lua_Has<SphereCollider3DComponent>);


    // Functions for CapsuleCollider3DComponent =====================================================

    luaL_dostring(L, R"lua(
        CapsuleCollider3DComponent = Class(function(self, position, mass, radius, height)
            self.position = position
            self.mass = mass
            self.radius = radius
            self.height = height
        end)
    )lua");

    lua_register(L, "_GetCapsuleCollider3DComponent", [](lua_State* L) {
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
    });

    luaL_dostring(L, R"lua(
        function GetCapsuleCollider3DComponent(entity)
            x0, x1, x2, x3, x4, x5 = _GetCapsuleCollider3DComponent(entity)
            return CapsuleCollider3DComponent(Vec3(x0, x1, x2), x3, x4, x5)
        end
    )lua");

    lua_register(L, "_SetCapsuleCollider3DComponent", [](lua_State* L) {
        if (!CheckArgCount(L, CapsuleCollider3DComponentDimension() + 1)) { return luaL_error(L, "Bad number of args"); }

        int count = 2;
        ecs::Entity e = *static_cast<ecs::Entity*>(lua_touserdata(L, 1));
        auto& c = e.Get<CapsuleCollider3DComponent>();
        c.position = Pull<glm::vec3>(L, count);
        c.mass = Pull<float>(L, count);
        c.radius = Pull<float>(L, count);
        c.height = Pull<float>(L, count);
        return 0;
    });

    luaL_dostring(L, R"lua(
        function SetCapsuleCollider3DComponent(entity, c)
            _SetCapsuleCollider3DComponent(entity, c.position.x, c.position.y, c.position.z, c.mass, c.radius, c.height)
        end
    )lua");

    lua_register(L, "_AddCapsuleCollider3DComponent", [](lua_State* L) {
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
    });

    luaL_dostring(L, R"lua(
        function AddCapsuleCollider3DComponent(entity, c)
            _AddCapsuleCollider3DComponent(entity, c.position.x, c.position.y, c.position.z, c.mass, c.radius, c.height)
        end
    )lua");

    lua_register(L, "HasCapsuleCollider3DComponent", &Lua_Has<CapsuleCollider3DComponent>);


    // Functions for ScriptComponent =====================================================

    luaL_dostring(L, R"lua(
        ScriptComponent = Class(function(self, script, active)
            self.script = script
            self.active = active
        end)
    )lua");

    lua_register(L, "_GetScriptComponent", [](lua_State* L) {
        if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }

        ecs::Entity e = *static_cast<ecs::Entity*>(lua_touserdata(L, 1));
        assert(e.Has<ScriptComponent>());

        int count = 0;
        const auto& c = e.Get<ScriptComponent>();
        count += Push(L, c.script);
        count += Push(L, c.active);
        return count;
    });

    luaL_dostring(L, R"lua(
        function GetScriptComponent(entity)
            x0, x1 = _GetScriptComponent(entity)
            return ScriptComponent(x0, x1)
        end
    )lua");

    lua_register(L, "_SetScriptComponent", [](lua_State* L) {
        if (!CheckArgCount(L, ScriptComponentDimension() + 1)) { return luaL_error(L, "Bad number of args"); }

        int count = 2;
        ecs::Entity e = *static_cast<ecs::Entity*>(lua_touserdata(L, 1));
        auto& c = e.Get<ScriptComponent>();
        c.script = Pull<std::string>(L, count);
        c.active = Pull<bool>(L, count);
        return 0;
    });

    luaL_dostring(L, R"lua(
        function SetScriptComponent(entity, c)
            _SetScriptComponent(entity, c.script, c.active)
        end
    )lua");

    lua_register(L, "_AddScriptComponent", [](lua_State* L) {
        if (!CheckArgCount(L, ScriptComponentDimension() + 1)) { return luaL_error(L, "Bad number of args"); }

        int count = 2;
        ecs::Entity e = *static_cast<ecs::Entity*>(lua_touserdata(L, 1));
        assert(!e.Has<ScriptComponent>());

        ScriptComponent c;
        c.script = Pull<std::string>(L, count);
        c.active = Pull<bool>(L, count);
        e.Add<ScriptComponent>(c);
        return 0;
    });

    luaL_dostring(L, R"lua(
        function AddScriptComponent(entity, c)
            _AddScriptComponent(entity, c.script, c.active)
        end
    )lua");

    lua_register(L, "HasScriptComponent", &Lua_Has<ScriptComponent>);


    // Functions for Camera3DComponent =====================================================

    luaL_dostring(L, R"lua(
        Camera3DComponent = Class(function(self, fov, pitch)
            self.fov = fov
            self.pitch = pitch
        end)
    )lua");

    lua_register(L, "_GetCamera3DComponent", [](lua_State* L) {
        if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }

        ecs::Entity e = *static_cast<ecs::Entity*>(lua_touserdata(L, 1));
        assert(e.Has<Camera3DComponent>());

        int count = 0;
        const auto& c = e.Get<Camera3DComponent>();
        count += Push(L, c.fov);
        count += Push(L, c.pitch);
        return count;
    });

    luaL_dostring(L, R"lua(
        function GetCamera3DComponent(entity)
            x0, x1 = _GetCamera3DComponent(entity)
            return Camera3DComponent(x0, x1)
        end
    )lua");

    lua_register(L, "_SetCamera3DComponent", [](lua_State* L) {
        if (!CheckArgCount(L, Camera3DComponentDimension() + 1)) { return luaL_error(L, "Bad number of args"); }

        int count = 2;
        ecs::Entity e = *static_cast<ecs::Entity*>(lua_touserdata(L, 1));
        auto& c = e.Get<Camera3DComponent>();
        c.fov = Pull<float>(L, count);
        c.pitch = Pull<float>(L, count);
        return 0;
    });

    luaL_dostring(L, R"lua(
        function SetCamera3DComponent(entity, c)
            _SetCamera3DComponent(entity, c.fov, c.pitch)
        end
    )lua");

    lua_register(L, "_AddCamera3DComponent", [](lua_State* L) {
        if (!CheckArgCount(L, Camera3DComponentDimension() + 1)) { return luaL_error(L, "Bad number of args"); }

        int count = 2;
        ecs::Entity e = *static_cast<ecs::Entity*>(lua_touserdata(L, 1));
        assert(!e.Has<Camera3DComponent>());

        Camera3DComponent c;
        c.fov = Pull<float>(L, count);
        c.pitch = Pull<float>(L, count);
        e.Add<Camera3DComponent>(c);
        return 0;
    });

    luaL_dostring(L, R"lua(
        function AddCamera3DComponent(entity, c)
            _AddCamera3DComponent(entity, c.fov, c.pitch)
        end
    )lua");

    lua_register(L, "HasCamera3DComponent", &Lua_Has<Camera3DComponent>);


    // Functions for SelectComponent =====================================================

    luaL_dostring(L, R"lua(
        SelectComponent = Class(function(self, selected, hovered)
            self.selected = selected
            self.hovered = hovered
        end)
    )lua");

    lua_register(L, "_GetSelectComponent", [](lua_State* L) {
        if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }

        ecs::Entity e = *static_cast<ecs::Entity*>(lua_touserdata(L, 1));
        assert(e.Has<SelectComponent>());

        int count = 0;
        const auto& c = e.Get<SelectComponent>();
        count += Push(L, c.selected);
        count += Push(L, c.hovered);
        return count;
    });

    luaL_dostring(L, R"lua(
        function GetSelectComponent(entity)
            x0, x1 = _GetSelectComponent(entity)
            return SelectComponent(x0, x1)
        end
    )lua");

    lua_register(L, "_SetSelectComponent", [](lua_State* L) {
        if (!CheckArgCount(L, SelectComponentDimension() + 1)) { return luaL_error(L, "Bad number of args"); }

        int count = 2;
        ecs::Entity e = *static_cast<ecs::Entity*>(lua_touserdata(L, 1));
        auto& c = e.Get<SelectComponent>();
        c.selected = Pull<bool>(L, count);
        c.hovered = Pull<bool>(L, count);
        return 0;
    });

    luaL_dostring(L, R"lua(
        function SetSelectComponent(entity, c)
            _SetSelectComponent(entity, c.selected, c.hovered)
        end
    )lua");

    lua_register(L, "_AddSelectComponent", [](lua_State* L) {
        if (!CheckArgCount(L, SelectComponentDimension() + 1)) { return luaL_error(L, "Bad number of args"); }

        int count = 2;
        ecs::Entity e = *static_cast<ecs::Entity*>(lua_touserdata(L, 1));
        assert(!e.Has<SelectComponent>());

        SelectComponent c;
        c.selected = Pull<bool>(L, count);
        c.hovered = Pull<bool>(L, count);
        e.Add<SelectComponent>(c);
        return 0;
    });

    luaL_dostring(L, R"lua(
        function AddSelectComponent(entity, c)
            _AddSelectComponent(entity, c.selected, c.hovered)
        end
    )lua");

    lua_register(L, "HasSelectComponent", &Lua_Has<SelectComponent>);


    // Functions for PathComponent =====================================================

    luaL_dostring(L, R"lua(
        PathComponent = Class(function(self, speed)
            self.speed = speed
        end)
    )lua");

    lua_register(L, "_GetPathComponent", [](lua_State* L) {
        if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }

        ecs::Entity e = *static_cast<ecs::Entity*>(lua_touserdata(L, 1));
        assert(e.Has<PathComponent>());

        int count = 0;
        const auto& c = e.Get<PathComponent>();
        count += Push(L, c.speed);
        return count;
    });

    luaL_dostring(L, R"lua(
        function GetPathComponent(entity)
            x0 = _GetPathComponent(entity)
            return PathComponent(x0)
        end
    )lua");

    lua_register(L, "_SetPathComponent", [](lua_State* L) {
        if (!CheckArgCount(L, PathComponentDimension() + 1)) { return luaL_error(L, "Bad number of args"); }

        int count = 2;
        ecs::Entity e = *static_cast<ecs::Entity*>(lua_touserdata(L, 1));
        auto& c = e.Get<PathComponent>();
        c.speed = Pull<float>(L, count);
        return 0;
    });

    luaL_dostring(L, R"lua(
        function SetPathComponent(entity, c)
            _SetPathComponent(entity, c.speed)
        end
    )lua");

    lua_register(L, "_AddPathComponent", [](lua_State* L) {
        if (!CheckArgCount(L, PathComponentDimension() + 1)) { return luaL_error(L, "Bad number of args"); }

        int count = 2;
        ecs::Entity e = *static_cast<ecs::Entity*>(lua_touserdata(L, 1));
        assert(!e.Has<PathComponent>());

        PathComponent c;
        c.speed = Pull<float>(L, count);
        e.Add<PathComponent>(c);
        return 0;
    });

    luaL_dostring(L, R"lua(
        function AddPathComponent(entity, c)
            _AddPathComponent(entity, c.speed)
        end
    )lua");

    lua_register(L, "HasPathComponent", &Lua_Has<PathComponent>);


    // Functions for GridComponent =====================================================

    luaL_dostring(L, R"lua(
        GridComponent = Class(function(self, x, z)
            self.x = x
            self.z = z
        end)
    )lua");

    lua_register(L, "_GetGridComponent", [](lua_State* L) {
        if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }

        ecs::Entity e = *static_cast<ecs::Entity*>(lua_touserdata(L, 1));
        assert(e.Has<GridComponent>());

        int count = 0;
        const auto& c = e.Get<GridComponent>();
        count += Push(L, c.x);
        count += Push(L, c.z);
        return count;
    });

    luaL_dostring(L, R"lua(
        function GetGridComponent(entity)
            x0, x1 = _GetGridComponent(entity)
            return GridComponent(x0, x1)
        end
    )lua");

    lua_register(L, "_SetGridComponent", [](lua_State* L) {
        if (!CheckArgCount(L, GridComponentDimension() + 1)) { return luaL_error(L, "Bad number of args"); }

        int count = 2;
        ecs::Entity e = *static_cast<ecs::Entity*>(lua_touserdata(L, 1));
        auto& c = e.Get<GridComponent>();
        c.x = Pull<int>(L, count);
        c.z = Pull<int>(L, count);
        return 0;
    });

    luaL_dostring(L, R"lua(
        function SetGridComponent(entity, c)
            _SetGridComponent(entity, c.x, c.z)
        end
    )lua");

    lua_register(L, "_AddGridComponent", [](lua_State* L) {
        if (!CheckArgCount(L, GridComponentDimension() + 1)) { return luaL_error(L, "Bad number of args"); }

        int count = 2;
        ecs::Entity e = *static_cast<ecs::Entity*>(lua_touserdata(L, 1));
        assert(!e.Has<GridComponent>());

        GridComponent c;
        c.x = Pull<int>(L, count);
        c.z = Pull<int>(L, count);
        e.Add<GridComponent>(c);
        return 0;
    });

    luaL_dostring(L, R"lua(
        function AddGridComponent(entity, c)
            _AddGridComponent(entity, c.x, c.z)
        end
    )lua");

    lua_register(L, "HasGridComponent", &Lua_Has<GridComponent>);


    // Functions for LightComponent =====================================================

    luaL_dostring(L, R"lua(
        LightComponent = Class(function(self, colour, brightness)
            self.colour = colour
            self.brightness = brightness
        end)
    )lua");

    lua_register(L, "_GetLightComponent", [](lua_State* L) {
        if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }

        ecs::Entity e = *static_cast<ecs::Entity*>(lua_touserdata(L, 1));
        assert(e.Has<LightComponent>());

        int count = 0;
        const auto& c = e.Get<LightComponent>();
        count += Push(L, c.colour);
        count += Push(L, c.brightness);
        return count;
    });

    luaL_dostring(L, R"lua(
        function GetLightComponent(entity)
            x0, x1, x2, x3 = _GetLightComponent(entity)
            return LightComponent(Vec3(x0, x1, x2), x3)
        end
    )lua");

    lua_register(L, "_SetLightComponent", [](lua_State* L) {
        if (!CheckArgCount(L, LightComponentDimension() + 1)) { return luaL_error(L, "Bad number of args"); }

        int count = 2;
        ecs::Entity e = *static_cast<ecs::Entity*>(lua_touserdata(L, 1));
        auto& c = e.Get<LightComponent>();
        c.colour = Pull<glm::vec3>(L, count);
        c.brightness = Pull<float>(L, count);
        return 0;
    });

    luaL_dostring(L, R"lua(
        function SetLightComponent(entity, c)
            _SetLightComponent(entity, c.colour.x, c.colour.y, c.colour.z, c.brightness)
        end
    )lua");

    lua_register(L, "_AddLightComponent", [](lua_State* L) {
        if (!CheckArgCount(L, LightComponentDimension() + 1)) { return luaL_error(L, "Bad number of args"); }

        int count = 2;
        ecs::Entity e = *static_cast<ecs::Entity*>(lua_touserdata(L, 1));
        assert(!e.Has<LightComponent>());

        LightComponent c;
        c.colour = Pull<glm::vec3>(L, count);
        c.brightness = Pull<float>(L, count);
        e.Add<LightComponent>(c);
        return 0;
    });

    luaL_dostring(L, R"lua(
        function AddLightComponent(entity, c)
            _AddLightComponent(entity, c.colour.x, c.colour.y, c.colour.z, c.brightness)
        end
    )lua");

    lua_register(L, "HasLightComponent", &Lua_Has<LightComponent>);


    // Functions for SunComponent =====================================================

    luaL_dostring(L, R"lua(
        SunComponent = Class(function(self, colour, brightness, direction, shadows)
            self.colour = colour
            self.brightness = brightness
            self.direction = direction
            self.shadows = shadows
        end)
    )lua");

    lua_register(L, "_GetSunComponent", [](lua_State* L) {
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
    });

    luaL_dostring(L, R"lua(
        function GetSunComponent(entity)
            x0, x1, x2, x3, x4, x5, x6, x7 = _GetSunComponent(entity)
            return SunComponent(Vec3(x0, x1, x2), x3, Vec3(x4, x5, x6), x7)
        end
    )lua");

    lua_register(L, "_SetSunComponent", [](lua_State* L) {
        if (!CheckArgCount(L, SunComponentDimension() + 1)) { return luaL_error(L, "Bad number of args"); }

        int count = 2;
        ecs::Entity e = *static_cast<ecs::Entity*>(lua_touserdata(L, 1));
        auto& c = e.Get<SunComponent>();
        c.colour = Pull<glm::vec3>(L, count);
        c.brightness = Pull<float>(L, count);
        c.direction = Pull<glm::vec3>(L, count);
        c.shadows = Pull<bool>(L, count);
        return 0;
    });

    luaL_dostring(L, R"lua(
        function SetSunComponent(entity, c)
            _SetSunComponent(entity, c.colour.x, c.colour.y, c.colour.z, c.brightness, c.direction.x, c.direction.y, c.direction.z, c.shadows)
        end
    )lua");

    lua_register(L, "_AddSunComponent", [](lua_State* L) {
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
    });

    luaL_dostring(L, R"lua(
        function AddSunComponent(entity, c)
            _AddSunComponent(entity, c.colour.x, c.colour.y, c.colour.z, c.brightness, c.direction.x, c.direction.y, c.direction.z, c.shadows)
        end
    )lua");

    lua_register(L, "HasSunComponent", &Lua_Has<SunComponent>);


    // Functions for AmbienceComponent =====================================================

    luaL_dostring(L, R"lua(
        AmbienceComponent = Class(function(self, colour, brightness)
            self.colour = colour
            self.brightness = brightness
        end)
    )lua");

    lua_register(L, "_GetAmbienceComponent", [](lua_State* L) {
        if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }

        ecs::Entity e = *static_cast<ecs::Entity*>(lua_touserdata(L, 1));
        assert(e.Has<AmbienceComponent>());

        int count = 0;
        const auto& c = e.Get<AmbienceComponent>();
        count += Push(L, c.colour);
        count += Push(L, c.brightness);
        return count;
    });

    luaL_dostring(L, R"lua(
        function GetAmbienceComponent(entity)
            x0, x1, x2, x3 = _GetAmbienceComponent(entity)
            return AmbienceComponent(Vec3(x0, x1, x2), x3)
        end
    )lua");

    lua_register(L, "_SetAmbienceComponent", [](lua_State* L) {
        if (!CheckArgCount(L, AmbienceComponentDimension() + 1)) { return luaL_error(L, "Bad number of args"); }

        int count = 2;
        ecs::Entity e = *static_cast<ecs::Entity*>(lua_touserdata(L, 1));
        auto& c = e.Get<AmbienceComponent>();
        c.colour = Pull<glm::vec3>(L, count);
        c.brightness = Pull<float>(L, count);
        return 0;
    });

    luaL_dostring(L, R"lua(
        function SetAmbienceComponent(entity, c)
            _SetAmbienceComponent(entity, c.colour.x, c.colour.y, c.colour.z, c.brightness)
        end
    )lua");

    lua_register(L, "_AddAmbienceComponent", [](lua_State* L) {
        if (!CheckArgCount(L, AmbienceComponentDimension() + 1)) { return luaL_error(L, "Bad number of args"); }

        int count = 2;
        ecs::Entity e = *static_cast<ecs::Entity*>(lua_touserdata(L, 1));
        assert(!e.Has<AmbienceComponent>());

        AmbienceComponent c;
        c.colour = Pull<glm::vec3>(L, count);
        c.brightness = Pull<float>(L, count);
        e.Add<AmbienceComponent>(c);
        return 0;
    });

    luaL_dostring(L, R"lua(
        function AddAmbienceComponent(entity, c)
            _AddAmbienceComponent(entity, c.colour.x, c.colour.y, c.colour.z, c.brightness)
        end
    )lua");

    lua_register(L, "HasAmbienceComponent", &Lua_Has<AmbienceComponent>);


    // Functions for ParticleComponent =====================================================

    luaL_dostring(L, R"lua(
        ParticleComponent = Class(function(self, interval, velocity, velocityNoise, acceleration, scale, life)
            self.interval = interval
            self.velocity = velocity
            self.velocityNoise = velocityNoise
            self.acceleration = acceleration
            self.scale = scale
            self.life = life
        end)
    )lua");

    lua_register(L, "_GetParticleComponent", [](lua_State* L) {
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
    });

    luaL_dostring(L, R"lua(
        function GetParticleComponent(entity)
            x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11 = _GetParticleComponent(entity)
            return ParticleComponent(x0, Vec3(x1, x2, x3), x4, Vec3(x5, x6, x7), Vec3(x8, x9, x10), x11)
        end
    )lua");

    lua_register(L, "_SetParticleComponent", [](lua_State* L) {
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
    });

    luaL_dostring(L, R"lua(
        function SetParticleComponent(entity, c)
            _SetParticleComponent(entity, c.interval, c.velocity.x, c.velocity.y, c.velocity.z, c.velocityNoise, c.acceleration.x, c.acceleration.y, c.acceleration.z, c.scale.x, c.scale.y, c.scale.z, c.life)
        end
    )lua");

    lua_register(L, "_AddParticleComponent", [](lua_State* L) {
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
    });

    luaL_dostring(L, R"lua(
        function AddParticleComponent(entity, c)
            _AddParticleComponent(entity, c.interval, c.velocity.x, c.velocity.y, c.velocity.z, c.velocityNoise, c.acceleration.x, c.acceleration.y, c.acceleration.z, c.scale.x, c.scale.y, c.scale.z, c.life)
        end
    )lua");

    lua_register(L, "HasParticleComponent", &Lua_Has<ParticleComponent>);


    // Functions for MeshAnimationComponent =====================================================

    luaL_dostring(L, R"lua(
        MeshAnimationComponent = Class(function(self, name, time, speed)
            self.name = name
            self.time = time
            self.speed = speed
        end)
    )lua");

    lua_register(L, "_GetMeshAnimationComponent", [](lua_State* L) {
        if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }

        ecs::Entity e = *static_cast<ecs::Entity*>(lua_touserdata(L, 1));
        assert(e.Has<MeshAnimationComponent>());

        int count = 0;
        const auto& c = e.Get<MeshAnimationComponent>();
        count += Push(L, c.name);
        count += Push(L, c.time);
        count += Push(L, c.speed);
        return count;
    });

    luaL_dostring(L, R"lua(
        function GetMeshAnimationComponent(entity)
            x0, x1, x2 = _GetMeshAnimationComponent(entity)
            return MeshAnimationComponent(x0, x1, x2)
        end
    )lua");

    lua_register(L, "_SetMeshAnimationComponent", [](lua_State* L) {
        if (!CheckArgCount(L, MeshAnimationComponentDimension() + 1)) { return luaL_error(L, "Bad number of args"); }

        int count = 2;
        ecs::Entity e = *static_cast<ecs::Entity*>(lua_touserdata(L, 1));
        auto& c = e.Get<MeshAnimationComponent>();
        c.name = Pull<std::string>(L, count);
        c.time = Pull<float>(L, count);
        c.speed = Pull<float>(L, count);
        return 0;
    });

    luaL_dostring(L, R"lua(
        function SetMeshAnimationComponent(entity, c)
            _SetMeshAnimationComponent(entity, c.name, c.time, c.speed)
        end
    )lua");

    lua_register(L, "_AddMeshAnimationComponent", [](lua_State* L) {
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
    });

    luaL_dostring(L, R"lua(
        function AddMeshAnimationComponent(entity, c)
            _AddMeshAnimationComponent(entity, c.name, c.time, c.speed)
        end
    )lua");

    lua_register(L, "HasMeshAnimationComponent", &Lua_Has<MeshAnimationComponent>);


}

}
}
