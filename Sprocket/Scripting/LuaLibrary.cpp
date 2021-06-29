#include "LuaLibrary.h"
#include "LuaScript.h"
#include "LuaConverter.h"
#include "ECS.h"
#include "Scene.h"
#include "Window.h"
#include "Components.h"
#include "Log.h"

#include "apecs.hpp"

#include <vector>
#include <utility>

#include <lua.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/trigonometric.hpp>

namespace spkt {
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
    auto entity = Converter<spkt::entity>::read(L, 1);
    Converter<bool>::push(L, entity.has<T>());
    return 1;
}

void add_command(lua_State* L, const std::function<void()>& command)
{
    using command_list_t = std::vector<std::function<void()>>;
    command_list_t& command_list = *get_pointer<command_list_t>(L, "__command_list__");
    command_list.push_back(command);
}

}

void load_registry_functions(lua::Script& script, spkt::registry& registry)
{
    lua_State* L = script.native_handle();
    script.set_value("__registry__", &registry);

    // Add functions for creating and destroying entities.
    lua_register(L, "NewEntity", [](lua_State* L) {
        if (!CheckArgCount(L, 0)) { return luaL_error(L, "Bad number of args"); }
        spkt::registry& registry = *get_pointer<spkt::registry>(L, "__registry__");
        auto new_entity = spkt::entity(registry, registry.create());
        Converter<spkt::entity>::push(L, new_entity);
        return 1;
    });

    lua_register(L, "DeleteEntity", [](lua_State* L) {
        if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }
        spkt::entity entity = *static_cast<spkt::entity*>(lua_touserdata(L, 1));
        add_command(L, [entity]() mutable { entity.destroy(); });
        return 0;
    });

    lua_register(L, "entity_from_id", [](lua_State* L) {
        if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }
        spkt::registry& registry = *get_pointer<spkt::registry>(L, "__registry__");
        auto id = Converter<apx::entity>::read(L, 1);
        Converter<spkt::entity>::push(L, {registry, id});
        return 1;
    });

    lua_register(L, "entity_singleton", [](lua_State* L) {
        if (!CheckArgCount(L, 0)) { return luaL_error(L, "Bad number of args"); }
        spkt::registry& registry = *get_pointer<spkt::registry>(L, "__registry__");
        auto singleton = registry.find<Singleton>();
        Converter<spkt::entity>::push(L, {registry, singleton});
        return 1;
    });

    // Input access via the singleton component.
    lua_register(L, "IsKeyDown", [](lua_State* L) {
        if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }
        spkt::registry& registry = *get_pointer<spkt::registry>(L, "__registry__");
        const auto& input = get_singleton<InputSingleton>(registry);
        Converter<bool>::push(L, input.keyboard[(int)lua_tointeger(L, 1)]);
        return 1;
    });

    lua_register(L, "IsMouseClicked", [](lua_State* L) {
        if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }
        spkt::registry& registry = *get_pointer<spkt::registry>(L, "__registry__");
        const auto& input = get_singleton<InputSingleton>(registry);
        Converter<bool>::push(L, input.mouse_click[(int)lua_tointeger(L, 1)]);
        return 1;
    });

    lua_register(L, "IsMouseUnclicked", [](lua_State* L) {
        if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }
        spkt::registry& registry = *get_pointer<spkt::registry>(L, "__registry__");
        const auto& input = get_singleton<InputSingleton>(registry);
        Converter<bool>::push(L, input.mouse_unclick[(int)lua_tointeger(L, 1)]);
        return 1;
    });

    lua_register(L, "IsMouseDown", [](lua_State* L) {
        if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }
        spkt::registry& registry = *get_pointer<spkt::registry>(L, "__registry__");
        const auto& input = get_singleton<InputSingleton>(registry);
        Converter<bool>::push(L, input.mouse[(int)lua_tointeger(L, 1)]);
        return 1;
    });

    lua_register(L, "GetMousePos", [](lua_State* L) {
        if (!CheckArgCount(L, 0)) { return luaL_error(L, "Bad number of args"); }
        spkt::registry& registry = *get_pointer<spkt::registry>(L, "__registry__");
        const auto& input = get_singleton<InputSingleton>(registry);
        Converter<glm::vec2>::push(L, input.mouse_pos);
        return 1;
    });

    lua_register(L, "GetMouseOffset", [](lua_State* L) {
        if (!CheckArgCount(L, 0)) { return luaL_error(L, "Bad number of args"); }
        spkt::registry& registry = *get_pointer<spkt::registry>(L, "__registry__");
        const auto& input = get_singleton<InputSingleton>(registry);
        Converter<glm::vec2>::push(L, input.mouse_offset);
        return 1;
    });

    lua_register(L, "GetMouseScrolled", [](lua_State* L) {
        if (!CheckArgCount(L, 0)) { return luaL_error(L, "Bad number of args"); }
        spkt::registry& registry = *get_pointer<spkt::registry>(L, "__registry__");
        const auto& input = get_singleton<InputSingleton>(registry);
        Converter<glm::vec2>::push(L, input.mouse_scrolled);
        return 1;
    });

    // Add functions for iterating over all entities in __scene__. The C++ functions
    // should not be used directly, instead they should be used via the Scene:Each
    // function implemented last in Lua.
    using Generator = apx::generator<apx::entity>;
    using Iterator = typename Generator::iterator;
    static_assert(std::is_trivially_destructible_v<Iterator>);

    lua_register(L, "_Each_New", [](lua_State* L) {
        if (!CheckArgCount(L, 0)) { return luaL_error(L, "Bad number of args"); }
        auto gen = new Generator(get_pointer<spkt::registry>(L, "__registry__")->all());
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
        Iterator iterator = *static_cast<Iterator*>(lua_touserdata(L, 1));
        spkt::registry& registry = *get_pointer<spkt::registry>(L, "__registry__");

        spkt::entity* luaEntity = static_cast<spkt::entity*>(lua_newuserdata(L, sizeof(spkt::entity)));
        *luaEntity = spkt::entity(registry, *iterator);
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

void load_entity_transformation_functions(lua::Script& script)
{
    lua_State* L = script.native_handle();

    lua_register(L, "SetLookAt", [](lua_State* L) {
        if (!CheckArgCount(L, 3)) { return luaL_error(L, "Bad number of args"); }
        spkt::entity entity = Converter<spkt::entity>::read(L, 1);
        glm::vec3 p = Converter<glm::vec3>::read(L, 2);
        glm::vec3 t = Converter<glm::vec3>::read(L, 3);
        auto& tr = entity.get<Transform3DComponent>();
        tr.position = p;
        tr.orientation = glm::conjugate(glm::quat_cast(glm::lookAt(tr.position, t, {0.0, 1.0, 0.0})));
        return 0;
    });

    lua_register(L, "RotateY", [](lua_State* L) {
        if (!CheckArgCount(L, 2)) { return luaL_error(L, "Bad number of args"); };
        spkt::entity entity = *static_cast<spkt::entity*>(lua_touserdata(L, 1));
        auto& tr = entity.get<Transform3DComponent>();
        float yaw = (float)lua_tonumber(L, 2);
        tr.orientation = glm::rotate(tr.orientation, yaw, {0, 1, 0});
        return 0;
    });

    lua_register(L, "GetForwardsDir", [](lua_State* L) {
        if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }
        spkt::entity entity = *static_cast<spkt::entity*>(lua_touserdata(L, 1));
        auto& tr = entity.get<Transform3DComponent>();
        auto o = tr.orientation;

        if (entity.has<Camera3DComponent>()) {
            auto pitch = entity.get<Camera3DComponent>().pitch;
            o = glm::rotate(o, pitch, {1, 0, 0});
        }

        Converter<glm::vec3>::push(L, Maths::Forwards(o));
        return 1;
    });

    lua_register(L, "GetRightDir", [](lua_State* L) {
        if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }
        spkt::entity entity = Converter<spkt::entity>::read(L, 1);
        auto& tr = entity.get<Transform3DComponent>();
        Converter<glm::vec3>::push(L, Maths::Right(tr.orientation));
        return 1;
    });

    lua_register(L, "MakeUpright", [](lua_State* L) {
        if (!CheckArgCount(L, 2)) { return luaL_error(L, "Bad number of args"); }
        spkt::entity entity = Converter<spkt::entity>::read(L, 1);
        auto& tr = entity.get<Transform3DComponent>();
        float yaw = Converter<float>::read(L, 2);
        tr.orientation = glm::quat(glm::vec3(0, yaw, 0));
        return 0;
    });

    lua_register(L, "AreEntitiesEqual", [](lua_State* L) {
        if (!CheckArgCount(L, 2)) { return luaL_error(L, "Bad number of args"); }
        spkt::entity entity1 = Converter<spkt::entity>::read(L, 1);
        spkt::entity entity2 = Converter<spkt::entity>::read(L, 2);
        Converter<bool>::push(L, entity1 == entity2);
        return 1;
    });
}

