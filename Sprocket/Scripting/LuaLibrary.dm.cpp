#include "LuaLibrary.h"

#include <Sprocket/Scene/ecs.h>
#include <Sprocket/Scene/scene.h>
#include <Sprocket/Scripting/LuaConverter.h>
#include <Sprocket/Scripting/LuaScript.h>
#include <Sprocket/Core/log.h>
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
    auto& reg = *get_pointer<spkt::registry>(L, "__registry__");
    auto entity = Converter<spkt::entity>::read(L, 1);
    Converter<bool>::push(L, reg.has<T>(entity));
    return 1;
}

template <typename T> int remove_impl(lua_State* L)
{
    if (!check_arg_count(L, 1)) { return luaL_error(L, "Bad number of args"); }
    auto& reg = *get_pointer<spkt::registry>(L, "__registry__");
    auto entity = Converter<spkt::entity>::read(L, 1);
    add_command(L, [&, entity]() { reg.remove<T>(entity); });
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
    auto& reg = *get_pointer<spkt::registry>(L, "__registry__");
    using view_t = decltype(reg.view<Comps...>());
    using iter_t = decltype(std::declval<view_t>().begin());

    auto view = new view_t(reg.view<Comps...>());
    auto iter = new iter_t(view->begin());
    lua_pushlightuserdata(L, static_cast<void*>(view));
    lua_pushlightuserdata(L, static_cast<void*>(iter));
    return 2;
}

template <typename... Comps>
int view_next(lua_State* L) {
    if (!check_arg_count(L, 2)) { return luaL_error(L, "Bad number of args"); }
    auto& reg = *get_pointer<spkt::registry>(L, "__registry__");
    using view_t = decltype(reg.view<Comps...>());
    using iter_t = decltype(std::declval<view_t>().begin());
    
    auto& view = *static_cast<view_t*>(lua_touserdata(L, 1));
    auto& iter = *static_cast<iter_t*>(lua_touserdata(L, 2));
    if (iter != view.end()) {
        Converter<spkt::entity>::push(L, *iter);
        ++iter;
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
            local view, iter = _view_{0}_init()
            return function() return _view_{0}_next(view, iter) end
        end
    )lua", name);
}

}

void load_entity_transformation_functions(lua::Script& script)
{
    lua_State* L = script.native_handle();

    lua_register(L, "SetLookAt", [](lua_State* L) {
        if (!check_arg_count(L, 3)) { return luaL_error(L, "Bad number of args"); }
        auto& reg = *get_pointer<spkt::registry>(L, "__registry__");
        auto entity = Converter<spkt::entity>::read(L, 1);
        glm::vec3 p = Converter<glm::vec3>::read(L, 2);
        glm::vec3 t = Converter<glm::vec3>::read(L, 3);
        auto& tr = reg.get<Transform3DComponent>(entity);
        tr.position = p;
        tr.orientation = glm::conjugate(glm::quat_cast(glm::lookAt(tr.position, t, {0.0, 1.0, 0.0})));
        return 0;
    });

    lua_register(L, "RotateY", [](lua_State* L) {
        if (!check_arg_count(L, 2)) { return luaL_error(L, "Bad number of args"); };
        auto& reg = *get_pointer<spkt::registry>(L, "__registry__");
        auto entity = Converter<spkt::entity>::read(L, 1);
        auto& tr = reg.get<Transform3DComponent>(entity);
        float yaw = (float)lua_tonumber(L, 2);
        tr.orientation = glm::rotate(tr.orientation, yaw, {0, 1, 0});
        return 0;
    });

    lua_register(L, "GetForwardsDir", [](lua_State* L) {
        if (!check_arg_count(L, 1)) { return luaL_error(L, "Bad number of args"); }
        auto& reg = *get_pointer<spkt::registry>(L, "__registry__");
        auto entity = Converter<spkt::entity>::read(L, 1);
        auto& tr = reg.get<Transform3DComponent>(entity);
        auto o = tr.orientation;
        
        if (auto* pc = reg.get_if<Camera3DComponent>(entity)) {
            o = glm::rotate(o, pc->pitch, {1, 0, 0});
        }

        Converter<glm::vec3>::push(L, Maths::Forwards(o));
        return 1;
    });

    lua_register(L, "GetRightDir", [](lua_State* L) {
        if (!check_arg_count(L, 1)) { return luaL_error(L, "Bad number of args"); }
        auto& reg = *get_pointer<spkt::registry>(L, "__registry__");
        auto entity = Converter<spkt::entity>::read(L, 1);
        auto& tr = reg.get<Transform3DComponent>(entity);
        Converter<glm::vec3>::push(L, Maths::Right(tr.orientation));
        return 1;
    });

    lua_register(L, "MakeUpright", [](lua_State* L) {
        if (!check_arg_count(L, 2)) { return luaL_error(L, "Bad number of args"); }
        auto& reg = *get_pointer<spkt::registry>(L, "__registry__");
        auto entity = Converter<spkt::entity>::read(L, 1);
        auto& tr = reg.get<Transform3DComponent>(entity);
        float yaw = Converter<float>::read(L, 2);
        tr.orientation = glm::quat(glm::vec3(0, yaw, 0));
        return 0;
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
        auto& reg = *get_pointer<spkt::registry>(L, "__registry__");
        Converter<spkt::entity>::push(L, reg.create());
        return 1;
    });

    lua_register(L, "DeleteEntity", [](lua_State* L) {
        if (!check_arg_count(L, 1)) { return luaL_error(L, "Bad number of args"); }
        auto& reg = *get_pointer<spkt::registry>(L, "__registry__");
        auto entity = Converter<spkt::entity>::read(L, 1);
        add_command(L, [&, entity]() { reg.destroy(entity); });
        return 0;
    });

    // Add functions for iterating over all entities in __scene__. The C++ functions
    // should not be used directly, instead they should be used via the Scene:Each
    // function implemented last in Lua.    
    lua_register(L, "_view_All_init", view_init<>);
    lua_register(L, "_view_All_next", view_next<>);
    luaL_dostring(L, view_source_code("All").c_str());

    // VIEW FUNCTIONS - GENERATED BY DATAMATIC
DATAMATIC_BEGIN SCRIPTABLE=true
    lua_register(L, "_view_{{Comp::name}}_init", view_init<{{Comp::name}}>);
    lua_register(L, "_view_{{Comp::name}}_next", view_next<{{Comp::name}}>);
    luaL_dostring(L, view_source_code("{{Comp::name}}").c_str());

DATAMATIC_END
}

