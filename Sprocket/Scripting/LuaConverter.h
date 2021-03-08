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
    static T pull_from(lua_State* L) {
        static_assert(false);
    }

    static int push_to(lua_State* L, const T& value) {
        static_assert(false);
    }
};

template <> struct Converter<int>
{
    static int pull_from(lua_State* L);
    static int push_to(lua_State* L, const int& value);
};

template <> struct Converter<u32>
{
    static u32 pull_from(lua_State* L);
    static int push_to(lua_State* L, const u32& value);
};

template <> struct Converter<bool>
{
    static bool pull_from(lua_State* L);
    static int push_to(lua_State* L, const bool& value);
};

template <> struct Converter<float>
{
    static float pull_from(lua_State* L);
    static int push_to(lua_State* L, const float& value);
};

template <> struct Converter<double>
{
    static double pull_from(lua_State* L);
    static int push_to(lua_State* L, const double& value);
};

template <> struct Converter<const char*>
{
    static const char* pull_from(lua_State* L);
    static int push_to(lua_State* L, const char* value);
};

template <> struct Converter<std::string>
{
    static std::string pull_from(lua_State* L);
    static int push_to(lua_State* L, const std::string& value);
};

template <> struct Converter<void*>
{
    static void* pull_from(lua_State* L);
    static int push_to(lua_State* L, void* value);
};

template <typename T> struct Converter<T*>
{
    static T* pull_from(lua_State* L) {
        return static_cast<T*>(Converter<void*>::pull_from(L));
    }

    static int push_to(lua_State* L, T* value) {
        return Converter<void*>::push_to(L, static_cast<void*>(value));
    }
};

template <> struct Converter<ecs::Entity>
{
    static ecs::Entity pull_from(lua_State* L);
    static int push_to(lua_State* L, ecs::Entity value);
};

// Implement the pull_from for these types when we need them, otherwise we are
// guessing the API here.

template <> struct Converter<glm::vec2>
{
    static glm::vec2 pull_from(lua_State* L) { return glm::vec2{1.0}; } // TODO: Implement
    static int push_to(lua_State* L, const glm::vec2& value);
};

template <> struct Converter<glm::vec3>
{
    static glm::vec3 pull_from(lua_State* L) { return glm::vec3{1.0}; } // TODO: Implement
    static int push_to(lua_State* L, const glm::vec3& value);
};

}
}