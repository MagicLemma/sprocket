#include "LuaLibrary.h"
#include "LuaScript.h"
#include "LuaConverter.h"
#include "ecs.h"
#include "Scene.h"
#include "Window.h"
#include "ecs.h
#include "Log.h"

#include "ecs.h"

#include <vector>
#include <utility>
#include <format>

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
    auto entity = Converter<spkt::handle>::read(L, 1);
    Converter<bool>::push(L, entity.has<T>());
    return 1;
}

void add_command(lua_State* L, const std::function<void()>& command)
{
    using command_list_t = std::vector<std::function<void()>>;
    command_list_t& command_list = *get_pointer<command_list_t>(L, "__command_list__");
    command_list.push_back(command);
}

std::string view_function_source(std::string_view name, std::string_view suffix)
{
    return std::format(R"lua(
        function {0}{1}()
            local generator = _Each_{0}_New()
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
    )lua", name, suffix);
}

}

void load_entity_transformation_functions(lua::Script& script)
{
    lua_State* L = script.native_handle();

    lua_register(L, "SetLookAt", [](lua_State* L) {
        if (!CheckArgCount(L, 3)) { return luaL_error(L, "Bad number of args"); }
        spkt::handle entity = Converter<spkt::handle>::read(L, 1);
        glm::vec3 p = Converter<glm::vec3>::read(L, 2);
        glm::vec3 t = Converter<glm::vec3>::read(L, 3);
        auto& tr = entity.get<Transform3DComponent>();
        tr.position = p;
        tr.orientation = glm::conjugate(glm::quat_cast(glm::lookAt(tr.position, t, {0.0, 1.0, 0.0})));
        return 0;
    });

    lua_register(L, "RotateY", [](lua_State* L) {
        if (!CheckArgCount(L, 2)) { return luaL_error(L, "Bad number of args"); };
        spkt::handle entity = *static_cast<spkt::handle*>(lua_touserdata(L, 1));
        auto& tr = entity.get<Transform3DComponent>();
        float yaw = (float)lua_tonumber(L, 2);
        tr.orientation = glm::rotate(tr.orientation, yaw, {0, 1, 0});
        return 0;
    });

    lua_register(L, "GetForwardsDir", [](lua_State* L) {
        if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }
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
        if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }
        spkt::handle entity = Converter<spkt::handle>::read(L, 1);
        auto& tr = entity.get<Transform3DComponent>();
        Converter<glm::vec3>::push(L, Maths::Right(tr.orientation));
        return 1;
    });

    lua_register(L, "MakeUpright", [](lua_State* L) {
        if (!CheckArgCount(L, 2)) { return luaL_error(L, "Bad number of args"); }
        spkt::handle entity = Converter<spkt::handle>::read(L, 1);
        auto& tr = entity.get<Transform3DComponent>();
        float yaw = Converter<float>::read(L, 2);
        tr.orientation = glm::quat(glm::vec3(0, yaw, 0));
        return 0;
    });

    lua_register(L, "AreEntitiesEqual", [](lua_State* L) {
        if (!CheckArgCount(L, 2)) { return luaL_error(L, "Bad number of args"); }
        spkt::handle entity1 = Converter<spkt::handle>::read(L, 1);
        spkt::handle entity2 = Converter<spkt::handle>::read(L, 2);
        Converter<bool>::push(L, entity1 == entity2);
        return 1;
    });
}

