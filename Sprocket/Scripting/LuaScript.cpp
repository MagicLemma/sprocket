#include "LuaScript.h"
#include "Log.h"
#include "ECS.h"
#include "Components.h"

#include <lua.hpp>

namespace Sprocket {
namespace lua {
namespace {

void do_file(lua_State* L, const char* file)
{
    if (luaL_dofile(L, file)) {
        log::error("[Lua]: Could not load {}", lua_tostring(L, -1));
    }
}

}

Script::Script(const std::string& file)
    : d_L(luaL_newstate(), [](lua_State* L) { lua_close(L); })
{
    lua_State* L = d_L.get();
    luaL_openlibs(L);
    do_file(L, "Sprocket/Scripting/Sprocket_Base.lua");
    do_file(L, file.c_str());
}

Script::Script()
    : d_L(luaL_newstate(), [](lua_State* L) { lua_close(L); })
{
    lua_State* L = d_L.get();
    luaL_openlibs(L);
    do_file(L, "Sprocket/Scripting/Sprocket_Base.lua");
}

void* Script::allocate(std::size_t size)
{
    return lua_newuserdata(d_L.get(), size);
}

void Script::push_value(bool val)
{
    lua_pushboolean(d_L.get(), val);
}

void Script::push_value(char val)
{
    lua_pushstring(d_L.get(), std::string(1, val).c_str());
}

void Script::push_value(int val)
{
    lua_pushinteger(d_L.get(), val);
}

void Script::push_value(float val)
{
    lua_pushnumber(d_L.get(), val);
}

void Script::push_value(double val)
{
    lua_pushnumber(d_L.get(), val);
}

void Script::push_value(const char* val)
{
    lua_pushstring(d_L.get(), val);
}

void Script::push_value(const std::string& val)
{
    lua_pushstring(d_L.get(), val.c_str());
}

void Script::push_value(void* val)
{
    lua_pushlightuserdata(d_L.get(), val);
}

void Script::print_errors(int rc) const
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

bool Script::has_function(const std::string& function)
{
    lua_getglobal(d_L.get(), function.c_str());
    if (!lua_isfunction(d_L.get(), -1)) {
        lua_pop(d_L.get(), -1);
        return false;
    }
    return true;
}

void Script::print_globals()
{
    log::info("Starting globals");
    lua_pushglobaltable(d_L.get());
    lua_pushnil(d_L.get());
    while (lua_next(d_L.get(), -2) != 0) {
        if (lua_isnumber(d_L.get(), -1)) {
            log::info("{} = {}", lua_tostring(d_L.get(), -2), lua_tonumber(d_L.get(), -1));
        }
        lua_pop(d_L.get(), 1);
    }
    lua_pop(d_L.get(), 1);
    log::info("Ending globals");
}

}
}