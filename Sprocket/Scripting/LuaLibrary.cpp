#include "LuaLibrary.h"

#include <Sprocket/Scene/ecs.h>
#include <Sprocket/Scene/Scene.h>
#include <Sprocket/Scripting/LuaConverter.h>
#include <Sprocket/Scripting/LuaScript.h>
#include <Sprocket/Utility/Log.h>
#include <Sprocket/Utility/Maths.h>

#include <lua.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/trigonometric.hpp>

#include <format>
#include <string>
#include <utility>
#include <vector>

namespace spkt {
namespace lua {
namespace {

template <typename... Ts>
using view_iterator = typename spkt::registry::iterator<Ts...>;

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

bool check_arg_count(lua_State* L, int argc)
{
    int args = lua_gettop(L);
    if (args != argc) {
        log::error("[Lua]: Expected {} args, got {}", argc, args);
        return false;
    }
    return true;
}

template <typename T> int has_impl(lua_State* L)
{
    if (!check_arg_count(L, 1)) { return luaL_error(L, "Bad number of args"); }
    auto entity = Converter<spkt::handle>::read(L, 1);
    Converter<bool>::push(L, entity.has<T>());
    return 1;
}

template <typename T> int remove_impl(lua_State* L)
{
    if (!check_arg_count(L, 1)) { return luaL_error(L, "Bad number of args"); }
    auto entity = Converter<spkt::handle>::read(L, 1);
    add_command(L, [entity]() mutable { entity.remove<T>(); });
    return 0;
}

void add_command(lua_State* L, const std::function<void()>& command)
{
    using command_list_t = std::vector<std::function<void()>>;
    command_list_t& command_list = *get_pointer<command_list_t>(L, "__command_list__");
    command_list.push_back(command);
}

template <typename... Comps>
int view_init(lua_State* L) {
    if (!check_arg_count(L, 0)) { return luaL_error(L, "Bad number of args"); }
    auto view = new view_iterator<Comps...>(get_pointer<spkt::registry>(L, "__registry__")->view<Comps...>());
    lua_pushlightuserdata(L, static_cast<void*>(view));
    return 1;
}

template <typename... Comps>
int view_next(lua_State* L) {
    if (!check_arg_count(L, 1)) { return luaL_error(L, "Bad number of args"); }
    spkt::registry& registry = *get_pointer<spkt::registry>(L, "__registry__");
    auto& view = *static_cast<view_iterator<Comps...>*>(lua_touserdata(L, 1));
    if (view.valid()) {
        auto& entity = *static_cast<spkt::handle*>(lua_newuserdata(L, sizeof(spkt::handle)));
        entity = spkt::handle(registry, *view);
        ++view;
    } else {
        delete &view;
        lua_pushnil(L);
    }
    return 1;
}

std::string view_source_code(std::string_view name)
{
    return std::format(R"lua(
        function view_{0}()
            local view = _view_{0}_init()
            return function() return _view_{0}_next(view) end
        end
    )lua", name);
}

}

void load_entity_transformation_functions(lua::Script& script)
{
    lua_State* L = script.native_handle();

    lua_register(L, "SetLookAt", [](lua_State* L) {
        if (!check_arg_count(L, 3)) { return luaL_error(L, "Bad number of args"); }
        spkt::handle entity = Converter<spkt::handle>::read(L, 1);
        glm::vec3 p = Converter<glm::vec3>::read(L, 2);
        glm::vec3 t = Converter<glm::vec3>::read(L, 3);
        auto& tr = entity.get<Transform3DComponent>();
        tr.position = p;
        tr.orientation = glm::conjugate(glm::quat_cast(glm::lookAt(tr.position, t, {0.0, 1.0, 0.0})));
        return 0;
    });

    lua_register(L, "RotateY", [](lua_State* L) {
        if (!check_arg_count(L, 2)) { return luaL_error(L, "Bad number of args"); };
        spkt::handle entity = *static_cast<spkt::handle*>(lua_touserdata(L, 1));
        auto& tr = entity.get<Transform3DComponent>();
        float yaw = (float)lua_tonumber(L, 2);
        tr.orientation = glm::rotate(tr.orientation, yaw, {0, 1, 0});
        return 0;
    });

    lua_register(L, "GetForwardsDir", [](lua_State* L) {
        if (!check_arg_count(L, 1)) { return luaL_error(L, "Bad number of args"); }
        spkt::handle entity = *static_cast<spkt::handle*>(lua_touserdata(L, 1));
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
        if (!check_arg_count(L, 1)) { return luaL_error(L, "Bad number of args"); }
        spkt::handle entity = Converter<spkt::handle>::read(L, 1);
        auto& tr = entity.get<Transform3DComponent>();
        Converter<glm::vec3>::push(L, Maths::Right(tr.orientation));
        return 1;
    });

    lua_register(L, "MakeUpright", [](lua_State* L) {
        if (!check_arg_count(L, 2)) { return luaL_error(L, "Bad number of args"); }
        spkt::handle entity = Converter<spkt::handle>::read(L, 1);
        auto& tr = entity.get<Transform3DComponent>();
        float yaw = Converter<float>::read(L, 2);
        tr.orientation = glm::quat(glm::vec3(0, yaw, 0));
        return 0;
    });

    lua_register(L, "AreEntitiesEqual", [](lua_State* L) {
        if (!check_arg_count(L, 2)) { return luaL_error(L, "Bad number of args"); }
        spkt::handle entity1 = Converter<spkt::handle>::read(L, 1);
        spkt::handle entity2 = Converter<spkt::handle>::read(L, 2);
        Converter<bool>::push(L, entity1 == entity2);
        return 1;
    });
}

void load_registry_functions(lua::Script& script, spkt::registry& registry)
{
    using namespace spkt;

    lua_State* L = script.native_handle();
    script.set_value("__registry__", &registry);

    // Add functions for creating and destroying entities.
    lua_register(L, "NewEntity", [](lua_State* L) {
        if (!check_arg_count(L, 0)) { return luaL_error(L, "Bad number of args"); }
        spkt::registry& registry = *get_pointer<spkt::registry>(L, "__registry__");
        auto new_entity = spkt::handle(registry, registry.create());
        Converter<spkt::handle>::push(L, new_entity);
        return 1;
    });

    lua_register(L, "DeleteEntity", [](lua_State* L) {
        if (!check_arg_count(L, 1)) { return luaL_error(L, "Bad number of args"); }
        spkt::handle entity = *static_cast<spkt::handle*>(lua_touserdata(L, 1));
        add_command(L, [entity]() mutable { entity.destroy(); });
        return 0;
    });

    lua_register(L, "entity_from_id", [](lua_State* L) {
        if (!check_arg_count(L, 1)) { return luaL_error(L, "Bad number of args"); }
        spkt::registry& registry = *get_pointer<spkt::registry>(L, "__registry__");
        auto id = Converter<spkt::entity>::read(L, 1);
        Converter<spkt::handle>::push(L, {registry, id});
        return 1;
    });

    lua_register(L, "entity_singleton", [](lua_State* L) {
        if (!check_arg_count(L, 0)) { return luaL_error(L, "Bad number of args"); }
        spkt::registry& registry = *get_pointer<spkt::registry>(L, "__registry__");
        auto singleton = registry.find<Singleton>();
        Converter<spkt::handle>::push(L, {registry, singleton});
        return 1;
    });

    // Input access via the singleton component.
    lua_register(L, "IsKeyDown", [](lua_State* L) {
        if (!check_arg_count(L, 1)) { return luaL_error(L, "Bad number of args"); }
        spkt::registry& registry = *get_pointer<spkt::registry>(L, "__registry__");
        const auto& input = get_singleton<InputSingleton>(registry);
        Converter<bool>::push(L, input.keyboard[(int)lua_tointeger(L, 1)]);
        return 1;
    });

    lua_register(L, "IsMouseClicked", [](lua_State* L) {
        if (!check_arg_count(L, 1)) { return luaL_error(L, "Bad number of args"); }
        spkt::registry& registry = *get_pointer<spkt::registry>(L, "__registry__");
        const auto& input = get_singleton<InputSingleton>(registry);
        Converter<bool>::push(L, input.mouse_click[(int)lua_tointeger(L, 1)]);
        return 1;
    });

    lua_register(L, "IsMouseUnclicked", [](lua_State* L) {
        if (!check_arg_count(L, 1)) { return luaL_error(L, "Bad number of args"); }
        spkt::registry& registry = *get_pointer<spkt::registry>(L, "__registry__");
        const auto& input = get_singleton<InputSingleton>(registry);
        Converter<bool>::push(L, input.mouse_unclick[(int)lua_tointeger(L, 1)]);
        return 1;
    });

    lua_register(L, "IsMouseDown", [](lua_State* L) {
        if (!check_arg_count(L, 1)) { return luaL_error(L, "Bad number of args"); }
        spkt::registry& registry = *get_pointer<spkt::registry>(L, "__registry__");
        const auto& input = get_singleton<InputSingleton>(registry);
        Converter<bool>::push(L, input.mouse[(int)lua_tointeger(L, 1)]);
        return 1;
    });

    lua_register(L, "GetMousePos", [](lua_State* L) {
        if (!check_arg_count(L, 0)) { return luaL_error(L, "Bad number of args"); }
        spkt::registry& registry = *get_pointer<spkt::registry>(L, "__registry__");
        const auto& input = get_singleton<InputSingleton>(registry);
        Converter<glm::vec2>::push(L, input.mouse_pos);
        return 1;
    });

    lua_register(L, "GetMouseOffset", [](lua_State* L) {
        if (!check_arg_count(L, 0)) { return luaL_error(L, "Bad number of args"); }
        spkt::registry& registry = *get_pointer<spkt::registry>(L, "__registry__");
        const auto& input = get_singleton<InputSingleton>(registry);
        Converter<glm::vec2>::push(L, input.mouse_offset);
        return 1;
    });

    lua_register(L, "GetMouseScrolled", [](lua_State* L) {
        if (!check_arg_count(L, 0)) { return luaL_error(L, "Bad number of args"); }
        spkt::registry& registry = *get_pointer<spkt::registry>(L, "__registry__");
        const auto& input = get_singleton<InputSingleton>(registry);
        Converter<glm::vec2>::push(L, input.mouse_scrolled);
        return 1;
    });

    // Add functions for iterating over all entities in __scene__. The C++ functions
    // should not be used directly, instead they should be used via the Scene:Each
    // function implemented last in Lua.
    lua_register(L, "_view_All_init", view_init<>);
    lua_register(L, "_view_All_next", view_next<>);
    luaL_dostring(L, view_source_code("All").c_str());

// VIEW FUNCTIONS - GENERATED BY DATAMATIC
    lua_register(L, "_view_NameComponent_init", view_init<NameComponent>);
    lua_register(L, "_view_NameComponent_next", view_next<NameComponent>);
    luaL_dostring(L, view_source_code("NameComponent").c_str());

    lua_register(L, "_view_Transform2DComponent_init", view_init<Transform2DComponent>);
    lua_register(L, "_view_Transform2DComponent_next", view_next<Transform2DComponent>);
    luaL_dostring(L, view_source_code("Transform2DComponent").c_str());

    lua_register(L, "_view_Transform3DComponent_init", view_init<Transform3DComponent>);
    lua_register(L, "_view_Transform3DComponent_next", view_next<Transform3DComponent>);
    luaL_dostring(L, view_source_code("Transform3DComponent").c_str());

    lua_register(L, "_view_StaticModelComponent_init", view_init<StaticModelComponent>);
    lua_register(L, "_view_StaticModelComponent_next", view_next<StaticModelComponent>);
    luaL_dostring(L, view_source_code("StaticModelComponent").c_str());

    lua_register(L, "_view_AnimatedModelComponent_init", view_init<AnimatedModelComponent>);
    lua_register(L, "_view_AnimatedModelComponent_next", view_next<AnimatedModelComponent>);
    luaL_dostring(L, view_source_code("AnimatedModelComponent").c_str());

    lua_register(L, "_view_RigidBody3DComponent_init", view_init<RigidBody3DComponent>);
    lua_register(L, "_view_RigidBody3DComponent_next", view_next<RigidBody3DComponent>);
    luaL_dostring(L, view_source_code("RigidBody3DComponent").c_str());

    lua_register(L, "_view_BoxCollider3DComponent_init", view_init<BoxCollider3DComponent>);
    lua_register(L, "_view_BoxCollider3DComponent_next", view_next<BoxCollider3DComponent>);
    luaL_dostring(L, view_source_code("BoxCollider3DComponent").c_str());

    lua_register(L, "_view_SphereCollider3DComponent_init", view_init<SphereCollider3DComponent>);
    lua_register(L, "_view_SphereCollider3DComponent_next", view_next<SphereCollider3DComponent>);
    luaL_dostring(L, view_source_code("SphereCollider3DComponent").c_str());

    lua_register(L, "_view_CapsuleCollider3DComponent_init", view_init<CapsuleCollider3DComponent>);
    lua_register(L, "_view_CapsuleCollider3DComponent_next", view_next<CapsuleCollider3DComponent>);
    luaL_dostring(L, view_source_code("CapsuleCollider3DComponent").c_str());

    lua_register(L, "_view_ScriptComponent_init", view_init<ScriptComponent>);
    lua_register(L, "_view_ScriptComponent_next", view_next<ScriptComponent>);
    luaL_dostring(L, view_source_code("ScriptComponent").c_str());

    lua_register(L, "_view_Camera3DComponent_init", view_init<Camera3DComponent>);
    lua_register(L, "_view_Camera3DComponent_next", view_next<Camera3DComponent>);
    luaL_dostring(L, view_source_code("Camera3DComponent").c_str());

    lua_register(L, "_view_PathComponent_init", view_init<PathComponent>);
    lua_register(L, "_view_PathComponent_next", view_next<PathComponent>);
    luaL_dostring(L, view_source_code("PathComponent").c_str());

    lua_register(L, "_view_LightComponent_init", view_init<LightComponent>);
    lua_register(L, "_view_LightComponent_next", view_next<LightComponent>);
    luaL_dostring(L, view_source_code("LightComponent").c_str());

    lua_register(L, "_view_SunComponent_init", view_init<SunComponent>);
    lua_register(L, "_view_SunComponent_next", view_next<SunComponent>);
    luaL_dostring(L, view_source_code("SunComponent").c_str());

    lua_register(L, "_view_AmbienceComponent_init", view_init<AmbienceComponent>);
    lua_register(L, "_view_AmbienceComponent_next", view_next<AmbienceComponent>);
    luaL_dostring(L, view_source_code("AmbienceComponent").c_str());

    lua_register(L, "_view_ParticleComponent_init", view_init<ParticleComponent>);
    lua_register(L, "_view_ParticleComponent_next", view_next<ParticleComponent>);
    luaL_dostring(L, view_source_code("ParticleComponent").c_str());

    lua_register(L, "_view_CollisionEvent_init", view_init<CollisionEvent>);
    lua_register(L, "_view_CollisionEvent_next", view_next<CollisionEvent>);
    luaL_dostring(L, view_source_code("CollisionEvent").c_str());

}

// COMPONENT RELATED CODE - GENERATED BY DATAMATIC
// C++ Functions for NameComponent =====================================================

int _GetNameComponent(lua_State* L) {
    if (!check_arg_count(L, 1)) { return luaL_error(L, "Bad number of args"); }
    spkt::handle e = Converter<spkt::handle>::read(L, 1);
    assert(e.has<NameComponent>());
    const auto& c = e.get<NameComponent>();
    Converter<std::string>::push(L, c.name);
    return 1;
}

int _SetNameComponent(lua_State* L) {
    if (!check_arg_count(L, 1 + 1)) { return luaL_error(L, "Bad number of args"); }
    int ptr = 0;
    spkt::handle e = Converter<spkt::handle>::read(L, ++ptr);
    auto& c = e.get<NameComponent>();
    c.name = Converter<std::string>::read(L, ++ptr);
    return 0;
}

int _AddNameComponent(lua_State* L) {
    if (!check_arg_count(L, 1 + 1)) { return luaL_error(L, "Bad number of args"); }
    int ptr = 0;
    spkt::handle e = Converter<spkt::handle>::read(L, ++ptr);
    assert(!e.has<NameComponent>());
    NameComponent c;
    c.name = Converter<std::string>::read(L, ++ptr);
    add_command(L, [e, c]() mutable { e.add<NameComponent>(c); });
    return 0;
}

// C++ Functions for Transform2DComponent =====================================================

int _GetTransform2DComponent(lua_State* L) {
    if (!check_arg_count(L, 1)) { return luaL_error(L, "Bad number of args"); }
    spkt::handle e = Converter<spkt::handle>::read(L, 1);
    assert(e.has<Transform2DComponent>());
    const auto& c = e.get<Transform2DComponent>();
    Converter<glm::vec2>::push(L, c.position);
    Converter<float>::push(L, c.rotation);
    Converter<glm::vec2>::push(L, c.scale);
    return 3;
}

int _SetTransform2DComponent(lua_State* L) {
    if (!check_arg_count(L, 3 + 1)) { return luaL_error(L, "Bad number of args"); }
    int ptr = 0;
    spkt::handle e = Converter<spkt::handle>::read(L, ++ptr);
    auto& c = e.get<Transform2DComponent>();
    c.position = Converter<glm::vec2>::read(L, ++ptr);
    c.rotation = Converter<float>::read(L, ++ptr);
    c.scale = Converter<glm::vec2>::read(L, ++ptr);
    return 0;
}

int _AddTransform2DComponent(lua_State* L) {
    if (!check_arg_count(L, 3 + 1)) { return luaL_error(L, "Bad number of args"); }
    int ptr = 0;
    spkt::handle e = Converter<spkt::handle>::read(L, ++ptr);
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
    if (!check_arg_count(L, 1)) { return luaL_error(L, "Bad number of args"); }
    spkt::handle e = Converter<spkt::handle>::read(L, 1);
    assert(e.has<Transform3DComponent>());
    const auto& c = e.get<Transform3DComponent>();
    Converter<glm::vec3>::push(L, c.position);
    Converter<glm::vec3>::push(L, c.scale);
    return 2;
}

int _SetTransform3DComponent(lua_State* L) {
    if (!check_arg_count(L, 2 + 1)) { return luaL_error(L, "Bad number of args"); }
    int ptr = 0;
    spkt::handle e = Converter<spkt::handle>::read(L, ++ptr);
    auto& c = e.get<Transform3DComponent>();
    c.position = Converter<glm::vec3>::read(L, ++ptr);
    c.scale = Converter<glm::vec3>::read(L, ++ptr);
    return 0;
}

int _AddTransform3DComponent(lua_State* L) {
    if (!check_arg_count(L, 2 + 1)) { return luaL_error(L, "Bad number of args"); }
    int ptr = 0;
    spkt::handle e = Converter<spkt::handle>::read(L, ++ptr);
    assert(!e.has<Transform3DComponent>());
    Transform3DComponent c;
    c.position = Converter<glm::vec3>::read(L, ++ptr);
    c.scale = Converter<glm::vec3>::read(L, ++ptr);
    add_command(L, [e, c]() mutable { e.add<Transform3DComponent>(c); });
    return 0;
}

// C++ Functions for StaticModelComponent =====================================================

int _GetStaticModelComponent(lua_State* L) {
    if (!check_arg_count(L, 1)) { return luaL_error(L, "Bad number of args"); }
    spkt::handle e = Converter<spkt::handle>::read(L, 1);
    assert(e.has<StaticModelComponent>());
    const auto& c = e.get<StaticModelComponent>();
    Converter<std::string>::push(L, c.mesh);
    Converter<std::string>::push(L, c.material);
    return 2;
}

int _SetStaticModelComponent(lua_State* L) {
    if (!check_arg_count(L, 2 + 1)) { return luaL_error(L, "Bad number of args"); }
    int ptr = 0;
    spkt::handle e = Converter<spkt::handle>::read(L, ++ptr);
    auto& c = e.get<StaticModelComponent>();
    c.mesh = Converter<std::string>::read(L, ++ptr);
    c.material = Converter<std::string>::read(L, ++ptr);
    return 0;
}

int _AddStaticModelComponent(lua_State* L) {
    if (!check_arg_count(L, 2 + 1)) { return luaL_error(L, "Bad number of args"); }
    int ptr = 0;
    spkt::handle e = Converter<spkt::handle>::read(L, ++ptr);
    assert(!e.has<StaticModelComponent>());
    StaticModelComponent c;
    c.mesh = Converter<std::string>::read(L, ++ptr);
    c.material = Converter<std::string>::read(L, ++ptr);
    add_command(L, [e, c]() mutable { e.add<StaticModelComponent>(c); });
    return 0;
}

// C++ Functions for AnimatedModelComponent =====================================================

int _GetAnimatedModelComponent(lua_State* L) {
    if (!check_arg_count(L, 1)) { return luaL_error(L, "Bad number of args"); }
    spkt::handle e = Converter<spkt::handle>::read(L, 1);
    assert(e.has<AnimatedModelComponent>());
    const auto& c = e.get<AnimatedModelComponent>();
    Converter<std::string>::push(L, c.mesh);
    Converter<std::string>::push(L, c.material);
    Converter<std::string>::push(L, c.animation_name);
    Converter<float>::push(L, c.animation_time);
    Converter<float>::push(L, c.animation_speed);
    return 5;
}

int _SetAnimatedModelComponent(lua_State* L) {
    if (!check_arg_count(L, 5 + 1)) { return luaL_error(L, "Bad number of args"); }
    int ptr = 0;
    spkt::handle e = Converter<spkt::handle>::read(L, ++ptr);
    auto& c = e.get<AnimatedModelComponent>();
    c.mesh = Converter<std::string>::read(L, ++ptr);
    c.material = Converter<std::string>::read(L, ++ptr);
    c.animation_name = Converter<std::string>::read(L, ++ptr);
    c.animation_time = Converter<float>::read(L, ++ptr);
    c.animation_speed = Converter<float>::read(L, ++ptr);
    return 0;
}

int _AddAnimatedModelComponent(lua_State* L) {
    if (!check_arg_count(L, 5 + 1)) { return luaL_error(L, "Bad number of args"); }
    int ptr = 0;
    spkt::handle e = Converter<spkt::handle>::read(L, ++ptr);
    assert(!e.has<AnimatedModelComponent>());
    AnimatedModelComponent c;
    c.mesh = Converter<std::string>::read(L, ++ptr);
    c.material = Converter<std::string>::read(L, ++ptr);
    c.animation_name = Converter<std::string>::read(L, ++ptr);
    c.animation_time = Converter<float>::read(L, ++ptr);
    c.animation_speed = Converter<float>::read(L, ++ptr);
    add_command(L, [e, c]() mutable { e.add<AnimatedModelComponent>(c); });
    return 0;
}

// C++ Functions for RigidBody3DComponent =====================================================

int _GetRigidBody3DComponent(lua_State* L) {
    if (!check_arg_count(L, 1)) { return luaL_error(L, "Bad number of args"); }
    spkt::handle e = Converter<spkt::handle>::read(L, 1);
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
    if (!check_arg_count(L, 8 + 1)) { return luaL_error(L, "Bad number of args"); }
    int ptr = 0;
    spkt::handle e = Converter<spkt::handle>::read(L, ++ptr);
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
    if (!check_arg_count(L, 8 + 1)) { return luaL_error(L, "Bad number of args"); }
    int ptr = 0;
    spkt::handle e = Converter<spkt::handle>::read(L, ++ptr);
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
    if (!check_arg_count(L, 1)) { return luaL_error(L, "Bad number of args"); }
    spkt::handle e = Converter<spkt::handle>::read(L, 1);
    assert(e.has<BoxCollider3DComponent>());
    const auto& c = e.get<BoxCollider3DComponent>();
    Converter<glm::vec3>::push(L, c.position);
    Converter<float>::push(L, c.mass);
    Converter<glm::vec3>::push(L, c.halfExtents);
    Converter<bool>::push(L, c.applyScale);
    return 4;
}

int _SetBoxCollider3DComponent(lua_State* L) {
    if (!check_arg_count(L, 4 + 1)) { return luaL_error(L, "Bad number of args"); }
    int ptr = 0;
    spkt::handle e = Converter<spkt::handle>::read(L, ++ptr);
    auto& c = e.get<BoxCollider3DComponent>();
    c.position = Converter<glm::vec3>::read(L, ++ptr);
    c.mass = Converter<float>::read(L, ++ptr);
    c.halfExtents = Converter<glm::vec3>::read(L, ++ptr);
    c.applyScale = Converter<bool>::read(L, ++ptr);
    return 0;
}

int _AddBoxCollider3DComponent(lua_State* L) {
    if (!check_arg_count(L, 4 + 1)) { return luaL_error(L, "Bad number of args"); }
    int ptr = 0;
    spkt::handle e = Converter<spkt::handle>::read(L, ++ptr);
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
    if (!check_arg_count(L, 1)) { return luaL_error(L, "Bad number of args"); }
    spkt::handle e = Converter<spkt::handle>::read(L, 1);
    assert(e.has<SphereCollider3DComponent>());
    const auto& c = e.get<SphereCollider3DComponent>();
    Converter<glm::vec3>::push(L, c.position);
    Converter<float>::push(L, c.mass);
    Converter<float>::push(L, c.radius);
    return 3;
}

int _SetSphereCollider3DComponent(lua_State* L) {
    if (!check_arg_count(L, 3 + 1)) { return luaL_error(L, "Bad number of args"); }
    int ptr = 0;
    spkt::handle e = Converter<spkt::handle>::read(L, ++ptr);
    auto& c = e.get<SphereCollider3DComponent>();
    c.position = Converter<glm::vec3>::read(L, ++ptr);
    c.mass = Converter<float>::read(L, ++ptr);
    c.radius = Converter<float>::read(L, ++ptr);
    return 0;
}

int _AddSphereCollider3DComponent(lua_State* L) {
    if (!check_arg_count(L, 3 + 1)) { return luaL_error(L, "Bad number of args"); }
    int ptr = 0;
    spkt::handle e = Converter<spkt::handle>::read(L, ++ptr);
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
    if (!check_arg_count(L, 1)) { return luaL_error(L, "Bad number of args"); }
    spkt::handle e = Converter<spkt::handle>::read(L, 1);
    assert(e.has<CapsuleCollider3DComponent>());
    const auto& c = e.get<CapsuleCollider3DComponent>();
    Converter<glm::vec3>::push(L, c.position);
    Converter<float>::push(L, c.mass);
    Converter<float>::push(L, c.radius);
    Converter<float>::push(L, c.height);
    return 4;
}

int _SetCapsuleCollider3DComponent(lua_State* L) {
    if (!check_arg_count(L, 4 + 1)) { return luaL_error(L, "Bad number of args"); }
    int ptr = 0;
    spkt::handle e = Converter<spkt::handle>::read(L, ++ptr);
    auto& c = e.get<CapsuleCollider3DComponent>();
    c.position = Converter<glm::vec3>::read(L, ++ptr);
    c.mass = Converter<float>::read(L, ++ptr);
    c.radius = Converter<float>::read(L, ++ptr);
    c.height = Converter<float>::read(L, ++ptr);
    return 0;
}

int _AddCapsuleCollider3DComponent(lua_State* L) {
    if (!check_arg_count(L, 4 + 1)) { return luaL_error(L, "Bad number of args"); }
    int ptr = 0;
    spkt::handle e = Converter<spkt::handle>::read(L, ++ptr);
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
    if (!check_arg_count(L, 1)) { return luaL_error(L, "Bad number of args"); }
    spkt::handle e = Converter<spkt::handle>::read(L, 1);
    assert(e.has<ScriptComponent>());
    const auto& c = e.get<ScriptComponent>();
    Converter<std::string>::push(L, c.script);
    Converter<bool>::push(L, c.active);
    return 2;
}

int _SetScriptComponent(lua_State* L) {
    if (!check_arg_count(L, 2 + 1)) { return luaL_error(L, "Bad number of args"); }
    int ptr = 0;
    spkt::handle e = Converter<spkt::handle>::read(L, ++ptr);
    auto& c = e.get<ScriptComponent>();
    c.script = Converter<std::string>::read(L, ++ptr);
    c.active = Converter<bool>::read(L, ++ptr);
    return 0;
}

int _AddScriptComponent(lua_State* L) {
    if (!check_arg_count(L, 2 + 1)) { return luaL_error(L, "Bad number of args"); }
    int ptr = 0;
    spkt::handle e = Converter<spkt::handle>::read(L, ++ptr);
    assert(!e.has<ScriptComponent>());
    ScriptComponent c;
    c.script = Converter<std::string>::read(L, ++ptr);
    c.active = Converter<bool>::read(L, ++ptr);
    add_command(L, [e, c]() mutable { e.add<ScriptComponent>(c); });
    return 0;
}

// C++ Functions for Camera3DComponent =====================================================

int _GetCamera3DComponent(lua_State* L) {
    if (!check_arg_count(L, 1)) { return luaL_error(L, "Bad number of args"); }
    spkt::handle e = Converter<spkt::handle>::read(L, 1);
    assert(e.has<Camera3DComponent>());
    const auto& c = e.get<Camera3DComponent>();
    Converter<float>::push(L, c.fov);
    Converter<float>::push(L, c.pitch);
    return 2;
}

int _SetCamera3DComponent(lua_State* L) {
    if (!check_arg_count(L, 2 + 1)) { return luaL_error(L, "Bad number of args"); }
    int ptr = 0;
    spkt::handle e = Converter<spkt::handle>::read(L, ++ptr);
    auto& c = e.get<Camera3DComponent>();
    c.fov = Converter<float>::read(L, ++ptr);
    c.pitch = Converter<float>::read(L, ++ptr);
    return 0;
}

int _AddCamera3DComponent(lua_State* L) {
    if (!check_arg_count(L, 2 + 1)) { return luaL_error(L, "Bad number of args"); }
    int ptr = 0;
    spkt::handle e = Converter<spkt::handle>::read(L, ++ptr);
    assert(!e.has<Camera3DComponent>());
    Camera3DComponent c;
    c.fov = Converter<float>::read(L, ++ptr);
    c.pitch = Converter<float>::read(L, ++ptr);
    add_command(L, [e, c]() mutable { e.add<Camera3DComponent>(c); });
    return 0;
}

// C++ Functions for PathComponent =====================================================

int _GetPathComponent(lua_State* L) {
    if (!check_arg_count(L, 1)) { return luaL_error(L, "Bad number of args"); }
    spkt::handle e = Converter<spkt::handle>::read(L, 1);
    assert(e.has<PathComponent>());
    const auto& c = e.get<PathComponent>();
    Converter<float>::push(L, c.speed);
    return 1;
}

int _SetPathComponent(lua_State* L) {
    if (!check_arg_count(L, 1 + 1)) { return luaL_error(L, "Bad number of args"); }
    int ptr = 0;
    spkt::handle e = Converter<spkt::handle>::read(L, ++ptr);
    auto& c = e.get<PathComponent>();
    c.speed = Converter<float>::read(L, ++ptr);
    return 0;
}

int _AddPathComponent(lua_State* L) {
    if (!check_arg_count(L, 1 + 1)) { return luaL_error(L, "Bad number of args"); }
    int ptr = 0;
    spkt::handle e = Converter<spkt::handle>::read(L, ++ptr);
    assert(!e.has<PathComponent>());
    PathComponent c;
    c.speed = Converter<float>::read(L, ++ptr);
    add_command(L, [e, c]() mutable { e.add<PathComponent>(c); });
    return 0;
}

// C++ Functions for LightComponent =====================================================

int _GetLightComponent(lua_State* L) {
    if (!check_arg_count(L, 1)) { return luaL_error(L, "Bad number of args"); }
    spkt::handle e = Converter<spkt::handle>::read(L, 1);
    assert(e.has<LightComponent>());
    const auto& c = e.get<LightComponent>();
    Converter<glm::vec3>::push(L, c.colour);
    Converter<float>::push(L, c.brightness);
    return 2;
}

int _SetLightComponent(lua_State* L) {
    if (!check_arg_count(L, 2 + 1)) { return luaL_error(L, "Bad number of args"); }
    int ptr = 0;
    spkt::handle e = Converter<spkt::handle>::read(L, ++ptr);
    auto& c = e.get<LightComponent>();
    c.colour = Converter<glm::vec3>::read(L, ++ptr);
    c.brightness = Converter<float>::read(L, ++ptr);
    return 0;
}

int _AddLightComponent(lua_State* L) {
    if (!check_arg_count(L, 2 + 1)) { return luaL_error(L, "Bad number of args"); }
    int ptr = 0;
    spkt::handle e = Converter<spkt::handle>::read(L, ++ptr);
    assert(!e.has<LightComponent>());
    LightComponent c;
    c.colour = Converter<glm::vec3>::read(L, ++ptr);
    c.brightness = Converter<float>::read(L, ++ptr);
    add_command(L, [e, c]() mutable { e.add<LightComponent>(c); });
    return 0;
}

// C++ Functions for SunComponent =====================================================

int _GetSunComponent(lua_State* L) {
    if (!check_arg_count(L, 1)) { return luaL_error(L, "Bad number of args"); }
    spkt::handle e = Converter<spkt::handle>::read(L, 1);
    assert(e.has<SunComponent>());
    const auto& c = e.get<SunComponent>();
    Converter<glm::vec3>::push(L, c.colour);
    Converter<float>::push(L, c.brightness);
    Converter<glm::vec3>::push(L, c.direction);
    Converter<bool>::push(L, c.shadows);
    return 4;
}

int _SetSunComponent(lua_State* L) {
    if (!check_arg_count(L, 4 + 1)) { return luaL_error(L, "Bad number of args"); }
    int ptr = 0;
    spkt::handle e = Converter<spkt::handle>::read(L, ++ptr);
    auto& c = e.get<SunComponent>();
    c.colour = Converter<glm::vec3>::read(L, ++ptr);
    c.brightness = Converter<float>::read(L, ++ptr);
    c.direction = Converter<glm::vec3>::read(L, ++ptr);
    c.shadows = Converter<bool>::read(L, ++ptr);
    return 0;
}

int _AddSunComponent(lua_State* L) {
    if (!check_arg_count(L, 4 + 1)) { return luaL_error(L, "Bad number of args"); }
    int ptr = 0;
    spkt::handle e = Converter<spkt::handle>::read(L, ++ptr);
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
    if (!check_arg_count(L, 1)) { return luaL_error(L, "Bad number of args"); }
    spkt::handle e = Converter<spkt::handle>::read(L, 1);
    assert(e.has<AmbienceComponent>());
    const auto& c = e.get<AmbienceComponent>();
    Converter<glm::vec3>::push(L, c.colour);
    Converter<float>::push(L, c.brightness);
    return 2;
}

int _SetAmbienceComponent(lua_State* L) {
    if (!check_arg_count(L, 2 + 1)) { return luaL_error(L, "Bad number of args"); }
    int ptr = 0;
    spkt::handle e = Converter<spkt::handle>::read(L, ++ptr);
    auto& c = e.get<AmbienceComponent>();
    c.colour = Converter<glm::vec3>::read(L, ++ptr);
    c.brightness = Converter<float>::read(L, ++ptr);
    return 0;
}

int _AddAmbienceComponent(lua_State* L) {
    if (!check_arg_count(L, 2 + 1)) { return luaL_error(L, "Bad number of args"); }
    int ptr = 0;
    spkt::handle e = Converter<spkt::handle>::read(L, ++ptr);
    assert(!e.has<AmbienceComponent>());
    AmbienceComponent c;
    c.colour = Converter<glm::vec3>::read(L, ++ptr);
    c.brightness = Converter<float>::read(L, ++ptr);
    add_command(L, [e, c]() mutable { e.add<AmbienceComponent>(c); });
    return 0;
}

// C++ Functions for ParticleComponent =====================================================

int _GetParticleComponent(lua_State* L) {
    if (!check_arg_count(L, 1)) { return luaL_error(L, "Bad number of args"); }
    spkt::handle e = Converter<spkt::handle>::read(L, 1);
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
    if (!check_arg_count(L, 6 + 1)) { return luaL_error(L, "Bad number of args"); }
    int ptr = 0;
    spkt::handle e = Converter<spkt::handle>::read(L, ++ptr);
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
    if (!check_arg_count(L, 6 + 1)) { return luaL_error(L, "Bad number of args"); }
    int ptr = 0;
    spkt::handle e = Converter<spkt::handle>::read(L, ++ptr);
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

// C++ Functions for CollisionEvent =====================================================

int _GetCollisionEvent(lua_State* L) {
    if (!check_arg_count(L, 1)) { return luaL_error(L, "Bad number of args"); }
    spkt::handle e = Converter<spkt::handle>::read(L, 1);
    assert(e.has<CollisionEvent>());
    const auto& c = e.get<CollisionEvent>();
    Converter<spkt::entity>::push(L, c.entity_a);
    Converter<spkt::entity>::push(L, c.entity_b);
    return 2;
}

int _SetCollisionEvent(lua_State* L) {
    if (!check_arg_count(L, 2 + 1)) { return luaL_error(L, "Bad number of args"); }
    int ptr = 0;
    spkt::handle e = Converter<spkt::handle>::read(L, ++ptr);
    auto& c = e.get<CollisionEvent>();
    c.entity_a = Converter<spkt::entity>::read(L, ++ptr);
    c.entity_b = Converter<spkt::entity>::read(L, ++ptr);
    return 0;
}

int _AddCollisionEvent(lua_State* L) {
    if (!check_arg_count(L, 2 + 1)) { return luaL_error(L, "Bad number of args"); }
    int ptr = 0;
    spkt::handle e = Converter<spkt::handle>::read(L, ++ptr);
    assert(!e.has<CollisionEvent>());
    CollisionEvent c;
    c.entity_a = Converter<spkt::entity>::read(L, ++ptr);
    c.entity_b = Converter<spkt::entity>::read(L, ++ptr);
    add_command(L, [e, c]() mutable { e.add<CollisionEvent>(c); });
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

    lua_register(L, "HasNameComponent", &has_impl<NameComponent>);
    lua_register(L, "RemoveNameComponent", &remove_impl<NameComponent>);


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

    lua_register(L, "HasTransform2DComponent", &has_impl<Transform2DComponent>);
    lua_register(L, "RemoveTransform2DComponent", &remove_impl<Transform2DComponent>);


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

    lua_register(L, "HasTransform3DComponent", &has_impl<Transform3DComponent>);
    lua_register(L, "RemoveTransform3DComponent", &remove_impl<Transform3DComponent>);


    // Lua functions for StaticModelComponent =====================================================

    luaL_dostring(L, R"lua(
        StaticModelComponent = Class(function(self, mesh, material)
            self.mesh = mesh
            self.material = material
        end)
    )lua");

    lua_register(L, "_GetStaticModelComponent", &_GetStaticModelComponent);

    luaL_dostring(L, R"lua(
        function GetStaticModelComponent(entity)
            mesh, material = _GetStaticModelComponent(entity)
            return StaticModelComponent(mesh, material)
        end
    )lua");

    lua_register(L, "_SetStaticModelComponent", &_SetStaticModelComponent);

    luaL_dostring(L, R"lua(
        function SetStaticModelComponent(entity, c)
            _SetStaticModelComponent(entity, c.mesh, c.material)
        end
    )lua");

    lua_register(L, "_AddStaticModelComponent", &_AddStaticModelComponent);

    luaL_dostring(L, R"lua(
        function AddStaticModelComponent(entity, c)
            _AddStaticModelComponent(entity, c.mesh, c.material)
        end
    )lua");

    lua_register(L, "HasStaticModelComponent", &has_impl<StaticModelComponent>);
    lua_register(L, "RemoveStaticModelComponent", &remove_impl<StaticModelComponent>);


    // Lua functions for AnimatedModelComponent =====================================================

    luaL_dostring(L, R"lua(
        AnimatedModelComponent = Class(function(self, mesh, material, animation_name, animation_time, animation_speed)
            self.mesh = mesh
            self.material = material
            self.animation_name = animation_name
            self.animation_time = animation_time
            self.animation_speed = animation_speed
        end)
    )lua");

    lua_register(L, "_GetAnimatedModelComponent", &_GetAnimatedModelComponent);

    luaL_dostring(L, R"lua(
        function GetAnimatedModelComponent(entity)
            mesh, material, animation_name, animation_time, animation_speed = _GetAnimatedModelComponent(entity)
            return AnimatedModelComponent(mesh, material, animation_name, animation_time, animation_speed)
        end
    )lua");

    lua_register(L, "_SetAnimatedModelComponent", &_SetAnimatedModelComponent);

    luaL_dostring(L, R"lua(
        function SetAnimatedModelComponent(entity, c)
            _SetAnimatedModelComponent(entity, c.mesh, c.material, c.animation_name, c.animation_time, c.animation_speed)
        end
    )lua");

    lua_register(L, "_AddAnimatedModelComponent", &_AddAnimatedModelComponent);

    luaL_dostring(L, R"lua(
        function AddAnimatedModelComponent(entity, c)
            _AddAnimatedModelComponent(entity, c.mesh, c.material, c.animation_name, c.animation_time, c.animation_speed)
        end
    )lua");

    lua_register(L, "HasAnimatedModelComponent", &has_impl<AnimatedModelComponent>);
    lua_register(L, "RemoveAnimatedModelComponent", &remove_impl<AnimatedModelComponent>);


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

    lua_register(L, "HasRigidBody3DComponent", &has_impl<RigidBody3DComponent>);
    lua_register(L, "RemoveRigidBody3DComponent", &remove_impl<RigidBody3DComponent>);


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

    lua_register(L, "HasBoxCollider3DComponent", &has_impl<BoxCollider3DComponent>);
    lua_register(L, "RemoveBoxCollider3DComponent", &remove_impl<BoxCollider3DComponent>);


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

    lua_register(L, "HasSphereCollider3DComponent", &has_impl<SphereCollider3DComponent>);
    lua_register(L, "RemoveSphereCollider3DComponent", &remove_impl<SphereCollider3DComponent>);


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

    lua_register(L, "HasCapsuleCollider3DComponent", &has_impl<CapsuleCollider3DComponent>);
    lua_register(L, "RemoveCapsuleCollider3DComponent", &remove_impl<CapsuleCollider3DComponent>);


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

    lua_register(L, "HasScriptComponent", &has_impl<ScriptComponent>);
    lua_register(L, "RemoveScriptComponent", &remove_impl<ScriptComponent>);


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

    lua_register(L, "HasCamera3DComponent", &has_impl<Camera3DComponent>);
    lua_register(L, "RemoveCamera3DComponent", &remove_impl<Camera3DComponent>);


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

    lua_register(L, "HasPathComponent", &has_impl<PathComponent>);
    lua_register(L, "RemovePathComponent", &remove_impl<PathComponent>);


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

    lua_register(L, "HasLightComponent", &has_impl<LightComponent>);
    lua_register(L, "RemoveLightComponent", &remove_impl<LightComponent>);


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

    lua_register(L, "HasSunComponent", &has_impl<SunComponent>);
    lua_register(L, "RemoveSunComponent", &remove_impl<SunComponent>);


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

    lua_register(L, "HasAmbienceComponent", &has_impl<AmbienceComponent>);
    lua_register(L, "RemoveAmbienceComponent", &remove_impl<AmbienceComponent>);


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

    lua_register(L, "HasParticleComponent", &has_impl<ParticleComponent>);
    lua_register(L, "RemoveParticleComponent", &remove_impl<ParticleComponent>);


    // Lua functions for CollisionEvent =====================================================

    luaL_dostring(L, R"lua(
        CollisionEvent = Class(function(self, entity_a, entity_b)
            self.entity_a = entity_a
            self.entity_b = entity_b
        end)
    )lua");

    lua_register(L, "_GetCollisionEvent", &_GetCollisionEvent);

    luaL_dostring(L, R"lua(
        function GetCollisionEvent(entity)
            entity_a, entity_b = _GetCollisionEvent(entity)
            return CollisionEvent(entity_a, entity_b)
        end
    )lua");

    lua_register(L, "_SetCollisionEvent", &_SetCollisionEvent);

    luaL_dostring(L, R"lua(
        function SetCollisionEvent(entity, c)
            _SetCollisionEvent(entity, c.entity_a, c.entity_b)
        end
    )lua");

    lua_register(L, "_AddCollisionEvent", &_AddCollisionEvent);

    luaL_dostring(L, R"lua(
        function AddCollisionEvent(entity, c)
            _AddCollisionEvent(entity, c.entity_a, c.entity_b)
        end
    )lua");

    lua_register(L, "HasCollisionEvent", &has_impl<CollisionEvent>);
    lua_register(L, "RemoveCollisionEvent", &remove_impl<CollisionEvent>);


}

}
}