// COMPONENT RELATED CODE - GENERATED BY DATAMATIC
// C++ Functions for NameComponent =====================================================

int _GetNameComponent(lua_State* L) {
    if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }
    spkt::entity e = Converter<spkt::entity>::read(L, 1);
    assert(e.has<NameComponent>());
    const auto& c = e.get<NameComponent>();
    Converter<std::string>::push(L, c.name);
    return 1;
}

int _SetNameComponent(lua_State* L) {
    if (!CheckArgCount(L, 1 + 1)) { return luaL_error(L, "Bad number of args"); }
    int ptr = 0;
    spkt::entity e = Converter<spkt::entity>::read(L, ++ptr);
    auto& c = e.get<NameComponent>();
    c.name = Converter<std::string>::read(L, ++ptr);
    return 0;
}

int _AddNameComponent(lua_State* L) {
    if (!CheckArgCount(L, 1 + 1)) { return luaL_error(L, "Bad number of args"); }
    int ptr = 0;
    spkt::entity e = Converter<spkt::entity>::read(L, ++ptr);
    assert(!e.has<NameComponent>());
    NameComponent c;
    c.name = Converter<std::string>::read(L, ++ptr);
    add_command(L, [e, c]() mutable { e.add<NameComponent>(c); });
    return 0;
}

// C++ Functions for Transform2DComponent =====================================================

int _GetTransform2DComponent(lua_State* L) {
    if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }
    spkt::entity e = Converter<spkt::entity>::read(L, 1);
    assert(e.has<Transform2DComponent>());
    const auto& c = e.get<Transform2DComponent>();
    Converter<glm::vec2>::push(L, c.position);
    Converter<float>::push(L, c.rotation);
    Converter<glm::vec2>::push(L, c.scale);
    return 3;
}

int _SetTransform2DComponent(lua_State* L) {
    if (!CheckArgCount(L, 3 + 1)) { return luaL_error(L, "Bad number of args"); }
    int ptr = 0;
    spkt::entity e = Converter<spkt::entity>::read(L, ++ptr);
    auto& c = e.get<Transform2DComponent>();
    c.position = Converter<glm::vec2>::read(L, ++ptr);
    c.rotation = Converter<float>::read(L, ++ptr);
    c.scale = Converter<glm::vec2>::read(L, ++ptr);
    return 0;
}

int _AddTransform2DComponent(lua_State* L) {
    if (!CheckArgCount(L, 3 + 1)) { return luaL_error(L, "Bad number of args"); }
    int ptr = 0;
    spkt::entity e = Converter<spkt::entity>::read(L, ++ptr);
    assert(!e.has<Transform2DComponent>());
    Transform2DComponent c;
    c.position = Converter<glm::vec2>::read(L, ++ptr);
    c.rotation = Converter<float>::read(L, ++ptr);
    c.scale = Converter<glm::vec2>::read(L, ++ptr);
    add_command(L, [e, c]() mutable { e.add<Transform2DComponent>(c); });
    return 0;
}

// C++ Functions for Transform3DComponent =====================================================

int _GetTransform3DComponent(lua_State* L) {
    if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }
    spkt::entity e = Converter<spkt::entity>::read(L, 1);
    assert(e.has<Transform3DComponent>());
    const auto& c = e.get<Transform3DComponent>();
    Converter<glm::vec3>::push(L, c.position);
    Converter<glm::vec3>::push(L, c.scale);
    return 2;
}

int _SetTransform3DComponent(lua_State* L) {
    if (!CheckArgCount(L, 2 + 1)) { return luaL_error(L, "Bad number of args"); }
    int ptr = 0;
    spkt::entity e = Converter<spkt::entity>::read(L, ++ptr);
    auto& c = e.get<Transform3DComponent>();
    c.position = Converter<glm::vec3>::read(L, ++ptr);
    c.scale = Converter<glm::vec3>::read(L, ++ptr);
    return 0;
}

int _AddTransform3DComponent(lua_State* L) {
    if (!CheckArgCount(L, 2 + 1)) { return luaL_error(L, "Bad number of args"); }
    int ptr = 0;
    spkt::entity e = Converter<spkt::entity>::read(L, ++ptr);
    assert(!e.has<Transform3DComponent>());
    Transform3DComponent c;
    c.position = Converter<glm::vec3>::read(L, ++ptr);
    c.scale = Converter<glm::vec3>::read(L, ++ptr);
    add_command(L, [e, c]() mutable { e.add<Transform3DComponent>(c); });
    return 0;
}

// C++ Functions for ModelComponent =====================================================

int _GetModelComponent(lua_State* L) {
    if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }
    spkt::entity e = Converter<spkt::entity>::read(L, 1);
    assert(e.has<ModelComponent>());
    const auto& c = e.get<ModelComponent>();
    Converter<std::string>::push(L, c.mesh);
    Converter<std::string>::push(L, c.material);
    return 2;
}

int _SetModelComponent(lua_State* L) {
    if (!CheckArgCount(L, 2 + 1)) { return luaL_error(L, "Bad number of args"); }
    int ptr = 0;
    spkt::entity e = Converter<spkt::entity>::read(L, ++ptr);
    auto& c = e.get<ModelComponent>();
    c.mesh = Converter<std::string>::read(L, ++ptr);
    c.material = Converter<std::string>::read(L, ++ptr);
    return 0;
}

