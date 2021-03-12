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
}

// COMPONENT RELATED CODE - GENERATED BY DATAMATIC
void load_entity_component_functions(lua::Script& script)
{
    lua_State* L = script.native_handle();

#ifdef DATAMATIC_BLOCK SCRIPTABLE=true
    // Functions for {{Comp.Name}} =====================================================

    constexpr int {{Comp.Name}}_dimension = {{Comp.Lua.dimension}};

    luaL_dostring(L, R"lua(
        {{Comp.Name}} = Class(function(self, {{Comp.Lua.Sig}})
            self.{{Attr.Name}} = {{Attr.Name}}
        end)
    )lua");

    lua_register(L, "_Get{{Comp.Name}}", [](lua_State* L) {
        if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }

        int ptr = 1;
        ecs::Entity e = Converter<ecs::Entity>::read(L, ptr);
        assert(e.Has<{{Comp.Name}}>());

        int count = 0;
        const auto& c = e.Get<{{Comp.Name}}>();
        count += Converter<{{Attr.Type}}>::push(L, c.{{Attr.Name}});
        assert(count == {{Comp.Name}}_dimension);
        return count;
    });

    luaL_dostring(L, R"lua(
        {{Comp.Lua.Getter}}
    )lua");

    lua_register(L, "_Set{{Comp.Name}}", [](lua_State* L) {
        if (!CheckArgCount(L, {{Comp.Name}}_dimension + 1)) { return luaL_error(L, "Bad number of args"); }

        int ptr = 1;
        ecs::Entity e = Converter<ecs::Entity>::read(L, ptr);
        auto& c = e.Get<{{Comp.Name}}>();
        c.{{Attr.Name}} = Converter<{{Attr.Type}}>::read(L, ptr);
        assert(ptr == {{Comp.Name}}_dimension + 2);
        return 0;
    });

    luaL_dostring(L, R"lua(
        {{Comp.Lua.Setter}}
    )lua");

    lua_register(L, "_Add{{Comp.Name}}", [](lua_State* L) {
        if (!CheckArgCount(L, {{Comp.Name}}_dimension + 1)) { return luaL_error(L, "Bad number of args"); }

        int ptr = 1;
        ecs::Entity e = Converter<ecs::Entity>::read(L, ptr);
        assert(!e.Has<{{Comp.Name}}>());

        {{Comp.Name}} c;
        c.{{Attr.Name}} = Converter<{{Attr.Type}}>::read(L, ptr);
        e.Add<{{Comp.Name}}>(c);
        assert(ptr == {{Comp.Name}}_dimension + 2);
        return 0;
    });

    luaL_dostring(L, R"lua(
        {{Comp.Lua.Adder}}
    )lua");

    lua_register(L, "Has{{Comp.Name}}", &_has_impl<{{Comp.Name}}>);


#endif
}

}
}