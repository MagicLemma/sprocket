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

template <typename T> struct Converter
{
    // Pushes to the top of the Lua stack.
    static void push(lua_State* L, const T& value) { static_assert(false); }
    
    // Reads the value at the given index in the Lua stack.
    static T read(lua_State* L, int index) { static_assert(false); }
};

template <> struct Converter<int>
{
    static void push(lua_State* L, const int& value);
    static int read(lua_State* L, int index);
};

template <> struct Converter<std::uint32_t>
{
    static void push(lua_State* L, const std::uint32_t& value);
    static std::uint32_t read(lua_State* L, int index);
};

template <> struct Converter<bool>
{
    static void push(lua_State* L, const bool& value);
    static bool read(lua_State* L, int index);
};

template <> struct Converter<float>
{
    static void push(lua_State* L, const float& value);
    static float read(lua_State* L, int index);
};

template <> struct Converter<double>
{
    static void push(lua_State* L, const double& value);
    static double read(lua_State* L, int index);
};

template <> struct Converter<const char*>
{
    static void push(lua_State* L, const char* value);
    static const char* read(lua_State* L, int index);
};

template <> struct Converter<std::string>
{
    static void push(lua_State* L, const std::string& value);
    static std::string read(lua_State* L, int index);
};

template <> struct Converter<void*>
{
    static void push(lua_State* L, void* value);
    static void* read(lua_State* L, int index);
};

template <typename T> struct Converter<T*>
{
    static void push(lua_State* L, T* value) {
        return Converter<void*>::push(L, (void*)value);
    }
    static T* read(lua_State* L, int index) {
        return (T*)Converter<void*>::read(L, index);
    }
};

template <> struct Converter<spkt::handle>
{
    static void push(lua_State* L, const spkt::handle& value);
    static spkt::handle read(lua_State* L, int index);
};

template <> struct Converter<spkt::entity>
{
    static void push(lua_State* L, const spkt::entity& value);
    static spkt::entity read(lua_State* L, int index);
};

template <> struct Converter<glm::vec2>
{
    static void push(lua_State* L, const glm::vec2& value);
    static glm::vec2 read(lua_State* L, int index);
};

template <> struct Converter<glm::vec3>
{
    static void push(lua_State* L, const glm::vec3& value);
    static glm::vec3 read(lua_State* L, int index);
};

template <typename T, typename... Rest> struct Converter<std::vector<T, Rest...>>
{
    using vector_t = std::vector<T, Rest...>;
    using value_type = typename vector_t::value_type;

    static void push(lua_State* L, const vector_t& value)
    {
        lua_newtable(L);
        int index = 1;
        for (const auto& elem : value) {
            Converter<value_type>::push(L, elem);
            lua_rawseti(L, -2, index++);
        }
    }

    static vector_t read(lua_State* L, int index)
    {
        vector_t value;
        assert(lua_istable(L, index));
        lua_pushnil(L);
        while (0 != lua_next(L, index)) {
            value.push_back(Converter<value_type>::read(L, index + 2));
            lua_pop(L, 1);
        }
        return value;
    }
};

template <typename Left, typename Right> struct Converter<std::pair<Left, Right>>
{
    using pair_t = std::pair<Left, Right>;
    
    static void push(lua_State* L, const pair_t& value)
    {
        lua_newtable(L);
        Converter<Left>::push(L, value.first);
        lua_rawseti(L, -2, 1);
        Converter<Right>::push(L, value.second);
        lua_rawseti(L, -2, 2);
    }

    static pair_t read(lua_State* L, int index)
    {
        pair_t value;
        assert(lua_istable(L, index));
        lua_pushnil(L);

        int rc = lua_next(L, index);
        assert(rc != 0);
        value.first = Converter<Left>::read(L, index + 2);
        lua_pop(L, 1);

        rc = lua_next(L, index);
        assert(rc != 0);
        value.second = Converter<Right>::read(L, index + 2);
        lua_pop(L, 1);

        rc = lua_next(L, index);
        assert(rc == 0);

        return value;
    }
};

}
}