int _AddModelComponent(lua_State* L) {
    if (!CheckArgCount(L, 2 + 1)) { return luaL_error(L, "Bad number of args"); }
    int ptr = 0;
    spkt::entity e = Converter<spkt::entity>::read(L, ++ptr);
    assert(!e.has<ModelComponent>());
    ModelComponent c;
    c.mesh = Converter<std::string>::read(L, ++ptr);
    c.material = Converter<std::string>::read(L, ++ptr);
    add_command(L, [e, c]() mutable { e.add<ModelComponent>(c); });
    return 0;
}

// C++ Functions for RigidBody3DComponent =====================================================

int _GetRigidBody3DComponent(lua_State* L) {
    if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }
    spkt::entity e = Converter<spkt::entity>::read(L, 1);
    assert(e.has<RigidBody3DComponent>());
    const auto& c = e.get<RigidBody3DComponent>();
    Converter<glm::vec3>::push(L, c.velocity);
    Converter<bool>::push(L, c.gravity);
    Converter<bool>::push(L, c.frozen);
    Converter<float>::push(L, c.bounciness);
    Converter<float>::push(L, c.frictionCoefficient);
    Converter<float>::push(L, c.rollingResistance);
    Converter<glm::vec3>::push(L, c.force);
    Converter<bool>::push(L, c.onFloor);
    return 8;
}

int _SetRigidBody3DComponent(lua_State* L) {
    if (!CheckArgCount(L, 8 + 1)) { return luaL_error(L, "Bad number of args"); }
    int ptr = 0;
    spkt::entity e = Converter<spkt::entity>::read(L, ++ptr);
    auto& c = e.get<RigidBody3DComponent>();
    c.velocity = Converter<glm::vec3>::read(L, ++ptr);
    c.gravity = Converter<bool>::read(L, ++ptr);
    c.frozen = Converter<bool>::read(L, ++ptr);
    c.bounciness = Converter<float>::read(L, ++ptr);
    c.frictionCoefficient = Converter<float>::read(L, ++ptr);
    c.rollingResistance = Converter<float>::read(L, ++ptr);
    c.force = Converter<glm::vec3>::read(L, ++ptr);
    c.onFloor = Converter<bool>::read(L, ++ptr);
    return 0;
}

int _AddRigidBody3DComponent(lua_State* L) {
    if (!CheckArgCount(L, 8 + 1)) { return luaL_error(L, "Bad number of args"); }
    int ptr = 0;
    spkt::entity e = Converter<spkt::entity>::read(L, ++ptr);
    assert(!e.has<RigidBody3DComponent>());
    RigidBody3DComponent c;
    c.velocity = Converter<glm::vec3>::read(L, ++ptr);
    c.gravity = Converter<bool>::read(L, ++ptr);
    c.frozen = Converter<bool>::read(L, ++ptr);
    c.bounciness = Converter<float>::read(L, ++ptr);
    c.frictionCoefficient = Converter<float>::read(L, ++ptr);
    c.rollingResistance = Converter<float>::read(L, ++ptr);
    c.force = Converter<glm::vec3>::read(L, ++ptr);
    c.onFloor = Converter<bool>::read(L, ++ptr);
    add_command(L, [e, c]() mutable { e.add<RigidBody3DComponent>(c); });
    return 0;
}

// C++ Functions for BoxCollider3DComponent =====================================================

int _GetBoxCollider3DComponent(lua_State* L) {
    if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }
    spkt::entity e = Converter<spkt::entity>::read(L, 1);
    assert(e.has<BoxCollider3DComponent>());
    const auto& c = e.get<BoxCollider3DComponent>();
    Converter<glm::vec3>::push(L, c.position);
    Converter<float>::push(L, c.mass);
    Converter<glm::vec3>::push(L, c.halfExtents);
    Converter<bool>::push(L, c.applyScale);
    return 4;
}

int _SetBoxCollider3DComponent(lua_State* L) {
    if (!CheckArgCount(L, 4 + 1)) { return luaL_error(L, "Bad number of args"); }
    int ptr = 0;
    spkt::entity e = Converter<spkt::entity>::read(L, ++ptr);
    auto& c = e.get<BoxCollider3DComponent>();
    c.position = Converter<glm::vec3>::read(L, ++ptr);
    c.mass = Converter<float>::read(L, ++ptr);
    c.halfExtents = Converter<glm::vec3>::read(L, ++ptr);
    c.applyScale = Converter<bool>::read(L, ++ptr);
    return 0;
}

int _AddBoxCollider3DComponent(lua_State* L) {
    if (!CheckArgCount(L, 4 + 1)) { return luaL_error(L, "Bad number of args"); }
    int ptr = 0;
    spkt::entity e = Converter<spkt::entity>::read(L, ++ptr);
    assert(!e.has<BoxCollider3DComponent>());
    BoxCollider3DComponent c;
    c.position = Converter<glm::vec3>::read(L, ++ptr);
    c.mass = Converter<float>::read(L, ++ptr);
    c.halfExtents = Converter<glm::vec3>::read(L, ++ptr);
    c.applyScale = Converter<bool>::read(L, ++ptr);
    add_command(L, [e, c]() mutable { e.add<BoxCollider3DComponent>(c); });
    return 0;
}

// C++ Functions for SphereCollider3DComponent =====================================================

int _GetSphereCollider3DComponent(lua_State* L) {
    if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }
    spkt::entity e = Converter<spkt::entity>::read(L, 1);
    assert(e.has<SphereCollider3DComponent>());
    const auto& c = e.get<SphereCollider3DComponent>();
    Converter<glm::vec3>::push(L, c.position);
    Converter<float>::push(L, c.mass);
    Converter<float>::push(L, c.radius);
    return 3;
}

int _SetSphereCollider3DComponent(lua_State* L) {
    if (!CheckArgCount(L, 3 + 1)) { return luaL_error(L, "Bad number of args"); }
    int ptr = 0;
    spkt::entity e = Converter<spkt::entity>::read(L, ++ptr);
    auto& c = e.get<SphereCollider3DComponent>();
    c.position = Converter<glm::vec3>::read(L, ++ptr);
    c.mass = Converter<float>::read(L, ++ptr);
    c.radius = Converter<float>::read(L, ++ptr);
    return 0;
}

int _AddSphereCollider3DComponent(lua_State* L) {
    if (!CheckArgCount(L, 3 + 1)) { return luaL_error(L, "Bad number of args"); }
    int ptr = 0;
    spkt::entity e = Converter<spkt::entity>::read(L, ++ptr);
    assert(!e.has<SphereCollider3DComponent>());
    SphereCollider3DComponent c;
    c.position = Converter<glm::vec3>::read(L, ++ptr);
    c.mass = Converter<float>::read(L, ++ptr);
    c.radius = Converter<float>::read(L, ++ptr);
    add_command(L, [e, c]() mutable { e.add<SphereCollider3DComponent>(c); });
    return 0;
}

// C++ Functions for CapsuleCollider3DComponent =====================================================

int _GetCapsuleCollider3DComponent(lua_State* L) {
    if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }
    spkt::entity e = Converter<spkt::entity>::read(L, 1);
    assert(e.has<CapsuleCollider3DComponent>());
    const auto& c = e.get<CapsuleCollider3DComponent>();
    Converter<glm::vec3>::push(L, c.position);
    Converter<float>::push(L, c.mass);
    Converter<float>::push(L, c.radius);
    Converter<float>::push(L, c.height);
    return 4;
}

