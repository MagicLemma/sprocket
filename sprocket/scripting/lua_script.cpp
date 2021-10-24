#include "lua_script.h"

#include <sprocket/Core/log.h>

#include <lua.hpp>

#include <string>

namespace spkt {
namespace lua {
namespace {

void do_file(lua_State* L, const char* file)
{
    if (luaL_dofile(L, file)) {
        log::error("[Lua]: Could not load {}", lua_tostring(L, -1));
    }
}

}

script::script(const std::string& file)
    : d_L(luaL_newstate(), [](lua_State* L) { lua_close(L); })
{
    lua_State* L = d_L.get();
    luaL_openlibs(L);
    do_file(L, "Sprocket/Scripting/Sprocket_Base.lua");
    do_file(L, file.c_str());
}

script::script()
    : d_L(luaL_newstate(), [](lua_State* L) { lua_close(L); })
{
    lua_State* L = d_L.get();
    luaL_openlibs(L);
    do_file(L, "Sprocket/Scripting/Sprocket_Base.lua");
}

void script::print_errors(int rc) const
{
    if (rc == LUA_OK) { return; } // No error
    
    std::string err = lua_tostring(d_L.get(), -1);
    switch (rc) {
        case LUA_ERRRUN:
            log::error("[Lua] Runtime error: {}", err);
            return;
        case LUA_ERRMEM:
            log::error("[Lua] Memory allocation error: {}", err);
            return;
        case LUA_ERRERR:
            log::error("[Lua] Error handler func failed: {}", err);
            return;
        default:
            log::error("[Lua] Unknown error: {}", err);
    }
}

bool script::has_function(const std::string& function)
{
    lua_State* L = d_L.get();
    lua_getglobal(L, function.c_str());
    bool is_function = lua_isfunction(L, -1);
    lua_pop(L, 1);
    return is_function;
}

void script::print_globals()
{
    lua_State* L = d_L.get();

    log::info("Starting globals");
    lua_pushglobaltable(L);
    lua_pushnil(L);
    while (lua_next(L, -2) != 0) {
        if (lua_isnumber(L, -1)) {
            log::info("{} = {}", lua_tostring(L, -2), lua_tonumber(L, -1));
        }
        lua_pop(L, 1);
    }
    lua_pop(d_L.get(), 1);
    log::info("Ending globals");
}

}
}