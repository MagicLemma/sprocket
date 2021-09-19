#include "lua_input.h"

#include <Sprocket/Core/log.h>
#include <Sprocket/Utility/input_store.h>
#include <Sprocket/Scripting/lua_script.h>
#include <Sprocket/Scripting/lua_converter.h>

#include <lua.h>

namespace spkt {
namespace lua {
namespace {

constexpr const char* INPUT_VAR_NAME = "__input__";

const input_store& get_input(lua_State* L)
{
    lua_getglobal(L, INPUT_VAR_NAME);
    const auto& input = *static_cast<input_store*>(lua_touserdata(L, -1));
    lua_pop(L, 1);
    return input;
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

}

void load_input_store(lua::script& script, input_store& input)
{
    lua_State* L = script.native_handle();
    script.set_value(INPUT_VAR_NAME, &input);

    lua_register(L, "IsKeyDown", [](lua_State* L) {
        if (!check_arg_count(L, 1)) { return luaL_error(L, "Bad number of args"); }
        const auto& input = get_input(L);
        Converter<bool>::push(L, input.is_keyboard_down((int)lua_tointeger(L, 1)));
        return 1;
    });

    lua_register(L, "IsKeyClicked", [](lua_State* L) {
        if (!check_arg_count(L, 1)) { return luaL_error(L, "Bad number of args"); }
        const auto& input = get_input(L);
        Converter<bool>::push(L, input.is_keyboard_clicked((int)lua_tointeger(L, 1)));
        return 1;
    });

    lua_register(L, "IsKeyUnclicked", [](lua_State* L) {
        if (!check_arg_count(L, 1)) { return luaL_error(L, "Bad number of args"); }
        const auto& input = get_input(L);
        Converter<bool>::push(L, input.is_keyboard_unclicked((int)lua_tointeger(L, 1)));
        return 1;
    });


    lua_register(L, "IsMouseDown", [](lua_State* L) {
        if (!check_arg_count(L, 1)) { return luaL_error(L, "Bad number of args"); }
        const auto& input = get_input(L);
        Converter<bool>::push(L, input.is_mouse_down((int)lua_tointeger(L, 1)));
        return 1;
    });

    lua_register(L, "IsMouseClicked", [](lua_State* L) {
        if (!check_arg_count(L, 1)) { return luaL_error(L, "Bad number of args"); }
        const auto& input = get_input(L);
        Converter<bool>::push(L, input.is_mouse_clicked((int)lua_tointeger(L, 1)));
        return 1;
    });

    lua_register(L, "IsMouseUnclicked", [](lua_State* L) {
        if (!check_arg_count(L, 1)) { return luaL_error(L, "Bad number of args"); }
        const auto& input = get_input(L);
        Converter<bool>::push(L, input.is_mouse_unclicked((int)lua_tointeger(L, 1)));
        return 1;
    });
    

    lua_register(L, "GetMousePos", [](lua_State* L) {
        if (!check_arg_count(L, 0)) { return luaL_error(L, "Bad number of args"); }
        const auto& input = get_input(L);
        Converter<glm::vec2>::push(L, input.mouse_position());
        return 1;
    });

    lua_register(L, "GetMouseOffset", [](lua_State* L) {
        if (!check_arg_count(L, 0)) { return luaL_error(L, "Bad number of args"); }
        const auto& input = get_input(L);
        Converter<glm::vec2>::push(L, input.mouse_offset());
        return 1;
    });

    lua_register(L, "GetMouseScrolled", [](lua_State* L) {
        if (!check_arg_count(L, 0)) { return luaL_error(L, "Bad number of args"); }
        const auto& input = get_input(L);
        Converter<glm::vec2>::push(L, input.mouse_scrolled());
        return 1;
    });
}

}
}