int _SetCapsuleCollider3DComponent(lua_State* L) {
    if (!CheckArgCount(L, 4 + 1)) { return luaL_error(L, "Bad number of args"); }
    int ptr = 0;
    spkt::entity e = Converter<spkt::entity>::read(L, ++ptr);
    auto& c = e.get<CapsuleCollider3DComponent>();
    c.position = Converter<glm::vec3>::read(L, ++ptr);
    c.mass = Converter<float>::read(L, ++ptr);
    c.radius = Converter<float>::read(L, ++ptr);
    c.height = Converter<float>::read(L, ++ptr);
    return 0;
}

int _AddCapsuleCollider3DComponent(lua_State* L) {
    if (!CheckArgCount(L, 4 + 1)) { return luaL_error(L, "Bad number of args"); }
    int ptr = 0;
    spkt::entity e = Converter<spkt::entity>::read(L, ++ptr);
    assert(!e.has<CapsuleCollider3DComponent>());
    CapsuleCollider3DComponent c;
    c.position = Converter<glm::vec3>::read(L, ++ptr);
    c.mass = Converter<float>::read(L, ++ptr);
    c.radius = Converter<float>::read(L, ++ptr);
    c.height = Converter<float>::read(L, ++ptr);
    add_command(L, [e, c]() mutable { e.add<CapsuleCollider3DComponent>(c); });
    return 0;
}

// C++ Functions for ScriptComponent =====================================================

int _GetScriptComponent(lua_State* L) {
    if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }
    spkt::entity e = Converter<spkt::entity>::read(L, 1);
    assert(e.has<ScriptComponent>());
    const auto& c = e.get<ScriptComponent>();
    Converter<std::string>::push(L, c.script);
    Converter<bool>::push(L, c.active);
    return 2;
}

int _SetScriptComponent(lua_State* L) {
    if (!CheckArgCount(L, 2 + 1)) { return luaL_error(L, "Bad number of args"); }
    int ptr = 0;
    spkt::entity e = Converter<spkt::entity>::read(L, ++ptr);
    auto& c = e.get<ScriptComponent>();
    c.script = Converter<std::string>::read(L, ++ptr);
    c.active = Converter<bool>::read(L, ++ptr);
    return 0;
}

int _AddScriptComponent(lua_State* L) {
    if (!CheckArgCount(L, 2 + 1)) { return luaL_error(L, "Bad number of args"); }
    int ptr = 0;
    spkt::entity e = Converter<spkt::entity>::read(L, ++ptr);
    assert(!e.has<ScriptComponent>());
    ScriptComponent c;
    c.script = Converter<std::string>::read(L, ++ptr);
    c.active = Converter<bool>::read(L, ++ptr);
    add_command(L, [e, c]() mutable { e.add<ScriptComponent>(c); });
    return 0;
}

// C++ Functions for Camera3DComponent =====================================================

int _GetCamera3DComponent(lua_State* L) {
    if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }
    spkt::entity e = Converter<spkt::entity>::read(L, 1);
    assert(e.has<Camera3DComponent>());
    const auto& c = e.get<Camera3DComponent>();
    Converter<float>::push(L, c.fov);
    Converter<float>::push(L, c.pitch);
    return 2;
}

int _SetCamera3DComponent(lua_State* L) {
    if (!CheckArgCount(L, 2 + 1)) { return luaL_error(L, "Bad number of args"); }
    int ptr = 0;
    spkt::entity e = Converter<spkt::entity>::read(L, ++ptr);
    auto& c = e.get<Camera3DComponent>();
    c.fov = Converter<float>::read(L, ++ptr);
    c.pitch = Converter<float>::read(L, ++ptr);
    return 0;
}

int _AddCamera3DComponent(lua_State* L) {
    if (!CheckArgCount(L, 2 + 1)) { return luaL_error(L, "Bad number of args"); }
    int ptr = 0;
    spkt::entity e = Converter<spkt::entity>::read(L, ++ptr);
    assert(!e.has<Camera3DComponent>());
    Camera3DComponent c;
    c.fov = Converter<float>::read(L, ++ptr);
    c.pitch = Converter<float>::read(L, ++ptr);
    add_command(L, [e, c]() mutable { e.add<Camera3DComponent>(c); });
    return 0;
}

// C++ Functions for PathComponent =====================================================

int _GetPathComponent(lua_State* L) {
    if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }
    spkt::entity e = Converter<spkt::entity>::read(L, 1);
    assert(e.has<PathComponent>());
    const auto& c = e.get<PathComponent>();
    Converter<float>::push(L, c.speed);
    return 1;
}

int _SetPathComponent(lua_State* L) {
    if (!CheckArgCount(L, 1 + 1)) { return luaL_error(L, "Bad number of args"); }
    int ptr = 0;
    spkt::entity e = Converter<spkt::entity>::read(L, ++ptr);
    auto& c = e.get<PathComponent>();
    c.speed = Converter<float>::read(L, ++ptr);
    return 0;
}

int _AddPathComponent(lua_State* L) {
    if (!CheckArgCount(L, 1 + 1)) { return luaL_error(L, "Bad number of args"); }
    int ptr = 0;
    spkt::entity e = Converter<spkt::entity>::read(L, ++ptr);
    assert(!e.has<PathComponent>());
    PathComponent c;
    c.speed = Converter<float>::read(L, ++ptr);
    add_command(L, [e, c]() mutable { e.add<PathComponent>(c); });
    return 0;
}

// C++ Functions for LightComponent =====================================================

int _GetLightComponent(lua_State* L) {
    if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }
    spkt::entity e = Converter<spkt::entity>::read(L, 1);
    assert(e.has<LightComponent>());
    const auto& c = e.get<LightComponent>();
    Converter<glm::vec3>::push(L, c.colour);
    Converter<float>::push(L, c.brightness);
    return 2;
}

int _SetLightComponent(lua_State* L) {
    if (!CheckArgCount(L, 2 + 1)) { return luaL_error(L, "Bad number of args"); }
    int ptr = 0;
    spkt::entity e = Converter<spkt::entity>::read(L, ++ptr);
    auto& c = e.get<LightComponent>();
    c.colour = Converter<glm::vec3>::read(L, ++ptr);
    c.brightness = Converter<float>::read(L, ++ptr);
    return 0;
}

int _AddLightComponent(lua_State* L) {
    if (!CheckArgCount(L, 2 + 1)) { return luaL_error(L, "Bad number of args"); }
    int ptr = 0;
    spkt::entity e = Converter<spkt::entity>::read(L, ++ptr);
    assert(!e.has<LightComponent>());
    LightComponent c;
    c.colour = Converter<glm::vec3>::read(L, ++ptr);
    c.brightness = Converter<float>::read(L, ++ptr);
    add_command(L, [e, c]() mutable { e.add<LightComponent>(c); });
    return 0;
}

// C++ Functions for SunComponent =====================================================