void load_registry_functions(lua::Script& script, spkt::registry& registry)
{
    lua_State* L = script.native_handle();
    script.set_value("__registry__", &registry);

    // Add functions for creating and destroying entities.
    lua_register(L, "NewEntity", [](lua_State* L) {
        if (!CheckArgCount(L, 0)) { return luaL_error(L, "Bad number of args"); }
        spkt::registry& registry = *get_pointer<spkt::registry>(L, "__registry__");
        auto new_entity = spkt::handle(registry, registry.create());
        Converter<spkt::handle>::push(L, new_entity);
        return 1;
    });

    lua_register(L, "DeleteEntity", [](lua_State* L) {
        if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }
        spkt::handle entity = *static_cast<spkt::handle*>(lua_touserdata(L, 1));
        add_command(L, [entity]() mutable { entity.destroy(); });
        return 0;
    });

    lua_register(L, "entity_from_id", [](lua_State* L) {
        if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }
        spkt::registry& registry = *get_pointer<spkt::registry>(L, "__registry__");
        auto id = Converter<spkt::entity>::read(L, 1);
        Converter<spkt::handle>::push(L, {registry, id});
        return 1;
    });

    lua_register(L, "entity_singleton", [](lua_State* L) {
        if (!CheckArgCount(L, 0)) { return luaL_error(L, "Bad number of args"); }
        spkt::registry& registry = *get_pointer<spkt::registry>(L, "__registry__");
        auto singleton = registry.find<Singleton>();
        Converter<spkt::handle>::push(L, {registry, singleton});
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
    using Generator = apx::generator<spkt::entity>;
    using Iterator = typename Generator::iterator;
    static_assert(std::is_trivially_destructible_v<Iterator>);
    
    lua_register(L, "_Each_All_New", [](lua_State* L) {
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

        spkt::handle* luaEntity = static_cast<spkt::handle*>(lua_newuserdata(L, sizeof(spkt::handle)));
        *luaEntity = spkt::handle(registry, *iterator);
        return 1;
    });

    // Hook all of the above functions into a single generator function.
    luaL_dostring(L, view_function_source("All", "").c_str());

// VIEW FUNCTIONS - GENERATED BY DATAMATIC
DATAMATIC_BEGIN SCRIPTABLE=true
    lua_register(L, "_Each_{{Comp::name}}_New", [](lua_State* L) {
        if (!CheckArgCount(L, 0)) { return luaL_error(L, "Bad number of args"); }
        auto gen = new Generator(get_pointer<spkt::registry>(L, "__registry__")->view<{{Comp::name}}>());
        lua_pushlightuserdata(L, static_cast<void*>(gen));
        return 1;
    });

    luaL_dostring(L, view_function_source("{{Comp::name}}", "View").c_str());

DATAMATIC_END
}

// COMPONENT RELATED CODE - GENERATED BY DATAMATIC
DATAMATIC_BEGIN SCRIPTABLE=true
// C++ Functions for {{Comp::name}} =====================================================

int _Get{{Comp::name}}(lua_State* L) {
    if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }
    spkt::handle e = Converter<spkt::handle>::read(L, 1);
    assert(e.has<{{Comp::name}}>());
    const auto& c = e.get<{{Comp::name}}>();
    Converter<{{Attr::type}}>::push(L, c.{{Attr::name}});
    return {{Comp::attr_count}};
}

int _Set{{Comp::name}}(lua_State* L) {
    if (!CheckArgCount(L, {{Comp::attr_count}} + 1)) { return luaL_error(L, "Bad number of args"); }
    int ptr = 0;
    spkt::handle e = Converter<spkt::handle>::read(L, ++ptr);
    auto& c = e.get<{{Comp::name}}>();
    c.{{Attr::name}} = Converter<{{Attr::type}}>::read(L, ++ptr);
    return 0;
}

int _Add{{Comp::name}}(lua_State* L) {
    if (!CheckArgCount(L, {{Comp::attr_count}} + 1)) { return luaL_error(L, "Bad number of args"); }
    int ptr = 0;
    spkt::handle e = Converter<spkt::handle>::read(L, ++ptr);
    assert(!e.has<{{Comp::name}}>());
    {{Comp::name}} c;
    c.{{Attr::name}} = Converter<{{Attr::type}}>::read(L, ++ptr);
    add_command(L, [e, c]() mutable { e.add<{{Comp::name}}>(c); });
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

    lua_register(L, "Has{{Comp::name}}", &_has_impl<{{Comp::name}}>);


DATAMATIC_END
}

}
}