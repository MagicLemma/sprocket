// GENERATED FILE
#include "LuaLibrary.h"
#include "LuaScript.h"
#include "LuaConverter.h"
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

template <typename T> int _has_impl(lua_State* L)
{
    if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }
    ecs::Entity entity = *static_cast<ecs::Entity*>(lua_touserdata(L, 1));
    lua_pushboolean(L, entity.Has<T>());
    return 1;
}

}

void load_registry_functions(lua::Script& script, ecs::Registry& registry)
{
    lua_State* L = script.native_handle();
    script.set_value("__registry__", &registry);

    // Add functions for creating and destroying entities.
    lua_register(L, "NewEntity", [](lua_State* L) {
        if (!CheckArgCount(L, 0)) { return luaL_error(L, "Bad number of args"); }
        auto luaEntity = static_cast<ecs::Entity*>(lua_newuserdata(L, sizeof(ecs::Entity)));
        *luaEntity = get_pointer<ecs::Registry>(L, "__registry__")->New();
        return 1;
    });

    lua_register(L, "DeleteEntity", [](lua_State* L) {
        if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }
        auto luaEntity = static_cast<ecs::Entity*>(lua_touserdata(L, 1));
        luaEntity->destroy();
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
        auto gen = new Generator(get_pointer<ecs::Registry>(L, "__registry__")->Each());
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

void load_input_functions(lua::Script& script, InputProxy& input)
{
    lua_State* L = script.native_handle();
    script.set_value("__input__", &input);

    lua_register(L, "IsKeyDown", [](lua_State* L) {
        if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }

        if (auto ip = get_pointer<InputProxy>(L, "__input__"); ip) {
            int x = (int)lua_tointeger(L, 1);
            lua_pushboolean(L, ip->is_keyboard_down(x));
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
            lua_pushboolean(L, ip->is_mouse_down(x));
        }
        else {
            lua_pushboolean(L, false);
        }

        return 1;
    });
}

void load_window_functions(lua::Script& script, Window& window)
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

void load_entity_transformation_functions(lua::Script& script)
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

        return Converter<glm::vec3>::push(L, Maths::Forwards(o));
    });

    luaL_dostring(L, R"lua(
        function GetForwardsDir(entity)
            local x, y, z = _GetForwardsDir(entity)
            return Vec3(x, y, z)
        end
    )lua");

    lua_register(L, "_GetRightDir", [](lua_State* L) {
        if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }
        int ptr = 1;
        ecs::Entity entity = Converter<ecs::Entity>::read(L, ptr);
        auto& tr = entity.Get<Transform3DComponent>();
        return Converter<glm::vec3>::push(L, Maths::Right(tr.orientation));
    });

    luaL_dostring(L, R"lua(
        function GetRightDir(entity)
            local x, y, z = _GetRightDir(entity)
            return Vec3(x, y, z)
        end
    )lua");

    lua_register(L, "MakeUpright", [](lua_State* L) {
        if (!CheckArgCount(L, 2)) { return luaL_error(L, "Bad number of args"); }
        int ptr = 1;
        ecs::Entity entity = Converter<ecs::Entity>::read(L, ptr);
        auto& tr = entity.Get<Transform3DComponent>();
        float yaw = Converter<float>::read(L, ptr);
        tr.orientation = glm::quat(glm::vec3(0, yaw, 0));
        return 0;
    });

    lua_register(L, "AreEntitiesEqual", [](lua_State* L) {
        if (!CheckArgCount(L, 2)) { return luaL_error(L, "Bad number of args"); }
        int ptr = 1;
        ecs::Entity entity1 = Converter<ecs::Entity>::read(L, ptr);
        ecs::Entity entity2 = Converter<ecs::Entity>::read(L, ptr);
        return Converter<bool>::push(L, entity1 == entity2);
    });
}