int _GetSunComponent(lua_State* L) {
    if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }
    spkt::entity e = Converter<spkt::entity>::read(L, 1);
    assert(e.has<SunComponent>());
    const auto& c = e.get<SunComponent>();
    Converter<glm::vec3>::push(L, c.colour);
    Converter<float>::push(L, c.brightness);
    Converter<glm::vec3>::push(L, c.direction);
    Converter<bool>::push(L, c.shadows);
    return 4;
}

int _SetSunComponent(lua_State* L) {
    if (!CheckArgCount(L, 4 + 1)) { return luaL_error(L, "Bad number of args"); }
    int ptr = 0;
    spkt::entity e = Converter<spkt::entity>::read(L, ++ptr);
    auto& c = e.get<SunComponent>();
    c.colour = Converter<glm::vec3>::read(L, ++ptr);
    c.brightness = Converter<float>::read(L, ++ptr);
    c.direction = Converter<glm::vec3>::read(L, ++ptr);
    c.shadows = Converter<bool>::read(L, ++ptr);
    return 0;
}

int _AddSunComponent(lua_State* L) {
    if (!CheckArgCount(L, 4 + 1)) { return luaL_error(L, "Bad number of args"); }
    int ptr = 0;
    spkt::entity e = Converter<spkt::entity>::read(L, ++ptr);
    assert(!e.has<SunComponent>());
    SunComponent c;
    c.colour = Converter<glm::vec3>::read(L, ++ptr);
    c.brightness = Converter<float>::read(L, ++ptr);
    c.direction = Converter<glm::vec3>::read(L, ++ptr);
    c.shadows = Converter<bool>::read(L, ++ptr);
    add_command(L, [e, c]() mutable { e.add<SunComponent>(c); });
    return 0;
}

// C++ Functions for AmbienceComponent =====================================================

int _GetAmbienceComponent(lua_State* L) {
    if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }
    spkt::entity e = Converter<spkt::entity>::read(L, 1);
    assert(e.has<AmbienceComponent>());
    const auto& c = e.get<AmbienceComponent>();
    Converter<glm::vec3>::push(L, c.colour);
    Converter<float>::push(L, c.brightness);
    return 2;
}

int _SetAmbienceComponent(lua_State* L) {
    if (!CheckArgCount(L, 2 + 1)) { return luaL_error(L, "Bad number of args"); }
    int ptr = 0;
    spkt::entity e = Converter<spkt::entity>::read(L, ++ptr);
    auto& c = e.get<AmbienceComponent>();
    c.colour = Converter<glm::vec3>::read(L, ++ptr);
    c.brightness = Converter<float>::read(L, ++ptr);
    return 0;
}

int _AddAmbienceComponent(lua_State* L) {
    if (!CheckArgCount(L, 2 + 1)) { return luaL_error(L, "Bad number of args"); }
    int ptr = 0;
    spkt::entity e = Converter<spkt::entity>::read(L, ++ptr);
    assert(!e.has<AmbienceComponent>());
    AmbienceComponent c;
    c.colour = Converter<glm::vec3>::read(L, ++ptr);
    c.brightness = Converter<float>::read(L, ++ptr);
    add_command(L, [e, c]() mutable { e.add<AmbienceComponent>(c); });
    return 0;
}

// C++ Functions for ParticleComponent =====================================================

int _GetParticleComponent(lua_State* L) {
    if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }
    spkt::entity e = Converter<spkt::entity>::read(L, 1);
    assert(e.has<ParticleComponent>());
    const auto& c = e.get<ParticleComponent>();
    Converter<float>::push(L, c.interval);
    Converter<glm::vec3>::push(L, c.velocity);
    Converter<float>::push(L, c.velocityNoise);
    Converter<glm::vec3>::push(L, c.acceleration);
    Converter<glm::vec3>::push(L, c.scale);
    Converter<float>::push(L, c.life);
    return 6;
}

int _SetParticleComponent(lua_State* L) {
    if (!CheckArgCount(L, 6 + 1)) { return luaL_error(L, "Bad number of args"); }
    int ptr = 0;
    spkt::entity e = Converter<spkt::entity>::read(L, ++ptr);
    auto& c = e.get<ParticleComponent>();
    c.interval = Converter<float>::read(L, ++ptr);
    c.velocity = Converter<glm::vec3>::read(L, ++ptr);
    c.velocityNoise = Converter<float>::read(L, ++ptr);
    c.acceleration = Converter<glm::vec3>::read(L, ++ptr);
    c.scale = Converter<glm::vec3>::read(L, ++ptr);
    c.life = Converter<float>::read(L, ++ptr);
    return 0;
}

int _AddParticleComponent(lua_State* L) {
    if (!CheckArgCount(L, 6 + 1)) { return luaL_error(L, "Bad number of args"); }
    int ptr = 0;
    spkt::entity e = Converter<spkt::entity>::read(L, ++ptr);
    assert(!e.has<ParticleComponent>());
    ParticleComponent c;
    c.interval = Converter<float>::read(L, ++ptr);
    c.velocity = Converter<glm::vec3>::read(L, ++ptr);
    c.velocityNoise = Converter<float>::read(L, ++ptr);
    c.acceleration = Converter<glm::vec3>::read(L, ++ptr);
    c.scale = Converter<glm::vec3>::read(L, ++ptr);
    c.life = Converter<float>::read(L, ++ptr);
    add_command(L, [e, c]() mutable { e.add<ParticleComponent>(c); });
    return 0;
}

// C++ Functions for MeshAnimationComponent =====================================================

int _GetMeshAnimationComponent(lua_State* L) {
    if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }
    spkt::entity e = Converter<spkt::entity>::read(L, 1);
    assert(e.has<MeshAnimationComponent>());
    const auto& c = e.get<MeshAnimationComponent>();
    Converter<std::string>::push(L, c.name);
    Converter<float>::push(L, c.time);
    Converter<float>::push(L, c.speed);
    return 3;
}

int _SetMeshAnimationComponent(lua_State* L) {
    if (!CheckArgCount(L, 3 + 1)) { return luaL_error(L, "Bad number of args"); }
    int ptr = 0;
    spkt::entity e = Converter<spkt::entity>::read(L, ++ptr);
    auto& c = e.get<MeshAnimationComponent>();
    c.name = Converter<std::string>::read(L, ++ptr);
    c.time = Converter<float>::read(L, ++ptr);
    c.speed = Converter<float>::read(L, ++ptr);
    return 0;
}

int _AddMeshAnimationComponent(lua_State* L) {
    if (!CheckArgCount(L, 3 + 1)) { return luaL_error(L, "Bad number of args"); }
    int ptr = 0;
    spkt::entity e = Converter<spkt::entity>::read(L, ++ptr);
    assert(!e.has<MeshAnimationComponent>());
    MeshAnimationComponent c;
    c.name = Converter<std::string>::read(L, ++ptr);
    c.time = Converter<float>::read(L, ++ptr);
    c.speed = Converter<float>::read(L, ++ptr);
    add_command(L, [e, c]() mutable { e.add<MeshAnimationComponent>(c); });
    return 0;
}

// C++ Functions for PhysicsSingleton =====================================================

