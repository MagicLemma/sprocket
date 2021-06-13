#include "LuaConverter.h"

#include <lua.hpp>

#include <cassert>

namespace Sprocket {
namespace lua {

int Converter<int>::pop(lua_State* L)
{
    int ptr = -1;
    auto ret = read(L, ptr);
    lua_pop(L, 1);
    return ret;
}

int Converter<int>::push(lua_State* L, const int& value)
{
    lua_pushinteger(L, value);
    return 1;
}

int Converter<int>::read(lua_State* L, int index)
{
    assert(lua_isinteger(L, index));
    return static_cast<int>(lua_tointeger(L, index));
}


u32 Converter<u32>::pop(lua_State* L)
{
    int ptr = -1;
    auto ret = read(L, ptr);
    lua_pop(L, 1);
    return ret;
}

int Converter<u32>::push(lua_State* L, const u32& value)
{
    lua_pushinteger(L, value);
    return 1;
}

u32 Converter<u32>::read(lua_State* L, int index)
{
    assert(lua_isinteger(L, index));
    return static_cast<u32>(lua_tointeger(L, index));
}


bool Converter<bool>::pop(lua_State* L)
{
    int ptr = -1;
    auto ret = read(L, ptr);
    lua_pop(L, 1);
    return ret;
}

int Converter<bool>::push(lua_State* L, const bool& value)
{
    lua_pushboolean(L, value);
    return 1;
}

bool Converter<bool>::read(lua_State* L, int index)
{
    assert(lua_isboolean(L, index));
    return static_cast<bool>(lua_toboolean(L, index));
}


float Converter<float>::pop(lua_State* L)
{
    int ptr = -1;
    auto ret = read(L, ptr);
    lua_pop(L, 1);
    return ret;
}

int Converter<float>::push(lua_State* L, const float& value)
{
    lua_pushnumber(L, value);
    return 1;
}

float Converter<float>::read(lua_State* L, int index)
{
    assert(lua_isnumber(L, index));
    return static_cast<float>(lua_tonumber(L, index));
}


double Converter<double>::pop(lua_State* L)
{
    int ptr = -1;
    auto ret = read(L, ptr);
    lua_pop(L, 1);
    return ret;
}

int Converter<double>::push(lua_State* L, const double& value)
{
    lua_pushnumber(L, value);
    return 1;
}

double Converter<double>::read(lua_State* L, int index)
{
    assert(lua_isnumber(L, index));
    return static_cast<double>(lua_tonumber(L, index));
}


const char* Converter<const char*>::pop(lua_State* L)
{
    int ptr = -1;
    auto ret = read(L, ptr);
    lua_pop(L, 1);
    return ret;
}

int Converter<const char*>::push(lua_State* L, const char* value)
{
    lua_pushstring(L, value);
    return 1;
}

const char* Converter<const char*>::read(lua_State* L, int index)
{
    assert(lua_isstring(L, index));
    return lua_tostring(L, index);
}


std::string Converter<std::string>::pop(lua_State* L)
{
    int ptr = -1;
    auto ret = read(L, ptr);
    lua_pop(L, 1);
    return ret;
}

int Converter<std::string>::push(lua_State* L, const std::string& value)
{
    lua_pushstring(L, value.c_str());
    return 1;
}

std::string Converter<std::string>::read(lua_State* L, int index)
{
    assert(lua_isstring(L, index));
    return lua_tostring(L, index);
}


void* Converter<void*>::pop(lua_State* L)
{
    int ptr = -1;
    auto ret = read(L, ptr);
    lua_pop(L, 1);
    return ret;
}

int Converter<void*>::push(lua_State* L, void* value)
{
    lua_pushlightuserdata(L, value);
    return 1;
}

void* Converter<void*>::read(lua_State* L, int index)
{
    assert(lua_islightuserdata(L, index));
    return lua_touserdata(L, index);
}


spkt::entity Converter<spkt::entity>::pop(lua_State* L)
{
    int ptr = -1;
    auto ret = read(L, ptr);
    lua_pop(L, 1);
    return ret;
}

int Converter<spkt::entity>::push(lua_State* L, const spkt::entity& value)
{
    spkt::entity* handle = static_cast<spkt::entity*>(lua_newuserdata(L, sizeof(spkt::entity)));
    *handle = value;
    return 1;
}

spkt::entity Converter<spkt::entity>::read(lua_State* L, int index)
{
    assert(lua_isuserdata(L, index));
    return *static_cast<spkt::entity*>(lua_touserdata(L, index));
}


spkt::identifier Converter<spkt::identifier>::pop(lua_State* L)
{
    int ptr = -1;
    auto ret = read(L, ptr);
    lua_pop(L, 1);
    return ret;
}

int Converter<spkt::identifier>::push(lua_State* L, const spkt::identifier& value)
{
    spkt::identifier* handle = static_cast<spkt::identifier*>(lua_newuserdata(L, sizeof(spkt::identifier)));
    *handle = value;
    return 1;
}

spkt::identifier Converter<spkt::identifier>::read(lua_State* L, int index)
{
    assert(lua_isuserdata(L, index));
    return *static_cast<spkt::identifier*>(lua_touserdata(L, index));
}


glm::vec2 Converter<glm::vec2>::pop(lua_State* L)
{
    int ptr = -1;
    auto ret = read(L, ptr);
    lua_pop(L, 1);
    return ret;
}

int Converter<glm::vec2>::push(lua_State* L, const glm::vec2& value)
{
    glm::vec2* vec = (glm::vec2*)lua_newuserdata(L, sizeof(glm::vec2));
    *vec = value;
    luaL_setmetatable(L, "vec2");
    return 1;
}

glm::vec2 Converter<glm::vec2>::read(lua_State* L, int index)
{
    return *(glm::vec2*)luaL_checkudata(L, index, "vec2");
}


glm::vec3 Converter<glm::vec3>::pop(lua_State* L)
{
    int ptr = -1;
    auto ret = read(L, ptr);
    lua_pop(L, 1);
    return ret;
}

int Converter<glm::vec3>::push(lua_State* L, const glm::vec3& value)
{
    glm::vec3* vec = (glm::vec3*)lua_newuserdata(L, sizeof(glm::vec3));
    *vec = value;
    luaL_setmetatable(L, "vec3");
    return 1;
}

glm::vec3 Converter<glm::vec3>::read(lua_State* L, int index)
{
    return *(glm::vec3*)luaL_checkudata(L, index, "vec3");
}


}
}