// COMPONENT RELATED CODE - GENERATED BY DATAMATIC
void load_entity_component_functions(lua::Script& script)
{
    lua_State* L = script.native_handle();

    // Functions for NameComponent =====================================================

    constexpr int NameComponent_dimension = 1;

    luaL_dostring(L, R"lua(
        NameComponent = Class(function(self, name)
            self.name = name
        end)
    )lua");

    lua_register(L, "_GetNameComponent", [](lua_State* L) {
        if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }

        int ptr = 1;
        ecs::Entity e = Converter<ecs::Entity>::read(L, ptr);
        assert(e.Has<NameComponent>());

        int count = 0;
        const auto& c = e.Get<NameComponent>();
        count += Converter<std::string>::push(L, c.name);
        assert(count == NameComponent_dimension);
        return count;
    });

    luaL_dostring(L, R"lua(
        function GetNameComponent(entity)
            x0 = _GetNameComponent(entity)
            return NameComponent(x0)
        end
    )lua");

    lua_register(L, "_SetNameComponent", [](lua_State* L) {
        if (!CheckArgCount(L, NameComponent_dimension + 1)) { return luaL_error(L, "Bad number of args"); }

        int ptr = 1;
        ecs::Entity e = Converter<ecs::Entity>::read(L, ptr);
        auto& c = e.Get<NameComponent>();
        c.name = Converter<std::string>::read(L, ptr);
        assert(ptr == NameComponent_dimension + 2);
        return 0;
    });

    luaL_dostring(L, R"lua(
        function SetNameComponent(entity, c)
            _SetNameComponent(entity, c.name)
        end
    )lua");

    lua_register(L, "_AddNameComponent", [](lua_State* L) {
        if (!CheckArgCount(L, NameComponent_dimension + 1)) { return luaL_error(L, "Bad number of args"); }

        int ptr = 1;
        ecs::Entity e = Converter<ecs::Entity>::read(L, ptr);
        assert(!e.Has<NameComponent>());

        NameComponent c;
        c.name = Converter<std::string>::read(L, ptr);
        e.Add<NameComponent>(c);
        assert(ptr == NameComponent_dimension + 2);
        return 0;
    });

    luaL_dostring(L, R"lua(
        function AddNameComponent(entity, c)
            _AddNameComponent(entity, c.name)
        end
    )lua");

    lua_register(L, "HasNameComponent", &_has_impl<NameComponent>);


    // Functions for Transform2DComponent =====================================================

    constexpr int Transform2DComponent_dimension = 5;

    luaL_dostring(L, R"lua(
        Transform2DComponent = Class(function(self, position, rotation, scale)
            self.position = position
            self.rotation = rotation
            self.scale = scale
        end)
    )lua");

    lua_register(L, "_GetTransform2DComponent", [](lua_State* L) {
        if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }

        int ptr = 1;
        ecs::Entity e = Converter<ecs::Entity>::read(L, ptr);
        assert(e.Has<Transform2DComponent>());

        int count = 0;
        const auto& c = e.Get<Transform2DComponent>();
        count += Converter<glm::vec2>::push(L, c.position);
        count += Converter<float>::push(L, c.rotation);
        count += Converter<glm::vec2>::push(L, c.scale);
        assert(count == Transform2DComponent_dimension);
        return count;
    });

    luaL_dostring(L, R"lua(
        function GetTransform2DComponent(entity)
            x0, x1, x2, x3, x4 = _GetTransform2DComponent(entity)
            return Transform2DComponent(Vec3(x0, x1), x2, Vec3(x3, x4))
        end
    )lua");

    lua_register(L, "_SetTransform2DComponent", [](lua_State* L) {
        if (!CheckArgCount(L, Transform2DComponent_dimension + 1)) { return luaL_error(L, "Bad number of args"); }

        int ptr = 1;
        ecs::Entity e = Converter<ecs::Entity>::read(L, ptr);
        auto& c = e.Get<Transform2DComponent>();
        c.position = Converter<glm::vec2>::read(L, ptr);
        c.rotation = Converter<float>::read(L, ptr);
        c.scale = Converter<glm::vec2>::read(L, ptr);
        assert(ptr == Transform2DComponent_dimension + 2);
        return 0;
    });

    luaL_dostring(L, R"lua(
        function SetTransform2DComponent(entity, c)
            _SetTransform2DComponent(entity, c.position, c.rotation, c.scale)
        end
    )lua");

    lua_register(L, "_AddTransform2DComponent", [](lua_State* L) {
        if (!CheckArgCount(L, Transform2DComponent_dimension + 1)) { return luaL_error(L, "Bad number of args"); }

        int ptr = 1;
        ecs::Entity e = Converter<ecs::Entity>::read(L, ptr);
        assert(!e.Has<Transform2DComponent>());

        Transform2DComponent c;
        c.position = Converter<glm::vec2>::read(L, ptr);
        c.rotation = Converter<float>::read(L, ptr);
        c.scale = Converter<glm::vec2>::read(L, ptr);
        e.Add<Transform2DComponent>(c);
        assert(ptr == Transform2DComponent_dimension + 2);
        return 0;
    });

    luaL_dostring(L, R"lua(
        function AddTransform2DComponent(entity, c)
            _AddTransform2DComponent(entity, c.position, c.rotation, c.scale)
        end
    )lua");

    lua_register(L, "HasTransform2DComponent", &_has_impl<Transform2DComponent>);


    // Functions for Transform3DComponent =====================================================

    constexpr int Transform3DComponent_dimension = 6;

    luaL_dostring(L, R"lua(
        Transform3DComponent = Class(function(self, position, scale)
            self.position = position
            self.scale = scale
        end)
    )lua");

    lua_register(L, "_GetTransform3DComponent", [](lua_State* L) {
        if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }

        int ptr = 1;
        ecs::Entity e = Converter<ecs::Entity>::read(L, ptr);
        assert(e.Has<Transform3DComponent>());

        int count = 0;
        const auto& c = e.Get<Transform3DComponent>();
        count += Converter<glm::vec3>::push(L, c.position);
        count += Converter<glm::vec3>::push(L, c.scale);
        assert(count == Transform3DComponent_dimension);
        return count;
    });

    luaL_dostring(L, R"lua(
        function GetTransform3DComponent(entity)
            x0, x1, x2, x3, x4, x5 = _GetTransform3DComponent(entity)
            return Transform3DComponent(Vec3(x0, x1, x2), Vec3(x3, x4, x5))
        end
    )lua");

    lua_register(L, "_SetTransform3DComponent", [](lua_State* L) {
        if (!CheckArgCount(L, Transform3DComponent_dimension + 1)) { return luaL_error(L, "Bad number of args"); }

        int ptr = 1;
        ecs::Entity e = Converter<ecs::Entity>::read(L, ptr);
        auto& c = e.Get<Transform3DComponent>();
        c.position = Converter<glm::vec3>::read(L, ptr);
        c.scale = Converter<glm::vec3>::read(L, ptr);
        assert(ptr == Transform3DComponent_dimension + 2);
        return 0;
    });

    luaL_dostring(L, R"lua(
        function SetTransform3DComponent(entity, c)
            _SetTransform3DComponent(entity, c.position.x, c.position.y, c.position.z, c.scale.x, c.scale.y, c.scale.z)
        end
    )lua");

    lua_register(L, "_AddTransform3DComponent", [](lua_State* L) {
        if (!CheckArgCount(L, Transform3DComponent_dimension + 1)) { return luaL_error(L, "Bad number of args"); }

        int ptr = 1;
        ecs::Entity e = Converter<ecs::Entity>::read(L, ptr);
        assert(!e.Has<Transform3DComponent>());

        Transform3DComponent c;
        c.position = Converter<glm::vec3>::read(L, ptr);
        c.scale = Converter<glm::vec3>::read(L, ptr);
        e.Add<Transform3DComponent>(c);
        assert(ptr == Transform3DComponent_dimension + 2);
        return 0;
    });

    luaL_dostring(L, R"lua(
        function AddTransform3DComponent(entity, c)
            _AddTransform3DComponent(entity, c.position.x, c.position.y, c.position.z, c.scale.x, c.scale.y, c.scale.z)
        end
    )lua");

    lua_register(L, "HasTransform3DComponent", &_has_impl<Transform3DComponent>);


    // Functions for ModelComponent =====================================================

    constexpr int ModelComponent_dimension = 2;

    luaL_dostring(L, R"lua(
        ModelComponent = Class(function(self, mesh, material)
            self.mesh = mesh
            self.material = material
        end)
    )lua");

    lua_register(L, "_GetModelComponent", [](lua_State* L) {
        if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }

        int ptr = 1;
        ecs::Entity e = Converter<ecs::Entity>::read(L, ptr);
        assert(e.Has<ModelComponent>());

        int count = 0;
        const auto& c = e.Get<ModelComponent>();
        count += Converter<std::string>::push(L, c.mesh);
        count += Converter<std::string>::push(L, c.material);
        assert(count == ModelComponent_dimension);
        return count;
    });

    luaL_dostring(L, R"lua(
        function GetModelComponent(entity)
            x0, x1 = _GetModelComponent(entity)
            return ModelComponent(x0, x1)
        end
    )lua");

    lua_register(L, "_SetModelComponent", [](lua_State* L) {
        if (!CheckArgCount(L, ModelComponent_dimension + 1)) { return luaL_error(L, "Bad number of args"); }

        int ptr = 1;
        ecs::Entity e = Converter<ecs::Entity>::read(L, ptr);
        auto& c = e.Get<ModelComponent>();
        c.mesh = Converter<std::string>::read(L, ptr);
        c.material = Converter<std::string>::read(L, ptr);
        assert(ptr == ModelComponent_dimension + 2);
        return 0;
    });

    luaL_dostring(L, R"lua(
        function SetModelComponent(entity, c)
            _SetModelComponent(entity, c.mesh, c.material)
        end
    )lua");

    lua_register(L, "_AddModelComponent", [](lua_State* L) {
        if (!CheckArgCount(L, ModelComponent_dimension + 1)) { return luaL_error(L, "Bad number of args"); }

        int ptr = 1;
        ecs::Entity e = Converter<ecs::Entity>::read(L, ptr);
        assert(!e.Has<ModelComponent>());

        ModelComponent c;
        c.mesh = Converter<std::string>::read(L, ptr);
        c.material = Converter<std::string>::read(L, ptr);
        e.Add<ModelComponent>(c);
        assert(ptr == ModelComponent_dimension + 2);
        return 0;
    });

    luaL_dostring(L, R"lua(
        function AddModelComponent(entity, c)
            _AddModelComponent(entity, c.mesh, c.material)
        end
    )lua");

    lua_register(L, "HasModelComponent", &_has_impl<ModelComponent>);


    // Functions for RigidBody3DComponent =====================================================

    constexpr int RigidBody3DComponent_dimension = 12;

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

        int ptr = 1;
        ecs::Entity e = Converter<ecs::Entity>::read(L, ptr);
        assert(e.Has<RigidBody3DComponent>());

        int count = 0;
        const auto& c = e.Get<RigidBody3DComponent>();
        count += Converter<glm::vec3>::push(L, c.velocity);
        count += Converter<bool>::push(L, c.gravity);
        count += Converter<bool>::push(L, c.frozen);
        count += Converter<float>::push(L, c.bounciness);
        count += Converter<float>::push(L, c.frictionCoefficient);
        count += Converter<float>::push(L, c.rollingResistance);
        count += Converter<glm::vec3>::push(L, c.force);
        count += Converter<bool>::push(L, c.onFloor);
        assert(count == RigidBody3DComponent_dimension);
        return count;
    });

    luaL_dostring(L, R"lua(
        function GetRigidBody3DComponent(entity)
            x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11 = _GetRigidBody3DComponent(entity)
            return RigidBody3DComponent(Vec3(x0, x1, x2), x3, x4, x5, x6, x7, Vec3(x8, x9, x10), x11)
        end
    )lua");

    lua_register(L, "_SetRigidBody3DComponent", [](lua_State* L) {
        if (!CheckArgCount(L, RigidBody3DComponent_dimension + 1)) { return luaL_error(L, "Bad number of args"); }

        int ptr = 1;
        ecs::Entity e = Converter<ecs::Entity>::read(L, ptr);
        auto& c = e.Get<RigidBody3DComponent>();
        c.velocity = Converter<glm::vec3>::read(L, ptr);
        c.gravity = Converter<bool>::read(L, ptr);
        c.frozen = Converter<bool>::read(L, ptr);
        c.bounciness = Converter<float>::read(L, ptr);
        c.frictionCoefficient = Converter<float>::read(L, ptr);
        c.rollingResistance = Converter<float>::read(L, ptr);
        c.force = Converter<glm::vec3>::read(L, ptr);
        c.onFloor = Converter<bool>::read(L, ptr);
        assert(ptr == RigidBody3DComponent_dimension + 2);
        return 0;
    });

    luaL_dostring(L, R"lua(
        function SetRigidBody3DComponent(entity, c)
            _SetRigidBody3DComponent(entity, c.velocity.x, c.velocity.y, c.velocity.z, c.gravity, c.frozen, c.bounciness, c.frictionCoefficient, c.rollingResistance, c.force.x, c.force.y, c.force.z, c.onFloor)
        end
    )lua");

    lua_register(L, "_AddRigidBody3DComponent", [](lua_State* L) {
        if (!CheckArgCount(L, RigidBody3DComponent_dimension + 1)) { return luaL_error(L, "Bad number of args"); }

        int ptr = 1;
        ecs::Entity e = Converter<ecs::Entity>::read(L, ptr);
        assert(!e.Has<RigidBody3DComponent>());

        RigidBody3DComponent c;
        c.velocity = Converter<glm::vec3>::read(L, ptr);
        c.gravity = Converter<bool>::read(L, ptr);
        c.frozen = Converter<bool>::read(L, ptr);
        c.bounciness = Converter<float>::read(L, ptr);
        c.frictionCoefficient = Converter<float>::read(L, ptr);
        c.rollingResistance = Converter<float>::read(L, ptr);
        c.force = Converter<glm::vec3>::read(L, ptr);
        c.onFloor = Converter<bool>::read(L, ptr);
        e.Add<RigidBody3DComponent>(c);
        assert(ptr == RigidBody3DComponent_dimension + 2);
        return 0;
    });

    luaL_dostring(L, R"lua(
        function AddRigidBody3DComponent(entity, c)
            _AddRigidBody3DComponent(entity, c.velocity.x, c.velocity.y, c.velocity.z, c.gravity, c.frozen, c.bounciness, c.frictionCoefficient, c.rollingResistance, c.force.x, c.force.y, c.force.z, c.onFloor)
        end
    )lua");

    lua_register(L, "HasRigidBody3DComponent", &_has_impl<RigidBody3DComponent>);


    // Functions for BoxCollider3DComponent =====================================================

    constexpr int BoxCollider3DComponent_dimension = 8;

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

        int ptr = 1;
        ecs::Entity e = Converter<ecs::Entity>::read(L, ptr);
        assert(e.Has<BoxCollider3DComponent>());

        int count = 0;
        const auto& c = e.Get<BoxCollider3DComponent>();
        count += Converter<glm::vec3>::push(L, c.position);
        count += Converter<float>::push(L, c.mass);
        count += Converter<glm::vec3>::push(L, c.halfExtents);
        count += Converter<bool>::push(L, c.applyScale);
        assert(count == BoxCollider3DComponent_dimension);
        return count;
    });

    luaL_dostring(L, R"lua(
        function GetBoxCollider3DComponent(entity)
            x0, x1, x2, x3, x4, x5, x6, x7 = _GetBoxCollider3DComponent(entity)
            return BoxCollider3DComponent(Vec3(x0, x1, x2), x3, Vec3(x4, x5, x6), x7)
        end
    )lua");

    lua_register(L, "_SetBoxCollider3DComponent", [](lua_State* L) {
        if (!CheckArgCount(L, BoxCollider3DComponent_dimension + 1)) { return luaL_error(L, "Bad number of args"); }

        int ptr = 1;
        ecs::Entity e = Converter<ecs::Entity>::read(L, ptr);
        auto& c = e.Get<BoxCollider3DComponent>();
        c.position = Converter<glm::vec3>::read(L, ptr);
        c.mass = Converter<float>::read(L, ptr);
        c.halfExtents = Converter<glm::vec3>::read(L, ptr);
        c.applyScale = Converter<bool>::read(L, ptr);
        assert(ptr == BoxCollider3DComponent_dimension + 2);
        return 0;
    });

    luaL_dostring(L, R"lua(
        function SetBoxCollider3DComponent(entity, c)
            _SetBoxCollider3DComponent(entity, c.position.x, c.position.y, c.position.z, c.mass, c.halfExtents.x, c.halfExtents.y, c.halfExtents.z, c.applyScale)
        end
    )lua");

    lua_register(L, "_AddBoxCollider3DComponent", [](lua_State* L) {
        if (!CheckArgCount(L, BoxCollider3DComponent_dimension + 1)) { return luaL_error(L, "Bad number of args"); }

        int ptr = 1;
        ecs::Entity e = Converter<ecs::Entity>::read(L, ptr);
        assert(!e.Has<BoxCollider3DComponent>());

        BoxCollider3DComponent c;
        c.position = Converter<glm::vec3>::read(L, ptr);
        c.mass = Converter<float>::read(L, ptr);
        c.halfExtents = Converter<glm::vec3>::read(L, ptr);
        c.applyScale = Converter<bool>::read(L, ptr);
        e.Add<BoxCollider3DComponent>(c);
        assert(ptr == BoxCollider3DComponent_dimension + 2);
        return 0;
    });

    luaL_dostring(L, R"lua(
        function AddBoxCollider3DComponent(entity, c)
            _AddBoxCollider3DComponent(entity, c.position.x, c.position.y, c.position.z, c.mass, c.halfExtents.x, c.halfExtents.y, c.halfExtents.z, c.applyScale)
        end
    )lua");

    lua_register(L, "HasBoxCollider3DComponent", &_has_impl<BoxCollider3DComponent>);


    // Functions for SphereCollider3DComponent =====================================================

    constexpr int SphereCollider3DComponent_dimension = 5;

    luaL_dostring(L, R"lua(
        SphereCollider3DComponent = Class(function(self, position, mass, radius)
            self.position = position
            self.mass = mass
            self.radius = radius
        end)
    )lua");

    lua_register(L, "_GetSphereCollider3DComponent", [](lua_State* L) {
        if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }

        int ptr = 1;
        ecs::Entity e = Converter<ecs::Entity>::read(L, ptr);
        assert(e.Has<SphereCollider3DComponent>());

        int count = 0;
        const auto& c = e.Get<SphereCollider3DComponent>();
        count += Converter<glm::vec3>::push(L, c.position);
        count += Converter<float>::push(L, c.mass);
        count += Converter<float>::push(L, c.radius);
        assert(count == SphereCollider3DComponent_dimension);
        return count;
    });

    luaL_dostring(L, R"lua(
        function GetSphereCollider3DComponent(entity)
            x0, x1, x2, x3, x4 = _GetSphereCollider3DComponent(entity)
            return SphereCollider3DComponent(Vec3(x0, x1, x2), x3, x4)
        end
    )lua");

    lua_register(L, "_SetSphereCollider3DComponent", [](lua_State* L) {
        if (!CheckArgCount(L, SphereCollider3DComponent_dimension + 1)) { return luaL_error(L, "Bad number of args"); }

        int ptr = 1;
        ecs::Entity e = Converter<ecs::Entity>::read(L, ptr);
        auto& c = e.Get<SphereCollider3DComponent>();
        c.position = Converter<glm::vec3>::read(L, ptr);
        c.mass = Converter<float>::read(L, ptr);
        c.radius = Converter<float>::read(L, ptr);
        assert(ptr == SphereCollider3DComponent_dimension + 2);
        return 0;
    });

    luaL_dostring(L, R"lua(
        function SetSphereCollider3DComponent(entity, c)
            _SetSphereCollider3DComponent(entity, c.position.x, c.position.y, c.position.z, c.mass, c.radius)
        end
    )lua");

    lua_register(L, "_AddSphereCollider3DComponent", [](lua_State* L) {
        if (!CheckArgCount(L, SphereCollider3DComponent_dimension + 1)) { return luaL_error(L, "Bad number of args"); }

        int ptr = 1;
        ecs::Entity e = Converter<ecs::Entity>::read(L, ptr);
        assert(!e.Has<SphereCollider3DComponent>());

        SphereCollider3DComponent c;
        c.position = Converter<glm::vec3>::read(L, ptr);
        c.mass = Converter<float>::read(L, ptr);
        c.radius = Converter<float>::read(L, ptr);
        e.Add<SphereCollider3DComponent>(c);
        assert(ptr == SphereCollider3DComponent_dimension + 2);
        return 0;
    });

    luaL_dostring(L, R"lua(
        function AddSphereCollider3DComponent(entity, c)
            _AddSphereCollider3DComponent(entity, c.position.x, c.position.y, c.position.z, c.mass, c.radius)
        end
    )lua");

    lua_register(L, "HasSphereCollider3DComponent", &_has_impl<SphereCollider3DComponent>);


    // Functions for CapsuleCollider3DComponent =====================================================

    constexpr int CapsuleCollider3DComponent_dimension = 6;

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

        int ptr = 1;
        ecs::Entity e = Converter<ecs::Entity>::read(L, ptr);
        assert(e.Has<CapsuleCollider3DComponent>());

        int count = 0;
        const auto& c = e.Get<CapsuleCollider3DComponent>();
        count += Converter<glm::vec3>::push(L, c.position);
        count += Converter<float>::push(L, c.mass);
        count += Converter<float>::push(L, c.radius);
        count += Converter<float>::push(L, c.height);
        assert(count == CapsuleCollider3DComponent_dimension);
        return count;
    });

    luaL_dostring(L, R"lua(
        function GetCapsuleCollider3DComponent(entity)
            x0, x1, x2, x3, x4, x5 = _GetCapsuleCollider3DComponent(entity)
            return CapsuleCollider3DComponent(Vec3(x0, x1, x2), x3, x4, x5)
        end
    )lua");

    lua_register(L, "_SetCapsuleCollider3DComponent", [](lua_State* L) {
        if (!CheckArgCount(L, CapsuleCollider3DComponent_dimension + 1)) { return luaL_error(L, "Bad number of args"); }

        int ptr = 1;
        ecs::Entity e = Converter<ecs::Entity>::read(L, ptr);
        auto& c = e.Get<CapsuleCollider3DComponent>();
        c.position = Converter<glm::vec3>::read(L, ptr);
        c.mass = Converter<float>::read(L, ptr);
        c.radius = Converter<float>::read(L, ptr);
        c.height = Converter<float>::read(L, ptr);
        assert(ptr == CapsuleCollider3DComponent_dimension + 2);
        return 0;
    });

    luaL_dostring(L, R"lua(
        function SetCapsuleCollider3DComponent(entity, c)
            _SetCapsuleCollider3DComponent(entity, c.position.x, c.position.y, c.position.z, c.mass, c.radius, c.height)
        end
    )lua");

    lua_register(L, "_AddCapsuleCollider3DComponent", [](lua_State* L) {
        if (!CheckArgCount(L, CapsuleCollider3DComponent_dimension + 1)) { return luaL_error(L, "Bad number of args"); }

        int ptr = 1;
        ecs::Entity e = Converter<ecs::Entity>::read(L, ptr);
        assert(!e.Has<CapsuleCollider3DComponent>());

        CapsuleCollider3DComponent c;
        c.position = Converter<glm::vec3>::read(L, ptr);
        c.mass = Converter<float>::read(L, ptr);
        c.radius = Converter<float>::read(L, ptr);
        c.height = Converter<float>::read(L, ptr);
        e.Add<CapsuleCollider3DComponent>(c);
        assert(ptr == CapsuleCollider3DComponent_dimension + 2);
        return 0;
    });

    luaL_dostring(L, R"lua(
        function AddCapsuleCollider3DComponent(entity, c)
            _AddCapsuleCollider3DComponent(entity, c.position.x, c.position.y, c.position.z, c.mass, c.radius, c.height)
        end
    )lua");

    lua_register(L, "HasCapsuleCollider3DComponent", &_has_impl<CapsuleCollider3DComponent>);


    // Functions for ScriptComponent =====================================================

    constexpr int ScriptComponent_dimension = 2;

    luaL_dostring(L, R"lua(
        ScriptComponent = Class(function(self, script, active)
            self.script = script
            self.active = active
        end)
    )lua");

    lua_register(L, "_GetScriptComponent", [](lua_State* L) {
        if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }

        int ptr = 1;
        ecs::Entity e = Converter<ecs::Entity>::read(L, ptr);
        assert(e.Has<ScriptComponent>());

        int count = 0;
        const auto& c = e.Get<ScriptComponent>();
        count += Converter<std::string>::push(L, c.script);
        count += Converter<bool>::push(L, c.active);
        assert(count == ScriptComponent_dimension);
        return count;
    });

    luaL_dostring(L, R"lua(
        function GetScriptComponent(entity)
            x0, x1 = _GetScriptComponent(entity)
            return ScriptComponent(x0, x1)
        end
    )lua");

    lua_register(L, "_SetScriptComponent", [](lua_State* L) {
        if (!CheckArgCount(L, ScriptComponent_dimension + 1)) { return luaL_error(L, "Bad number of args"); }

        int ptr = 1;
        ecs::Entity e = Converter<ecs::Entity>::read(L, ptr);
        auto& c = e.Get<ScriptComponent>();
        c.script = Converter<std::string>::read(L, ptr);
        c.active = Converter<bool>::read(L, ptr);
        assert(ptr == ScriptComponent_dimension + 2);
        return 0;
    });

    luaL_dostring(L, R"lua(
        function SetScriptComponent(entity, c)
            _SetScriptComponent(entity, c.script, c.active)
        end
    )lua");

    lua_register(L, "_AddScriptComponent", [](lua_State* L) {
        if (!CheckArgCount(L, ScriptComponent_dimension + 1)) { return luaL_error(L, "Bad number of args"); }

        int ptr = 1;
        ecs::Entity e = Converter<ecs::Entity>::read(L, ptr);
        assert(!e.Has<ScriptComponent>());

        ScriptComponent c;
        c.script = Converter<std::string>::read(L, ptr);
        c.active = Converter<bool>::read(L, ptr);
        e.Add<ScriptComponent>(c);
        assert(ptr == ScriptComponent_dimension + 2);
        return 0;
    });

    luaL_dostring(L, R"lua(
        function AddScriptComponent(entity, c)
            _AddScriptComponent(entity, c.script, c.active)
        end
    )lua");

    lua_register(L, "HasScriptComponent", &_has_impl<ScriptComponent>);


    // Functions for Camera3DComponent =====================================================

    constexpr int Camera3DComponent_dimension = 2;

    luaL_dostring(L, R"lua(
        Camera3DComponent = Class(function(self, fov, pitch)
            self.fov = fov
            self.pitch = pitch
        end)
    )lua");

    lua_register(L, "_GetCamera3DComponent", [](lua_State* L) {
        if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }

        int ptr = 1;
        ecs::Entity e = Converter<ecs::Entity>::read(L, ptr);
        assert(e.Has<Camera3DComponent>());

        int count = 0;
        const auto& c = e.Get<Camera3DComponent>();
        count += Converter<float>::push(L, c.fov);
        count += Converter<float>::push(L, c.pitch);
        assert(count == Camera3DComponent_dimension);
        return count;
    });

    luaL_dostring(L, R"lua(
        function GetCamera3DComponent(entity)
            x0, x1 = _GetCamera3DComponent(entity)
            return Camera3DComponent(x0, x1)
        end
    )lua");

    lua_register(L, "_SetCamera3DComponent", [](lua_State* L) {
        if (!CheckArgCount(L, Camera3DComponent_dimension + 1)) { return luaL_error(L, "Bad number of args"); }

        int ptr = 1;
        ecs::Entity e = Converter<ecs::Entity>::read(L, ptr);
        auto& c = e.Get<Camera3DComponent>();
        c.fov = Converter<float>::read(L, ptr);
        c.pitch = Converter<float>::read(L, ptr);
        assert(ptr == Camera3DComponent_dimension + 2);
        return 0;
    });

    luaL_dostring(L, R"lua(
        function SetCamera3DComponent(entity, c)
            _SetCamera3DComponent(entity, c.fov, c.pitch)
        end
    )lua");

    lua_register(L, "_AddCamera3DComponent", [](lua_State* L) {
        if (!CheckArgCount(L, Camera3DComponent_dimension + 1)) { return luaL_error(L, "Bad number of args"); }

        int ptr = 1;
        ecs::Entity e = Converter<ecs::Entity>::read(L, ptr);
        assert(!e.Has<Camera3DComponent>());

        Camera3DComponent c;
        c.fov = Converter<float>::read(L, ptr);
        c.pitch = Converter<float>::read(L, ptr);
        e.Add<Camera3DComponent>(c);
        assert(ptr == Camera3DComponent_dimension + 2);
        return 0;
    });

    luaL_dostring(L, R"lua(
        function AddCamera3DComponent(entity, c)
            _AddCamera3DComponent(entity, c.fov, c.pitch)
        end
    )lua");

    lua_register(L, "HasCamera3DComponent", &_has_impl<Camera3DComponent>);


    // Functions for SelectComponent =====================================================

    constexpr int SelectComponent_dimension = 2;

    luaL_dostring(L, R"lua(
        SelectComponent = Class(function(self, selected, hovered)
            self.selected = selected
            self.hovered = hovered
        end)
    )lua");

    lua_register(L, "_GetSelectComponent", [](lua_State* L) {
        if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }

        int ptr = 1;
        ecs::Entity e = Converter<ecs::Entity>::read(L, ptr);
        assert(e.Has<SelectComponent>());

        int count = 0;
        const auto& c = e.Get<SelectComponent>();
        count += Converter<bool>::push(L, c.selected);
        count += Converter<bool>::push(L, c.hovered);
        assert(count == SelectComponent_dimension);
        return count;
    });

    luaL_dostring(L, R"lua(
        function GetSelectComponent(entity)
            x0, x1 = _GetSelectComponent(entity)
            return SelectComponent(x0, x1)
        end
    )lua");

    lua_register(L, "_SetSelectComponent", [](lua_State* L) {
        if (!CheckArgCount(L, SelectComponent_dimension + 1)) { return luaL_error(L, "Bad number of args"); }

        int ptr = 1;
        ecs::Entity e = Converter<ecs::Entity>::read(L, ptr);
        auto& c = e.Get<SelectComponent>();
        c.selected = Converter<bool>::read(L, ptr);
        c.hovered = Converter<bool>::read(L, ptr);
        assert(ptr == SelectComponent_dimension + 2);
        return 0;
    });

    luaL_dostring(L, R"lua(
        function SetSelectComponent(entity, c)
            _SetSelectComponent(entity, c.selected, c.hovered)
        end
    )lua");

    lua_register(L, "_AddSelectComponent", [](lua_State* L) {
        if (!CheckArgCount(L, SelectComponent_dimension + 1)) { return luaL_error(L, "Bad number of args"); }

        int ptr = 1;
        ecs::Entity e = Converter<ecs::Entity>::read(L, ptr);
        assert(!e.Has<SelectComponent>());

        SelectComponent c;
        c.selected = Converter<bool>::read(L, ptr);
        c.hovered = Converter<bool>::read(L, ptr);
        e.Add<SelectComponent>(c);
        assert(ptr == SelectComponent_dimension + 2);
        return 0;
    });

    luaL_dostring(L, R"lua(
        function AddSelectComponent(entity, c)
            _AddSelectComponent(entity, c.selected, c.hovered)
        end
    )lua");

    lua_register(L, "HasSelectComponent", &_has_impl<SelectComponent>);


    // Functions for PathComponent =====================================================

    constexpr int PathComponent_dimension = 1;

    luaL_dostring(L, R"lua(
        PathComponent = Class(function(self, speed)
            self.speed = speed
        end)
    )lua");

    lua_register(L, "_GetPathComponent", [](lua_State* L) {
        if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }

        int ptr = 1;
        ecs::Entity e = Converter<ecs::Entity>::read(L, ptr);
        assert(e.Has<PathComponent>());

        int count = 0;
        const auto& c = e.Get<PathComponent>();
        count += Converter<float>::push(L, c.speed);
        assert(count == PathComponent_dimension);
        return count;
    });

    luaL_dostring(L, R"lua(
        function GetPathComponent(entity)
            x0 = _GetPathComponent(entity)
            return PathComponent(x0)
        end
    )lua");

    lua_register(L, "_SetPathComponent", [](lua_State* L) {
        if (!CheckArgCount(L, PathComponent_dimension + 1)) { return luaL_error(L, "Bad number of args"); }

        int ptr = 1;
        ecs::Entity e = Converter<ecs::Entity>::read(L, ptr);
        auto& c = e.Get<PathComponent>();
        c.speed = Converter<float>::read(L, ptr);
        assert(ptr == PathComponent_dimension + 2);
        return 0;
    });

    luaL_dostring(L, R"lua(
        function SetPathComponent(entity, c)
            _SetPathComponent(entity, c.speed)
        end
    )lua");

    lua_register(L, "_AddPathComponent", [](lua_State* L) {
        if (!CheckArgCount(L, PathComponent_dimension + 1)) { return luaL_error(L, "Bad number of args"); }

        int ptr = 1;
        ecs::Entity e = Converter<ecs::Entity>::read(L, ptr);
        assert(!e.Has<PathComponent>());

        PathComponent c;
        c.speed = Converter<float>::read(L, ptr);
        e.Add<PathComponent>(c);
        assert(ptr == PathComponent_dimension + 2);
        return 0;
    });

    luaL_dostring(L, R"lua(
        function AddPathComponent(entity, c)
            _AddPathComponent(entity, c.speed)
        end
    )lua");

    lua_register(L, "HasPathComponent", &_has_impl<PathComponent>);


    // Functions for GridComponent =====================================================

    constexpr int GridComponent_dimension = 2;

    luaL_dostring(L, R"lua(
        GridComponent = Class(function(self, x, z)
            self.x = x
            self.z = z
        end)
    )lua");

    lua_register(L, "_GetGridComponent", [](lua_State* L) {
        if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }

        int ptr = 1;
        ecs::Entity e = Converter<ecs::Entity>::read(L, ptr);
        assert(e.Has<GridComponent>());

        int count = 0;
        const auto& c = e.Get<GridComponent>();
        count += Converter<int>::push(L, c.x);
        count += Converter<int>::push(L, c.z);
        assert(count == GridComponent_dimension);
        return count;
    });

    luaL_dostring(L, R"lua(
        function GetGridComponent(entity)
            x0, x1 = _GetGridComponent(entity)
            return GridComponent(x0, x1)
        end
    )lua");

    lua_register(L, "_SetGridComponent", [](lua_State* L) {
        if (!CheckArgCount(L, GridComponent_dimension + 1)) { return luaL_error(L, "Bad number of args"); }

        int ptr = 1;
        ecs::Entity e = Converter<ecs::Entity>::read(L, ptr);
        auto& c = e.Get<GridComponent>();
        c.x = Converter<int>::read(L, ptr);
        c.z = Converter<int>::read(L, ptr);
        assert(ptr == GridComponent_dimension + 2);
        return 0;
    });

    luaL_dostring(L, R"lua(
        function SetGridComponent(entity, c)
            _SetGridComponent(entity, c.x, c.z)
        end
    )lua");

    lua_register(L, "_AddGridComponent", [](lua_State* L) {
        if (!CheckArgCount(L, GridComponent_dimension + 1)) { return luaL_error(L, "Bad number of args"); }

        int ptr = 1;
        ecs::Entity e = Converter<ecs::Entity>::read(L, ptr);
        assert(!e.Has<GridComponent>());

        GridComponent c;
        c.x = Converter<int>::read(L, ptr);
        c.z = Converter<int>::read(L, ptr);
        e.Add<GridComponent>(c);
        assert(ptr == GridComponent_dimension + 2);
        return 0;
    });

    luaL_dostring(L, R"lua(
        function AddGridComponent(entity, c)
            _AddGridComponent(entity, c.x, c.z)
        end
    )lua");

    lua_register(L, "HasGridComponent", &_has_impl<GridComponent>);


    // Functions for LightComponent =====================================================

    constexpr int LightComponent_dimension = 4;

    luaL_dostring(L, R"lua(
        LightComponent = Class(function(self, colour, brightness)
            self.colour = colour
            self.brightness = brightness
        end)
    )lua");

    lua_register(L, "_GetLightComponent", [](lua_State* L) {
        if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }

        int ptr = 1;
        ecs::Entity e = Converter<ecs::Entity>::read(L, ptr);
        assert(e.Has<LightComponent>());

        int count = 0;
        const auto& c = e.Get<LightComponent>();
        count += Converter<glm::vec3>::push(L, c.colour);
        count += Converter<float>::push(L, c.brightness);
        assert(count == LightComponent_dimension);
        return count;
    });

    luaL_dostring(L, R"lua(
        function GetLightComponent(entity)
            x0, x1, x2, x3 = _GetLightComponent(entity)
            return LightComponent(Vec3(x0, x1, x2), x3)
        end
    )lua");

    lua_register(L, "_SetLightComponent", [](lua_State* L) {
        if (!CheckArgCount(L, LightComponent_dimension + 1)) { return luaL_error(L, "Bad number of args"); }

        int ptr = 1;
        ecs::Entity e = Converter<ecs::Entity>::read(L, ptr);
        auto& c = e.Get<LightComponent>();
        c.colour = Converter<glm::vec3>::read(L, ptr);
        c.brightness = Converter<float>::read(L, ptr);
        assert(ptr == LightComponent_dimension + 2);
        return 0;
    });

    luaL_dostring(L, R"lua(
        function SetLightComponent(entity, c)
            _SetLightComponent(entity, c.colour.x, c.colour.y, c.colour.z, c.brightness)
        end
    )lua");

    lua_register(L, "_AddLightComponent", [](lua_State* L) {
        if (!CheckArgCount(L, LightComponent_dimension + 1)) { return luaL_error(L, "Bad number of args"); }

        int ptr = 1;
        ecs::Entity e = Converter<ecs::Entity>::read(L, ptr);
        assert(!e.Has<LightComponent>());

        LightComponent c;
        c.colour = Converter<glm::vec3>::read(L, ptr);
        c.brightness = Converter<float>::read(L, ptr);
        e.Add<LightComponent>(c);
        assert(ptr == LightComponent_dimension + 2);
        return 0;
    });

    luaL_dostring(L, R"lua(
        function AddLightComponent(entity, c)
            _AddLightComponent(entity, c.colour.x, c.colour.y, c.colour.z, c.brightness)
        end
    )lua");

    lua_register(L, "HasLightComponent", &_has_impl<LightComponent>);


    // Functions for SunComponent =====================================================

    constexpr int SunComponent_dimension = 8;

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

        int ptr = 1;
        ecs::Entity e = Converter<ecs::Entity>::read(L, ptr);
        assert(e.Has<SunComponent>());

        int count = 0;
        const auto& c = e.Get<SunComponent>();
        count += Converter<glm::vec3>::push(L, c.colour);
        count += Converter<float>::push(L, c.brightness);
        count += Converter<glm::vec3>::push(L, c.direction);
        count += Converter<bool>::push(L, c.shadows);
        assert(count == SunComponent_dimension);
        return count;
    });

    luaL_dostring(L, R"lua(
        function GetSunComponent(entity)
            x0, x1, x2, x3, x4, x5, x6, x7 = _GetSunComponent(entity)
            return SunComponent(Vec3(x0, x1, x2), x3, Vec3(x4, x5, x6), x7)
        end
    )lua");

    lua_register(L, "_SetSunComponent", [](lua_State* L) {
        if (!CheckArgCount(L, SunComponent_dimension + 1)) { return luaL_error(L, "Bad number of args"); }

        int ptr = 1;
        ecs::Entity e = Converter<ecs::Entity>::read(L, ptr);
        auto& c = e.Get<SunComponent>();
        c.colour = Converter<glm::vec3>::read(L, ptr);
        c.brightness = Converter<float>::read(L, ptr);
        c.direction = Converter<glm::vec3>::read(L, ptr);
        c.shadows = Converter<bool>::read(L, ptr);
        assert(ptr == SunComponent_dimension + 2);
        return 0;
    });

    luaL_dostring(L, R"lua(
        function SetSunComponent(entity, c)
            _SetSunComponent(entity, c.colour.x, c.colour.y, c.colour.z, c.brightness, c.direction.x, c.direction.y, c.direction.z, c.shadows)
        end
    )lua");

    lua_register(L, "_AddSunComponent", [](lua_State* L) {
        if (!CheckArgCount(L, SunComponent_dimension + 1)) { return luaL_error(L, "Bad number of args"); }

        int ptr = 1;
        ecs::Entity e = Converter<ecs::Entity>::read(L, ptr);
        assert(!e.Has<SunComponent>());

        SunComponent c;
        c.colour = Converter<glm::vec3>::read(L, ptr);
        c.brightness = Converter<float>::read(L, ptr);
        c.direction = Converter<glm::vec3>::read(L, ptr);
        c.shadows = Converter<bool>::read(L, ptr);
        e.Add<SunComponent>(c);
        assert(ptr == SunComponent_dimension + 2);
        return 0;
    });

    luaL_dostring(L, R"lua(
        function AddSunComponent(entity, c)
            _AddSunComponent(entity, c.colour.x, c.colour.y, c.colour.z, c.brightness, c.direction.x, c.direction.y, c.direction.z, c.shadows)
        end
    )lua");

    lua_register(L, "HasSunComponent", &_has_impl<SunComponent>);


    // Functions for AmbienceComponent =====================================================

    constexpr int AmbienceComponent_dimension = 4;

    luaL_dostring(L, R"lua(
        AmbienceComponent = Class(function(self, colour, brightness)
            self.colour = colour
            self.brightness = brightness
        end)
    )lua");

    lua_register(L, "_GetAmbienceComponent", [](lua_State* L) {
        if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }

        int ptr = 1;
        ecs::Entity e = Converter<ecs::Entity>::read(L, ptr);
        assert(e.Has<AmbienceComponent>());

        int count = 0;
        const auto& c = e.Get<AmbienceComponent>();
        count += Converter<glm::vec3>::push(L, c.colour);
        count += Converter<float>::push(L, c.brightness);
        assert(count == AmbienceComponent_dimension);
        return count;
    });

    luaL_dostring(L, R"lua(
        function GetAmbienceComponent(entity)
            x0, x1, x2, x3 = _GetAmbienceComponent(entity)
            return AmbienceComponent(Vec3(x0, x1, x2), x3)
        end
    )lua");

    lua_register(L, "_SetAmbienceComponent", [](lua_State* L) {
        if (!CheckArgCount(L, AmbienceComponent_dimension + 1)) { return luaL_error(L, "Bad number of args"); }

        int ptr = 1;
        ecs::Entity e = Converter<ecs::Entity>::read(L, ptr);
        auto& c = e.Get<AmbienceComponent>();
        c.colour = Converter<glm::vec3>::read(L, ptr);
        c.brightness = Converter<float>::read(L, ptr);
        assert(ptr == AmbienceComponent_dimension + 2);
        return 0;
    });

    luaL_dostring(L, R"lua(
        function SetAmbienceComponent(entity, c)
            _SetAmbienceComponent(entity, c.colour.x, c.colour.y, c.colour.z, c.brightness)
        end
    )lua");

    lua_register(L, "_AddAmbienceComponent", [](lua_State* L) {
        if (!CheckArgCount(L, AmbienceComponent_dimension + 1)) { return luaL_error(L, "Bad number of args"); }

        int ptr = 1;
        ecs::Entity e = Converter<ecs::Entity>::read(L, ptr);
        assert(!e.Has<AmbienceComponent>());

        AmbienceComponent c;
        c.colour = Converter<glm::vec3>::read(L, ptr);
        c.brightness = Converter<float>::read(L, ptr);
        e.Add<AmbienceComponent>(c);
        assert(ptr == AmbienceComponent_dimension + 2);
        return 0;
    });

    luaL_dostring(L, R"lua(
        function AddAmbienceComponent(entity, c)
            _AddAmbienceComponent(entity, c.colour.x, c.colour.y, c.colour.z, c.brightness)
        end
    )lua");

    lua_register(L, "HasAmbienceComponent", &_has_impl<AmbienceComponent>);


    // Functions for ParticleComponent =====================================================

    constexpr int ParticleComponent_dimension = 12;

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

        int ptr = 1;
        ecs::Entity e = Converter<ecs::Entity>::read(L, ptr);
        assert(e.Has<ParticleComponent>());

        int count = 0;
        const auto& c = e.Get<ParticleComponent>();
        count += Converter<float>::push(L, c.interval);
        count += Converter<glm::vec3>::push(L, c.velocity);
        count += Converter<float>::push(L, c.velocityNoise);
        count += Converter<glm::vec3>::push(L, c.acceleration);
        count += Converter<glm::vec3>::push(L, c.scale);
        count += Converter<float>::push(L, c.life);
        assert(count == ParticleComponent_dimension);
        return count;
    });

    luaL_dostring(L, R"lua(
        function GetParticleComponent(entity)
            x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11 = _GetParticleComponent(entity)
            return ParticleComponent(x0, Vec3(x1, x2, x3), x4, Vec3(x5, x6, x7), Vec3(x8, x9, x10), x11)
        end
    )lua");

    lua_register(L, "_SetParticleComponent", [](lua_State* L) {
        if (!CheckArgCount(L, ParticleComponent_dimension + 1)) { return luaL_error(L, "Bad number of args"); }

        int ptr = 1;
        ecs::Entity e = Converter<ecs::Entity>::read(L, ptr);
        auto& c = e.Get<ParticleComponent>();
        c.interval = Converter<float>::read(L, ptr);
        c.velocity = Converter<glm::vec3>::read(L, ptr);
        c.velocityNoise = Converter<float>::read(L, ptr);
        c.acceleration = Converter<glm::vec3>::read(L, ptr);
        c.scale = Converter<glm::vec3>::read(L, ptr);
        c.life = Converter<float>::read(L, ptr);
        assert(ptr == ParticleComponent_dimension + 2);
        return 0;
    });

    luaL_dostring(L, R"lua(
        function SetParticleComponent(entity, c)
            _SetParticleComponent(entity, c.interval, c.velocity.x, c.velocity.y, c.velocity.z, c.velocityNoise, c.acceleration.x, c.acceleration.y, c.acceleration.z, c.scale.x, c.scale.y, c.scale.z, c.life)
        end
    )lua");

    lua_register(L, "_AddParticleComponent", [](lua_State* L) {
        if (!CheckArgCount(L, ParticleComponent_dimension + 1)) { return luaL_error(L, "Bad number of args"); }

        int ptr = 1;
        ecs::Entity e = Converter<ecs::Entity>::read(L, ptr);
        assert(!e.Has<ParticleComponent>());

        ParticleComponent c;
        c.interval = Converter<float>::read(L, ptr);
        c.velocity = Converter<glm::vec3>::read(L, ptr);
        c.velocityNoise = Converter<float>::read(L, ptr);
        c.acceleration = Converter<glm::vec3>::read(L, ptr);
        c.scale = Converter<glm::vec3>::read(L, ptr);
        c.life = Converter<float>::read(L, ptr);
        e.Add<ParticleComponent>(c);
        assert(ptr == ParticleComponent_dimension + 2);
        return 0;
    });

    luaL_dostring(L, R"lua(
        function AddParticleComponent(entity, c)
            _AddParticleComponent(entity, c.interval, c.velocity.x, c.velocity.y, c.velocity.z, c.velocityNoise, c.acceleration.x, c.acceleration.y, c.acceleration.z, c.scale.x, c.scale.y, c.scale.z, c.life)
        end
    )lua");

    lua_register(L, "HasParticleComponent", &_has_impl<ParticleComponent>);


    // Functions for MeshAnimationComponent =====================================================

    constexpr int MeshAnimationComponent_dimension = 3;

    luaL_dostring(L, R"lua(
        MeshAnimationComponent = Class(function(self, name, time, speed)
            self.name = name
            self.time = time
            self.speed = speed
        end)
    )lua");

    lua_register(L, "_GetMeshAnimationComponent", [](lua_State* L) {
        if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }

        int ptr = 1;
        ecs::Entity e = Converter<ecs::Entity>::read(L, ptr);
        assert(e.Has<MeshAnimationComponent>());

        int count = 0;
        const auto& c = e.Get<MeshAnimationComponent>();
        count += Converter<std::string>::push(L, c.name);
        count += Converter<float>::push(L, c.time);
        count += Converter<float>::push(L, c.speed);
        assert(count == MeshAnimationComponent_dimension);
        return count;
    });

    luaL_dostring(L, R"lua(
        function GetMeshAnimationComponent(entity)
            x0, x1, x2 = _GetMeshAnimationComponent(entity)
            return MeshAnimationComponent(x0, x1, x2)
        end
    )lua");

    lua_register(L, "_SetMeshAnimationComponent", [](lua_State* L) {
        if (!CheckArgCount(L, MeshAnimationComponent_dimension + 1)) { return luaL_error(L, "Bad number of args"); }

        int ptr = 1;
        ecs::Entity e = Converter<ecs::Entity>::read(L, ptr);
        auto& c = e.Get<MeshAnimationComponent>();
        c.name = Converter<std::string>::read(L, ptr);
        c.time = Converter<float>::read(L, ptr);
        c.speed = Converter<float>::read(L, ptr);
        assert(ptr == MeshAnimationComponent_dimension + 2);
        return 0;
    });

    luaL_dostring(L, R"lua(
        function SetMeshAnimationComponent(entity, c)
            _SetMeshAnimationComponent(entity, c.name, c.time, c.speed)
        end
    )lua");

    lua_register(L, "_AddMeshAnimationComponent", [](lua_State* L) {
        if (!CheckArgCount(L, MeshAnimationComponent_dimension + 1)) { return luaL_error(L, "Bad number of args"); }

        int ptr = 1;
        ecs::Entity e = Converter<ecs::Entity>::read(L, ptr);
        assert(!e.Has<MeshAnimationComponent>());

        MeshAnimationComponent c;
        c.name = Converter<std::string>::read(L, ptr);
        c.time = Converter<float>::read(L, ptr);
        c.speed = Converter<float>::read(L, ptr);
        e.Add<MeshAnimationComponent>(c);
        assert(ptr == MeshAnimationComponent_dimension + 2);
        return 0;
    });

    luaL_dostring(L, R"lua(
        function AddMeshAnimationComponent(entity, c)
            _AddMeshAnimationComponent(entity, c.name, c.time, c.speed)
        end
    )lua");

    lua_register(L, "HasMeshAnimationComponent", &_has_impl<MeshAnimationComponent>);


}

}
}