int _GetPhysicsSingleton(lua_State* L) {
    if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }
    spkt::entity e = Converter<spkt::entity>::read(L, 1);
    assert(e.has<PhysicsSingleton>());
    const auto& c = e.get<PhysicsSingleton>();
    Converter<std::vector<std::pair<apx::entity, apx::entity>>>::push(L, c.collisions);
    return 1;
}

int _SetPhysicsSingleton(lua_State* L) {
    if (!CheckArgCount(L, 1 + 1)) { return luaL_error(L, "Bad number of args"); }
    int ptr = 0;
    spkt::entity e = Converter<spkt::entity>::read(L, ++ptr);
    auto& c = e.get<PhysicsSingleton>();
    c.collisions = Converter<std::vector<std::pair<apx::entity, apx::entity>>>::read(L, ++ptr);
    return 0;
}

int _AddPhysicsSingleton(lua_State* L) {
    if (!CheckArgCount(L, 1 + 1)) { return luaL_error(L, "Bad number of args"); }
    int ptr = 0;
    spkt::entity e = Converter<spkt::entity>::read(L, ++ptr);
    assert(!e.has<PhysicsSingleton>());
    PhysicsSingleton c;
    c.collisions = Converter<std::vector<std::pair<apx::entity, apx::entity>>>::read(L, ++ptr);
    add_command(L, [e, c]() mutable { e.add<PhysicsSingleton>(c); });
    return 0;
}


