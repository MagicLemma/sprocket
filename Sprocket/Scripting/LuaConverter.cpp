#include "LuaConverter.h"

#include <lua.hpp>

#include <cassert>

namespace Sprocket {
namespace lua {

int Converter<int>::pop(lua_State* L)
{
    int ptr = -dimension;
    auto ret = read(L, ptr);
    lua_pop(L, dimension);
    return ret;
}

int Converter<int>::push(lua_State* L, const int& value)
{
    lua_pushinteger(L, value);
    return dimension;
}

int Converter<int>::read(lua_State* L, int& read_ptr)
{
    assert(lua_isinteger(L, read_ptr));
    return static_cast<int>(lua_tointeger(L, read_ptr++));
}


u32 Converter<u32>::pop(lua_State* L)
{
    int ptr = -dimension;
    auto ret = read(L, ptr);
    lua_pop(L, dimension);
    return ret;
}

int Converter<u32>::push(lua_State* L, const u32& value)
{
    lua_pushinteger(L, value);
    return dimension;
}

u32 Converter<u32>::read(lua_State* L, int& read_ptr)
{
    assert(lua_isinteger(L, read_ptr));
    return static_cast<u32>(lua_tointeger(L, read_ptr++));
}


bool Converter<bool>::pop(lua_State* L)
{
    int ptr = -dimension;
    auto ret = read(L, ptr);
    lua_pop(L, dimension);
    return ret;
}

int Converter<bool>::push(lua_State* L, const bool& value)
{
    lua_pushboolean(L, value);
    return dimension;
}

bool Converter<bool>::read(lua_State* L, int& read_ptr)
{
    assert(lua_isboolean(L, read_ptr));
    return static_cast<bool>(lua_toboolean(L, read_ptr++));
}


float Converter<float>::pop(lua_State* L)
{
    int ptr = -dimension;
    auto ret = read(L, ptr);
    lua_pop(L, dimension);
    return ret;
}

int Converter<float>::push(lua_State* L, const float& value)
{
    lua_pushnumber(L, value);
    return dimension;
}

float Converter<float>::read(lua_State* L, int& read_ptr)
{
    assert(lua_isnumber(L, read_ptr));
    return static_cast<float>(lua_tonumber(L, read_ptr++));
}


double Converter<double>::pop(lua_State* L)
{
    int ptr = -dimension;
    auto ret = read(L, ptr);
    lua_pop(L, dimension);
    return ret;
}

int Converter<double>::push(lua_State* L, const double& value)
{
    lua_pushnumber(L, value);
    return dimension;
}

double Converter<double>::read(lua_State* L, int& read_ptr)
{
    assert(lua_isnumber(L, read_ptr));
    return static_cast<double>(lua_tonumber(L, read_ptr++));
}


const char* Converter<const char*>::pop(lua_State* L)
{
    int ptr = -dimension;
    auto ret = read(L, ptr);
    lua_pop(L, dimension);
    return ret;
}

int Converter<const char*>::push(lua_State* L, const char* value)
{
    lua_pushstring(L, value);
    return dimension;
}

const char* Converter<const char*>::read(lua_State* L, int& read_ptr)
{
    assert(lua_isstring(L, read_ptr));
    return lua_tostring(L, read_ptr++);
}


std::string Converter<std::string>::pop(lua_State* L)
{
    int ptr = -dimension;
    auto ret = read(L, ptr);
    lua_pop(L, dimension);
    return ret;
}

int Converter<std::string>::push(lua_State* L, const std::string& value)
{
    lua_pushstring(L, value.c_str());
    return dimension;
}

std::string Converter<std::string>::read(lua_State* L, int& read_ptr)
{
    assert(lua_isstring(L, read_ptr));
    return lua_tostring(L, read_ptr++);
}


void* Converter<void*>::pop(lua_State* L)
{
    int ptr = -dimension;
    auto ret = read(L, ptr);
    lua_pop(L, dimension);
    return ret;
}

int Converter<void*>::push(lua_State* L, void* value)
{
    lua_pushlightuserdata(L, value);
    return dimension;
}

void* Converter<void*>::read(lua_State* L, int& read_ptr)
{
    assert(lua_islightuserdata(L, read_ptr));
    return lua_touserdata(L, read_ptr++);
}


ecs::Entity Converter<ecs::Entity>::pop(lua_State* L)
{
    int ptr = -dimension;
    auto ret = read(L, ptr);
    lua_pop(L, dimension);
    return ret;
}

int Converter<ecs::Entity>::push(lua_State* L, const ecs::Entity& value)
{
    ecs::Entity* handle = static_cast<ecs::Entity*>(lua_newuserdata(L, sizeof(ecs::Entity)));
    *handle = value;
    return dimension;
}

ecs::Entity Converter<ecs::Entity>::read(lua_State* L, int& read_ptr)
{
    assert(lua_isuserdata(L, read_ptr));
    return *static_cast<ecs::Entity*>(lua_touserdata(L, read_ptr++));
}


ecs::Identifier Converter<ecs::Identifier>::pop(lua_State* L)
{
    int ptr = -dimension;
    auto ret = read(L, ptr);
    lua_pop(L, dimension);
    return ret;
}

int Converter<ecs::Identifier>::push(lua_State* L, const ecs::Identifier& value)
{
    ecs::Identifier* handle = static_cast<ecs::Identifier*>(lua_newuserdata(L, sizeof(ecs::Identifier)));
    *handle = value;
    return dimension;
}

ecs::Identifier Converter<ecs::Identifier>::read(lua_State* L, int& read_ptr)
{
    assert(lua_isuserdata(L, read_ptr));
    return *static_cast<ecs::Identifier*>(lua_touserdata(L, read_ptr++));
}


glm::vec2 Converter<glm::vec2>::pop(lua_State* L)
{
    int ptr = -dimension;
    auto ret = read(L, ptr);
    lua_pop(L, dimension);
    return ret;
}

int Converter<glm::vec2>::push(lua_State* L, const glm::vec2& value)
{
    Converter<float>::push(L, value.x);
    Converter<float>::push(L, value.y);
    return dimension;
}

glm::vec2 Converter<glm::vec2>::read(lua_State* L, int& read_ptr)
{
    float x = Converter<float>::read(L, read_ptr);
    float y = Converter<float>::read(L, read_ptr);
    return {x, y};
}


glm::vec3 Converter<glm::vec3>::pop(lua_State* L)
{
    int ptr = -dimension;
    auto ret = read(L, ptr);
    lua_pop(L, dimension);
    return ret;
}

int Converter<glm::vec3>::push(lua_State* L, const glm::vec3& value)
{
    Converter<float>::push(L, value.x);
    Converter<float>::push(L, value.y);
    Converter<float>::push(L, value.z);
    return dimension;
}

glm::vec3 Converter<glm::vec3>::read(lua_State* L, int& read_ptr)
{
    float x = Converter<float>::read(L, read_ptr);
    float y = Converter<float>::read(L, read_ptr);
    float z = Converter<float>::read(L, read_ptr);
    return {x, y, z};
}


}
}