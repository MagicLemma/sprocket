#include "LuaConverter.h"

#include <lua.hpp>

#include <cassert>

namespace Sprocket {
namespace lua {

int Converter<int>::pull_from(lua_State* L)
{
    assert(lua_isinteger(L, -1));
    auto val = static_cast<int>(lua_tointeger(L, -1));
    lua_pop(L, 1);
    return val;
}

void Converter<int>::push_to(lua_State* L, const int& value)
{
    lua_pushinteger(L, value);
}


u32 Converter<u32>::pull_from(lua_State* L)
{
    assert(lua_isinteger(L, -1));
    auto val = static_cast<u32>(lua_tointeger(L, -1));
    lua_pop(L, 1);
    return val;
}

void Converter<u32>::push_to(lua_State* L, const u32& value)
{
    lua_pushinteger(L, value);
}



bool Converter<bool>::pull_from(lua_State* L)
{
    assert(lua_isboolean(L, -1));
    auto val = static_cast<bool>(lua_toboolean(L, -1));
    lua_pop(L, 1);
    return val;
}

void Converter<bool>::push_to(lua_State* L, const bool& value)
{
    lua_pushboolean(L, value);
}


double Converter<double>::pull_from(lua_State* L)
{
    assert(lua_isnumber(L, -1));
    auto val = static_cast<double>(lua_tonumber(L, -1));
    lua_pop(L, 1);
    return val;
}

void Converter<double>::push_to(lua_State* L, const double& value)
{
    lua_pushnumber(L, value);
}


float Converter<float>::pull_from(lua_State* L)
{
    assert(lua_isnumber(L, -1));
    auto val = static_cast<float>(lua_tonumber(L, -1));
    lua_pop(L, 1);
    return val;
}

void Converter<float>::push_to(lua_State* L, const float& value)
{
    lua_pushnumber(L, value);
}


const char* Converter<const char*>::pull_from(lua_State* L)
{
    assert(lua_isstring(L, -1));
    auto val = lua_tostring(L, -1);
    lua_pop(L, 1);
    return val;
}

void Converter<const char*>::push_to(lua_State* L, const char* value)
{
    lua_pushstring(L, value);
}


std::string Converter<std::string>::pull_from(lua_State* L)
{
    assert(lua_isstring(L, -1));
    auto val = std::string(lua_tostring(L, -1));
    lua_pop(L, 1);
    return val;
}

void Converter<std::string>::push_to(lua_State* L, const std::string& value)
{
    lua_pushstring(L, value.c_str());
}


void* Converter<void*>::pull_from(lua_State* L)
{
    assert(lua_islightuserdata(L, -1) || lua_isuserdata(L, -1));
    auto val = lua_touserdata(L, -1);
    lua_pop(L, 1);
    return val;
}

void Converter<void*>::push_to(lua_State* L, void* value)
{
    lua_pushlightuserdata(L, value);
}


ecs::Entity Converter<ecs::Entity>::pull_from(lua_State* L)
{
    assert(lua_isuserdata(L, -1));
    auto val = *static_cast<ecs::Entity*>(lua_touserdata(L, -1));
    lua_pop(L, 1);
    return val;
}

void Converter<ecs::Entity>::push_to(lua_State* L, ecs::Entity value)
{
    ecs::Entity* handle = static_cast<ecs::Entity*>(lua_newuserdata(L, sizeof(ecs::Entity)));
    *handle = value;
}


}
}