void load_entity_component_functions(lua::Script& script)
{
    lua_State* L = script.native_handle();

    // Lua functions for NameComponent =====================================================

    luaL_dostring(L, R"lua(
        NameComponent = Class(function(self, name)
            self.name = name
        end)
    )lua");

    lua_register(L, "_GetNameComponent", &_GetNameComponent);

    luaL_dostring(L, R"lua(
        function GetNameComponent(entity)
            name = _GetNameComponent(entity)
            return NameComponent(name)
        end
    )lua");

    lua_register(L, "_SetNameComponent", &_SetNameComponent);

    luaL_dostring(L, R"lua(
        function SetNameComponent(entity, c)
            _SetNameComponent(entity, c.name)
        end
    )lua");

    lua_register(L, "_AddNameComponent", &_AddNameComponent);

    luaL_dostring(L, R"lua(
        function AddNameComponent(entity, c)
            _AddNameComponent(entity, c.name)
        end
    )lua");

    lua_register(L, "HasNameComponent", &_has_impl<NameComponent>);


    // Lua functions for Transform2DComponent =====================================================

    luaL_dostring(L, R"lua(
        Transform2DComponent = Class(function(self, position, rotation, scale)
            self.position = position
            self.rotation = rotation
            self.scale = scale
        end)
    )lua");

    lua_register(L, "_GetTransform2DComponent", &_GetTransform2DComponent);

    luaL_dostring(L, R"lua(
        function GetTransform2DComponent(entity)
            position, rotation, scale = _GetTransform2DComponent(entity)
            return Transform2DComponent(position, rotation, scale)
        end
    )lua");

    lua_register(L, "_SetTransform2DComponent", &_SetTransform2DComponent);

    luaL_dostring(L, R"lua(
        function SetTransform2DComponent(entity, c)
            _SetTransform2DComponent(entity, c.position, c.rotation, c.scale)
        end
    )lua");

    lua_register(L, "_AddTransform2DComponent", &_AddTransform2DComponent);

    luaL_dostring(L, R"lua(
        function AddTransform2DComponent(entity, c)
            _AddTransform2DComponent(entity, c.position, c.rotation, c.scale)
        end
    )lua");

    lua_register(L, "HasTransform2DComponent", &_has_impl<Transform2DComponent>);


    // Lua functions for Transform3DComponent =====================================================

    luaL_dostring(L, R"lua(
        Transform3DComponent = Class(function(self, position, scale)
            self.position = position
            self.scale = scale
        end)
    )lua");

    lua_register(L, "_GetTransform3DComponent", &_GetTransform3DComponent);

    luaL_dostring(L, R"lua(
        function GetTransform3DComponent(entity)
            position, scale = _GetTransform3DComponent(entity)
            return Transform3DComponent(position, scale)
        end
    )lua");

    lua_register(L, "_SetTransform3DComponent", &_SetTransform3DComponent);

    luaL_dostring(L, R"lua(
        function SetTransform3DComponent(entity, c)
            _SetTransform3DComponent(entity, c.position, c.scale)
        end
    )lua");

    lua_register(L, "_AddTransform3DComponent", &_AddTransform3DComponent);

    luaL_dostring(L, R"lua(
        function AddTransform3DComponent(entity, c)
            _AddTransform3DComponent(entity, c.position, c.scale)
        end
    )lua");

    lua_register(L, "HasTransform3DComponent", &_has_impl<Transform3DComponent>);


    // Lua functions for ModelComponent =====================================================

    luaL_dostring(L, R"lua(
        ModelComponent = Class(function(self, mesh, material)
            self.mesh = mesh
            self.material = material
        end)
    )lua");

    lua_register(L, "_GetModelComponent", &_GetModelComponent);

    luaL_dostring(L, R"lua(
        function GetModelComponent(entity)
            mesh, material = _GetModelComponent(entity)
            return ModelComponent(mesh, material)
        end
    )lua");

    lua_register(L, "_SetModelComponent", &_SetModelComponent);

    luaL_dostring(L, R"lua(
        function SetModelComponent(entity, c)
            _SetModelComponent(entity, c.mesh, c.material)
        end
    )lua");

    lua_register(L, "_AddModelComponent", &_AddModelComponent);

    luaL_dostring(L, R"lua(
        function AddModelComponent(entity, c)
            _AddModelComponent(entity, c.mesh, c.material)
        end
    )lua");

    lua_register(L, "HasModelComponent", &_has_impl<ModelComponent>);


    // Lua functions for RigidBody3DComponent =====================================================

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

    lua_register(L, "_GetRigidBody3DComponent", &_GetRigidBody3DComponent);

    luaL_dostring(L, R"lua(
        function GetRigidBody3DComponent(entity)
            velocity, gravity, frozen, bounciness, frictionCoefficient, rollingResistance, force, onFloor = _GetRigidBody3DComponent(entity)
            return RigidBody3DComponent(velocity, gravity, frozen, bounciness, frictionCoefficient, rollingResistance, force, onFloor)
        end
    )lua");

    lua_register(L, "_SetRigidBody3DComponent", &_SetRigidBody3DComponent);

    luaL_dostring(L, R"lua(
        function SetRigidBody3DComponent(entity, c)
            _SetRigidBody3DComponent(entity, c.velocity, c.gravity, c.frozen, c.bounciness, c.frictionCoefficient, c.rollingResistance, c.force, c.onFloor)
        end
    )lua");

    lua_register(L, "_AddRigidBody3DComponent", &_AddRigidBody3DComponent);

    luaL_dostring(L, R"lua(
        function AddRigidBody3DComponent(entity, c)
            _AddRigidBody3DComponent(entity, c.velocity, c.gravity, c.frozen, c.bounciness, c.frictionCoefficient, c.rollingResistance, c.force, c.onFloor)
        end
    )lua");

    lua_register(L, "HasRigidBody3DComponent", &_has_impl<RigidBody3DComponent>);


    // Lua functions for BoxCollider3DComponent =====================================================

    luaL_dostring(L, R"lua(
        BoxCollider3DComponent = Class(function(self, position, mass, halfExtents, applyScale)
            self.position = position
            self.mass = mass
            self.halfExtents = halfExtents
            self.applyScale = applyScale
        end)
    )lua");

    lua_register(L, "_GetBoxCollider3DComponent", &_GetBoxCollider3DComponent);

    luaL_dostring(L, R"lua(
        function GetBoxCollider3DComponent(entity)
            position, mass, halfExtents, applyScale = _GetBoxCollider3DComponent(entity)
            return BoxCollider3DComponent(position, mass, halfExtents, applyScale)
        end
    )lua");

    lua_register(L, "_SetBoxCollider3DComponent", &_SetBoxCollider3DComponent);

    luaL_dostring(L, R"lua(
        function SetBoxCollider3DComponent(entity, c)
            _SetBoxCollider3DComponent(entity, c.position, c.mass, c.halfExtents, c.applyScale)
        end
    )lua");

    lua_register(L, "_AddBoxCollider3DComponent", &_AddBoxCollider3DComponent);

    luaL_dostring(L, R"lua(
        function AddBoxCollider3DComponent(entity, c)
            _AddBoxCollider3DComponent(entity, c.position, c.mass, c.halfExtents, c.applyScale)
        end
    )lua");

    lua_register(L, "HasBoxCollider3DComponent", &_has_impl<BoxCollider3DComponent>);


    // Lua functions for SphereCollider3DComponent =====================================================

    luaL_dostring(L, R"lua(
        SphereCollider3DComponent = Class(function(self, position, mass, radius)
            self.position = position
            self.mass = mass
            self.radius = radius
        end)
    )lua");

    lua_register(L, "_GetSphereCollider3DComponent", &_GetSphereCollider3DComponent);

    luaL_dostring(L, R"lua(
        function GetSphereCollider3DComponent(entity)
            position, mass, radius = _GetSphereCollider3DComponent(entity)
            return SphereCollider3DComponent(position, mass, radius)
        end
    )lua");

    lua_register(L, "_SetSphereCollider3DComponent", &_SetSphereCollider3DComponent);

    luaL_dostring(L, R"lua(
        function SetSphereCollider3DComponent(entity, c)
            _SetSphereCollider3DComponent(entity, c.position, c.mass, c.radius)
        end
    )lua");

    lua_register(L, "_AddSphereCollider3DComponent", &_AddSphereCollider3DComponent);

    luaL_dostring(L, R"lua(
        function AddSphereCollider3DComponent(entity, c)
            _AddSphereCollider3DComponent(entity, c.position, c.mass, c.radius)
        end
    )lua");

    lua_register(L, "HasSphereCollider3DComponent", &_has_impl<SphereCollider3DComponent>);


    // Lua functions for CapsuleCollider3DComponent =====================================================

    luaL_dostring(L, R"lua(
        CapsuleCollider3DComponent = Class(function(self, position, mass, radius, height)
            self.position = position
            self.mass = mass
            self.radius = radius
            self.height = height
        end)
    )lua");

    lua_register(L, "_GetCapsuleCollider3DComponent", &_GetCapsuleCollider3DComponent);

    luaL_dostring(L, R"lua(
        function GetCapsuleCollider3DComponent(entity)
            position, mass, radius, height = _GetCapsuleCollider3DComponent(entity)
            return CapsuleCollider3DComponent(position, mass, radius, height)
        end
    )lua");

    lua_register(L, "_SetCapsuleCollider3DComponent", &_SetCapsuleCollider3DComponent);

    luaL_dostring(L, R"lua(
        function SetCapsuleCollider3DComponent(entity, c)
            _SetCapsuleCollider3DComponent(entity, c.position, c.mass, c.radius, c.height)
        end
    )lua");

    lua_register(L, "_AddCapsuleCollider3DComponent", &_AddCapsuleCollider3DComponent);

    luaL_dostring(L, R"lua(
        function AddCapsuleCollider3DComponent(entity, c)
            _AddCapsuleCollider3DComponent(entity, c.position, c.mass, c.radius, c.height)
        end
    )lua");

    lua_register(L, "HasCapsuleCollider3DComponent", &_has_impl<CapsuleCollider3DComponent>);


    // Lua functions for ScriptComponent =====================================================

    luaL_dostring(L, R"lua(
        ScriptComponent = Class(function(self, script, active)
            self.script = script
            self.active = active
        end)
    )lua");

    lua_register(L, "_GetScriptComponent", &_GetScriptComponent);

    luaL_dostring(L, R"lua(
        function GetScriptComponent(entity)
            script, active = _GetScriptComponent(entity)
            return ScriptComponent(script, active)
        end
    )lua");

    lua_register(L, "_SetScriptComponent", &_SetScriptComponent);

    luaL_dostring(L, R"lua(
        function SetScriptComponent(entity, c)
            _SetScriptComponent(entity, c.script, c.active)
        end
    )lua");

    lua_register(L, "_AddScriptComponent", &_AddScriptComponent);

    luaL_dostring(L, R"lua(
        function AddScriptComponent(entity, c)
            _AddScriptComponent(entity, c.script, c.active)
        end
    )lua");

    lua_register(L, "HasScriptComponent", &_has_impl<ScriptComponent>);


    // Lua functions for Camera3DComponent =====================================================

    luaL_dostring(L, R"lua(
        Camera3DComponent = Class(function(self, fov, pitch)
            self.fov = fov
            self.pitch = pitch
        end)
    )lua");

    lua_register(L, "_GetCamera3DComponent", &_GetCamera3DComponent);

    luaL_dostring(L, R"lua(
        function GetCamera3DComponent(entity)
            fov, pitch = _GetCamera3DComponent(entity)
            return Camera3DComponent(fov, pitch)
        end
    )lua");

    lua_register(L, "_SetCamera3DComponent", &_SetCamera3DComponent);

    luaL_dostring(L, R"lua(
        function SetCamera3DComponent(entity, c)
            _SetCamera3DComponent(entity, c.fov, c.pitch)
        end
    )lua");

    lua_register(L, "_AddCamera3DComponent", &_AddCamera3DComponent);

    luaL_dostring(L, R"lua(
        function AddCamera3DComponent(entity, c)
            _AddCamera3DComponent(entity, c.fov, c.pitch)
        end
    )lua");

    lua_register(L, "HasCamera3DComponent", &_has_impl<Camera3DComponent>);


    // Lua functions for PathComponent =====================================================

    luaL_dostring(L, R"lua(
        PathComponent = Class(function(self, speed)
            self.speed = speed
        end)
    )lua");

    lua_register(L, "_GetPathComponent", &_GetPathComponent);

    luaL_dostring(L, R"lua(
        function GetPathComponent(entity)
            speed = _GetPathComponent(entity)
            return PathComponent(speed)
        end
    )lua");

    lua_register(L, "_SetPathComponent", &_SetPathComponent);

    luaL_dostring(L, R"lua(
        function SetPathComponent(entity, c)
            _SetPathComponent(entity, c.speed)
        end
    )lua");

    lua_register(L, "_AddPathComponent", &_AddPathComponent);

    luaL_dostring(L, R"lua(
        function AddPathComponent(entity, c)
            _AddPathComponent(entity, c.speed)
        end
    )lua");

    lua_register(L, "HasPathComponent", &_has_impl<PathComponent>);


    // Lua functions for LightComponent =====================================================

    luaL_dostring(L, R"lua(
        LightComponent = Class(function(self, colour, brightness)
            self.colour = colour
            self.brightness = brightness
        end)
    )lua");

    lua_register(L, "_GetLightComponent", &_GetLightComponent);

    luaL_dostring(L, R"lua(
        function GetLightComponent(entity)
            colour, brightness = _GetLightComponent(entity)
            return LightComponent(colour, brightness)
        end
    )lua");

    lua_register(L, "_SetLightComponent", &_SetLightComponent);

    luaL_dostring(L, R"lua(
        function SetLightComponent(entity, c)
            _SetLightComponent(entity, c.colour, c.brightness)
        end
    )lua");

    lua_register(L, "_AddLightComponent", &_AddLightComponent);

    luaL_dostring(L, R"lua(
        function AddLightComponent(entity, c)
            _AddLightComponent(entity, c.colour, c.brightness)
        end
    )lua");

    lua_register(L, "HasLightComponent", &_has_impl<LightComponent>);


    // Lua functions for SunComponent =====================================================

    luaL_dostring(L, R"lua(
        SunComponent = Class(function(self, colour, brightness, direction, shadows)
            self.colour = colour
            self.brightness = brightness
            self.direction = direction
            self.shadows = shadows
        end)
    )lua");

    lua_register(L, "_GetSunComponent", &_GetSunComponent);

    luaL_dostring(L, R"lua(
        function GetSunComponent(entity)
            colour, brightness, direction, shadows = _GetSunComponent(entity)
            return SunComponent(colour, brightness, direction, shadows)
        end
    )lua");

    lua_register(L, "_SetSunComponent", &_SetSunComponent);

    luaL_dostring(L, R"lua(
        function SetSunComponent(entity, c)
            _SetSunComponent(entity, c.colour, c.brightness, c.direction, c.shadows)
        end
    )lua");

    lua_register(L, "_AddSunComponent", &_AddSunComponent);

    luaL_dostring(L, R"lua(
        function AddSunComponent(entity, c)
            _AddSunComponent(entity, c.colour, c.brightness, c.direction, c.shadows)
        end
    )lua");

    lua_register(L, "HasSunComponent", &_has_impl<SunComponent>);


    // Lua functions for AmbienceComponent =====================================================

    luaL_dostring(L, R"lua(
        AmbienceComponent = Class(function(self, colour, brightness)
            self.colour = colour
            self.brightness = brightness
        end)
    )lua");

    lua_register(L, "_GetAmbienceComponent", &_GetAmbienceComponent);

    luaL_dostring(L, R"lua(
        function GetAmbienceComponent(entity)
            colour, brightness = _GetAmbienceComponent(entity)
            return AmbienceComponent(colour, brightness)
        end
    )lua");

    lua_register(L, "_SetAmbienceComponent", &_SetAmbienceComponent);

    luaL_dostring(L, R"lua(
        function SetAmbienceComponent(entity, c)
            _SetAmbienceComponent(entity, c.colour, c.brightness)
        end
    )lua");

    lua_register(L, "_AddAmbienceComponent", &_AddAmbienceComponent);

    luaL_dostring(L, R"lua(
        function AddAmbienceComponent(entity, c)
            _AddAmbienceComponent(entity, c.colour, c.brightness)
        end
    )lua");

    lua_register(L, "HasAmbienceComponent", &_has_impl<AmbienceComponent>);


    // Lua functions for ParticleComponent =====================================================

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

    lua_register(L, "_GetParticleComponent", &_GetParticleComponent);

    luaL_dostring(L, R"lua(
        function GetParticleComponent(entity)
            interval, velocity, velocityNoise, acceleration, scale, life = _GetParticleComponent(entity)
            return ParticleComponent(interval, velocity, velocityNoise, acceleration, scale, life)
        end
    )lua");

    lua_register(L, "_SetParticleComponent", &_SetParticleComponent);

    luaL_dostring(L, R"lua(
        function SetParticleComponent(entity, c)
            _SetParticleComponent(entity, c.interval, c.velocity, c.velocityNoise, c.acceleration, c.scale, c.life)
        end
    )lua");

    lua_register(L, "_AddParticleComponent", &_AddParticleComponent);

    luaL_dostring(L, R"lua(
        function AddParticleComponent(entity, c)
            _AddParticleComponent(entity, c.interval, c.velocity, c.velocityNoise, c.acceleration, c.scale, c.life)
        end
    )lua");

    lua_register(L, "HasParticleComponent", &_has_impl<ParticleComponent>);


    // Lua functions for MeshAnimationComponent =====================================================

    luaL_dostring(L, R"lua(
        MeshAnimationComponent = Class(function(self, name, time, speed)
            self.name = name
            self.time = time
            self.speed = speed
        end)
    )lua");

    lua_register(L, "_GetMeshAnimationComponent", &_GetMeshAnimationComponent);

    luaL_dostring(L, R"lua(
        function GetMeshAnimationComponent(entity)
            name, time, speed = _GetMeshAnimationComponent(entity)
            return MeshAnimationComponent(name, time, speed)
        end
    )lua");

    lua_register(L, "_SetMeshAnimationComponent", &_SetMeshAnimationComponent);

    luaL_dostring(L, R"lua(
        function SetMeshAnimationComponent(entity, c)
            _SetMeshAnimationComponent(entity, c.name, c.time, c.speed)
        end
    )lua");

    lua_register(L, "_AddMeshAnimationComponent", &_AddMeshAnimationComponent);

    luaL_dostring(L, R"lua(
        function AddMeshAnimationComponent(entity, c)
            _AddMeshAnimationComponent(entity, c.name, c.time, c.speed)
        end
    )lua");

    lua_register(L, "HasMeshAnimationComponent", &_has_impl<MeshAnimationComponent>);


    // Lua functions for PhysicsSingleton =====================================================

    luaL_dostring(L, R"lua(
        PhysicsSingleton = Class(function(self, collisions)
            self.collisions = collisions
        end)
    )lua");

    lua_register(L, "_GetPhysicsSingleton", &_GetPhysicsSingleton);

    luaL_dostring(L, R"lua(
        function GetPhysicsSingleton(entity)
            collisions = _GetPhysicsSingleton(entity)
            return PhysicsSingleton(collisions)
        end
    )lua");

    lua_register(L, "_SetPhysicsSingleton", &_SetPhysicsSingleton);

    luaL_dostring(L, R"lua(
        function SetPhysicsSingleton(entity, c)
            _SetPhysicsSingleton(entity, c.collisions)
        end
    )lua");

    lua_register(L, "_AddPhysicsSingleton", &_AddPhysicsSingleton);

    luaL_dostring(L, R"lua(
        function AddPhysicsSingleton(entity, c)
            _AddPhysicsSingleton(entity, c.collisions)
        end
    )lua");

    lua_register(L, "HasPhysicsSingleton", &_has_impl<PhysicsSingleton>);


}

}
}
