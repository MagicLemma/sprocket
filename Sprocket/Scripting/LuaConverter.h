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
    // Pulls from the top of the stack and removes the values.
    static T pull_from(lua_State* L) { static_assert(false); }

    // Pushes the given value to the top of the stack. More complicated object are pushed
    // as their parts, for example, vec3 is pushed as 3 floats.
    static int push_to(lua_State* L, const T& value) { static_assert(false); }

    // Constructs a T by getting values from the stack starting at position "count",
    // and goes up the stack if it needs more than one value. "count" will be modified
    // to point at the next position in the stack immediately after the last element
    // used to make T. No values are popped from the stack. This is intended to be used
    // in C++ implementations of Lua functions where more complicated objects must be
    // passed by their parts, such as a vec3 being passed as 3 floats.
    static T get_from_signature(lua_State* L, int& count) { static_assert(false); }
};

template <> struct Converter<int>
{
    static int pull_from(lua_State* L);
    static int push_to(lua_State* L, const int& value);
    static int get_from_signature(lua_State* L, int& count);
};

template <> struct Converter<u32>
{
    static u32 pull_from(lua_State* L);
    static int push_to(lua_State* L, const u32& value);
    static u32 get_from_signature(lua_State* L, int& count);
};

template <> struct Converter<bool>
{
    static bool pull_from(lua_State* L);
    static int push_to(lua_State* L, const bool& value);
    static bool get_from_signature(lua_State* L, int& count);
};

template <> struct Converter<float>
{
    static float pull_from(lua_State* L);
    static int push_to(lua_State* L, const float& value);
    static float get_from_signature(lua_State* L, int& count);
};

template <> struct Converter<double>
{
    static double pull_from(lua_State* L);
    static int push_to(lua_State* L, const double& value);
    static double get_from_signature(lua_State* L, int& count);
};

template <> struct Converter<const char*>
{
    static const char* pull_from(lua_State* L);
    static int push_to(lua_State* L, const char* value);
    static const char* get_from_signature(lua_State* L, int& count);
};

template <> struct Converter<std::string>
{
    static std::string pull_from(lua_State* L);
    static int push_to(lua_State* L, const std::string& value);
    static std::string get_from_signature(lua_State* L, int& count);
};

template <> struct Converter<void*>
{
    static void* pull_from(lua_State* L);
    static int push_to(lua_State* L, void* value);
    static void* get_from_signature(lua_State* L, int& count);
};

template <typename T> struct Converter<T*>
{
    static T* pull_from(lua_State* L) {
        return static_cast<T*>(Converter<void*>::pull_from(L));
    }

    static int push_to(lua_State* L, T* value) {
        return Converter<void*>::push_to(L, static_cast<void*>(value));
    }

    static T* get_from_signature(lua_State* L, int& count) {
        return static_cast<T*>(Converter<void*>::get_from_signature(L, count));
    }
};

template <> struct Converter<ecs::Entity>
{
    static ecs::Entity pull_from(lua_State* L);
    static int push_to(lua_State* L, ecs::Entity value);
    static ecs::Entity get_from_signature(lua_State* L, int& count);
};

// Implement the pull_from for these types when we need them, otherwise we are
// guessing the API here.

template <> struct Converter<glm::vec2>
{
    static glm::vec2 pull_from(lua_State* L) { return glm::vec2{1.0}; } // TODO: Implement
    static int push_to(lua_State* L, const glm::vec2& value);
    static glm::vec2 get_from_signature(lua_State* L, int& count);
};

template <> struct Converter<glm::vec3>
{
    static glm::vec3 pull_from(lua_State* L) { return glm::vec3{1.0}; } // TODO: Implement
    static int push_to(lua_State* L, const glm::vec3& value);
    static glm::vec3 get_from_signature(lua_State* L, int& count);
};

}
}