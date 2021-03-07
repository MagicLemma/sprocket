#pragma once
#include "Types.h"

#include "ECS.h"
#include <cstddef>
#include <string>

struct lua_State;

namespace Sprocket {
namespace lua {

template <typename T>
struct Converter
{
    static T pull_from(lua_State* L) {
        static_assert(false);
    }

    static void push_to(lua_State* L, const T& value) {
        static_assert(false);
    }

    static std::size_t arg_count() {
        static_assert(false);
    }
};

template <>
struct Converter<int>
{
    static int pull_from(lua_State* L);
    static void push_to(lua_State* L, const int& value);
    static std::size_t arg_count() { return 1; }
};

template <>
struct Converter<u32>
{
    static u32 pull_from(lua_State* L);
    static void push_to(lua_State* L, const u32& value);
    static std::size_t arg_count() { return 1; }
};

template <>
struct Converter<bool>
{
    static bool pull_from(lua_State* L);
    static void push_to(lua_State* L, const bool& value);
    static std::size_t arg_count() { return 1; }
};

template <>
struct Converter<float>
{
    static float pull_from(lua_State* L);
    static void push_to(lua_State* L, const float& value);
    static std::size_t arg_count() { return 1; }
};

template <>
struct Converter<double>
{
    static double pull_from(lua_State* L);
    static void push_to(lua_State* L, const double& value);
    static std::size_t arg_count() { return 1; }
};

template <>
struct Converter<const char*>
{
    static const char* pull_from(lua_State* L);
    static void push_to(lua_State* L, const char* value);
    static std::size_t arg_count() { return 1; }
};

template <>
struct Converter<std::string>
{
    static std::string pull_from(lua_State* L);
    static void push_to(lua_State* L, const std::string& value);
    static std::size_t arg_count() { return 1; }
};

template <>
struct Converter<void*>
{
    static void* pull_from(lua_State* L);
    static void push_to(lua_State* L, void* value);
    static std::size_t arg_count() { return 1; }   
};

template <typename T>
struct Converter<T*>
{
    static T* pull_from(lua_State* L) {
        return static_cast<T*>(Converter<void*>::pull_from(L));
    }

    static void push_to(lua_State* L, T* value) {
        Converter<void*>::push_to(L, static_cast<void*>(value));
    }

    static std::size_t arg_count() { return 1; }
};

template <>
struct Converter<ecs::Entity>
{
    static ecs::Entity pull_from(lua_State* L);
    static void push_to(lua_State* L, ecs::Entity value);
    static std::size_t arg_count() { return 1; }   
};

}
}