// COMPONENT RELATED CODE - GENERATED BY DATAMATIC
DATAMATIC_BEGIN SCRIPTABLE=true
// C++ Functions for {{Comp::name}} =====================================================

int _Get{{Comp::name}}(lua_State* L) {
    if (!check_arg_count(L, 1)) { return luaL_error(L, "Bad number of args"); }
    auto& reg = *get_pointer<spkt::registry>(L, "__registry__");
    auto e = Converter<spkt::entity>::read(L, 1);
    const auto& c = reg.get<{{Comp::name}}>(e);
    Converter<{{Attr::type}}>::push(L, c.{{Attr::name}});
    return {{Comp::attr_count}};
}

int _Set{{Comp::name}}(lua_State* L) {
    if (!check_arg_count(L, {{Comp::attr_count}} + 1)) { return luaL_error(L, "Bad number of args"); }
    int ptr = 0;
    auto& reg = *get_pointer<spkt::registry>(L, "__registry__");
    auto e = Converter<spkt::entity>::read(L, ++ptr);
    auto& c = reg.get<{{Comp::name}}>(e);
    c.{{Attr::name}} = Converter<{{Attr::type}}>::read(L, ++ptr);
    return 0;
}

int _Add{{Comp::name}}(lua_State* L) {
    if (!check_arg_count(L, {{Comp::attr_count}} + 1)) { return luaL_error(L, "Bad number of args"); }
    int ptr = 0;
    auto& reg = *get_pointer<spkt::registry>(L, "__registry__");
    auto e = Converter<spkt::entity>::read(L, ++ptr);
    assert(!reg.has<{{Comp::name}}>(e));
    {{Comp::name}} c;
    c.{{Attr::name}} = Converter<{{Attr::type}}>::read(L, ++ptr);
    add_command(L, [&, e, c]() { reg.add<{{Comp::name}}>(e, c); });
    return 0;
}

DATAMATIC_END

void load_entity_component_functions(lua::Script& script)
{
    lua_State* L = script.native_handle();

DATAMATIC_BEGIN SCRIPTABLE=true
    // Lua functions for {{Comp::name}} =====================================================

    luaL_dostring(L, R"lua(
        {{Comp::name}} = Class(function(self, {{Comp::attr_list("name", ", ")}})
            self.{{Attr::name}} = {{Attr::name}}
        end)
    )lua");

    lua_register(L, "_Get{{Comp::name}}", &_Get{{Comp::name}});

    luaL_dostring(L, R"lua(
        function Get{{Comp::name}}(entity)
            {{Comp::attr_list("name", ", ")}} = _Get{{Comp::name}}(entity)
            return {{Comp::name}}({{Comp::attr_list("name", ", ")}})
        end
    )lua");

    lua_register(L, "_Set{{Comp::name}}", &_Set{{Comp::name}});

    luaL_dostring(L, R"lua(
        function Set{{Comp::name}}(entity, c)
            _Set{{Comp::name}}(entity, {{Comp::attr_list("name", ", ", "c.{}")}})
        end
    )lua");

    lua_register(L, "_Add{{Comp::name}}", &_Add{{Comp::name}});

    luaL_dostring(L, R"lua(
        function Add{{Comp::name}}(entity, c)
            _Add{{Comp::name}}(entity, {{Comp::attr_list("name", ", ", "c.{}")}})
        end
    )lua");

    lua_register(L, "Has{{Comp::name}}", &has_impl<{{Comp::name}}>);
    lua_register(L, "Remove{{Comp::name}}", &remove_impl<{{Comp::name}}>);


DATAMATIC_END
}

}
}