#pragma once
#include <Sprocket/Scene/ecs.h>

#include <glm/glm.hpp>
#include <lua.hpp>

#include <cstddef>
#include <string>
#include <vector>
#include <utility>
#include <cassert>

struct lua_State;

namespace spkt {
namespace lua {

template <typename T>
struct converter;

template <>
struct converter<int>
{
    static void push(lua_State* L, const int& value);
    static int read(lua_State* L, int index);
};

template <>
struct converter<std::uint32_t>
{
    static void push(lua_State* L, const std::uint32_t& value);
    static std::uint32_t read(lua_State* L, int index);
};

template <>
struct converter<bool>
{
    static void push(lua_State* L, const bool& value);
    static bool read(lua_State* L, int index);
};

template <>
struct converter<float>
{
    static void push(lua_State* L, const float& value);
    static float read(lua_State* L, int index);
};

template <>
struct converter<double>
{
    static void push(lua_State* L, const double& value);
    static double read(lua_State* L, int index);
};

template <>
struct converter<const char*>
{
    static void push(lua_State* L, const char* value);
    static const char* read(lua_State* L, int index);
};

template <>
struct converter<std::string>
{
    static void push(lua_State* L, const std::string& value);
    static std::string read(lua_State* L, int index);
};

template <>
struct converter<void*>
{
    static void push(lua_State* L, void* value);
    static void* read(lua_State* L, int index);
};

template <typename T>
struct converter<T*>
{
    static void push(lua_State* L, T* value) {
        return converter<void*>::push(L, (void*)value);
    }
    static T* read(lua_State* L, int index) {
        return (T*)converter<void*>::read(L, index);
    }
};

template <>
struct converter<glm::vec2>
{
    static void push(lua_State* L, const glm::vec2& value);
    static glm::vec2 read(lua_State* L, int index);
};

template <>
struct converter<glm::vec3>
{
    static void push(lua_State* L, const glm::vec3& value);
    static glm::vec3 read(lua_State* L, int index);
};

template <typename T, typename... Rest>
struct converter<std::vector<T, Rest...>>
{
    using vector_t = std::vector<T, Rest...>;
    using value_type = typename vector_t::value_type;

    static void push(lua_State* L, const vector_t& value)
    {
        lua_newtable(L);
        int index = 1;
        for (const auto& elem : value) {
            converter<value_type>::push(L, elem);
            lua_rawseti(L, -2, index++);
        }
    }

    static vector_t read(lua_State* L, int index)
    {
        vector_t value;
        assert(lua_istable(L, index));
        lua_pushnil(L);
        while (0 != lua_next(L, index)) {
            value.push_back(converter<value_type>::read(L, index + 2));
            lua_pop(L, 1);
        }
        return value;
    }
};

template <typename Left, typename Right>
struct converter<std::pair<Left, Right>>
{
    using pair_t = std::pair<Left, Right>;
    
    static void push(lua_State* L, const pair_t& value)
    {
        lua_newtable(L);
        converter<Left>::push(L, value.first);
        lua_rawseti(L, -2, 1);
        converter<Right>::push(L, value.second);
        lua_rawseti(L, -2, 2);
    }

    static pair_t read(lua_State* L, int index)
    {
        pair_t value;
        assert(lua_istable(L, index));
        lua_pushnil(L);

        int rc = lua_next(L, index);
        assert(rc != 0);
        value.first = converter<Left>::read(L, index + 2);
        lua_pop(L, 1);

        rc = lua_next(L, index);
        assert(rc != 0);
        value.second = converter<Right>::read(L, index + 2);
        lua_pop(L, 1);

        rc = lua_next(L, index);
        assert(rc == 0);

        return value;
    }
};

}
}