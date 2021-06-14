#pragma once
#include "Types.h"

#include "ECS.h"
#include <cstddef>
#include <string>

#include <glm/glm.hpp>

struct lua_State;

namespace Sprocket {
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

template <> struct Converter<u32>
{
    static void push(lua_State* L, const u32& value);
    static u32 read(lua_State* L, int index);
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

template <> struct Converter<spkt::entity>
{
    static void push(lua_State* L, const spkt::entity& value);
    static spkt::entity read(lua_State* L, int index);
};

template <> struct Converter<spkt::identifier>
{
    static void push(lua_State* L, const spkt::identifier& value);
    static spkt::identifier read(lua_State* L, int index);
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

}
}