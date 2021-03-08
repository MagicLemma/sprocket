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

int Converter<int>::push_to(lua_State* L, const int& value)
{
    lua_pushinteger(L, value);
    return 1;
}


u32 Converter<u32>::pull_from(lua_State* L)
{
    assert(lua_isinteger(L, -1));
    auto val = static_cast<u32>(lua_tointeger(L, -1));
    lua_pop(L, 1);
    return val;
}

int Converter<u32>::push_to(lua_State* L, const u32& value)
{
    lua_pushinteger(L, value);
    return 1;
}


bool Converter<bool>::pull_from(lua_State* L)
{
    assert(lua_isboolean(L, -1));
    auto val = static_cast<bool>(lua_toboolean(L, -1));
    lua_pop(L, 1);
    return val;
}

int Converter<bool>::push_to(lua_State* L, const bool& value)
{
    lua_pushboolean(L, value);
    return 1;
}


double Converter<double>::pull_from(lua_State* L)
{
    assert(lua_isnumber(L, -1));
    auto val = static_cast<double>(lua_tonumber(L, -1));
    lua_pop(L, 1);
    return val;
}

int Converter<double>::push_to(lua_State* L, const double& value)
{
    lua_pushnumber(L, value);
    return 1;
}


float Converter<float>::pull_from(lua_State* L)
{
    assert(lua_isnumber(L, -1));
    auto val = static_cast<float>(lua_tonumber(L, -1));
    lua_pop(L, 1);
    return val;
}

int Converter<float>::push_to(lua_State* L, const float& value)
{
    lua_pushnumber(L, value);
    return 1;
}


const char* Converter<const char*>::pull_from(lua_State* L)
{
    assert(lua_isstring(L, -1));
    auto val = lua_tostring(L, -1);
    lua_pop(L, 1);
    return val;
}

int Converter<const char*>::push_to(lua_State* L, const char* value)
{
    lua_pushstring(L, value);
    return 1;
}


std::string Converter<std::string>::pull_from(lua_State* L)
{
    assert(lua_isstring(L, -1));
    auto val = std::string(lua_tostring(L, -1));
    lua_pop(L, 1);
    return val;
}

int Converter<std::string>::push_to(lua_State* L, const std::string& value)
{
    lua_pushstring(L, value.c_str());
    return 1;
}


void* Converter<void*>::pull_from(lua_State* L)
{
    assert(lua_islightuserdata(L, -1) || lua_isuserdata(L, -1));
    auto val = lua_touserdata(L, -1);
    lua_pop(L, 1);
    return val;
}

int Converter<void*>::push_to(lua_State* L, void* value)
{
    lua_pushlightuserdata(L, value);
    return 1;
}


ecs::Entity Converter<ecs::Entity>::pull_from(lua_State* L)
{
    assert(lua_isuserdata(L, -1));
    auto val = *static_cast<ecs::Entity*>(lua_touserdata(L, -1));
    lua_pop(L, 1);
    return val;
}

int Converter<ecs::Entity>::push_to(lua_State* L, ecs::Entity value)
{
    ecs::Entity* handle = static_cast<ecs::Entity*>(lua_newuserdata(L, sizeof(ecs::Entity)));
    *handle = value;
    return 1;
}


int Converter<glm::vec2>::push_to(lua_State* L, const glm::vec2& value)
{
    Converter<float>::push_to(L, value.x);
    Converter<float>::push_to(L, value.y);
    return 2;
}


int Converter<glm::vec3>::push_to(lua_State* L, const glm::vec3& value)
{
    Converter<float>::push_to(L, value.x);
    Converter<float>::push_to(L, value.y);
    Converter<float>::push_to(L, value.z);
    return 3;
}


}
}