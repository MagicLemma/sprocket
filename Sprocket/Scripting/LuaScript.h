#pragma once
#include "Events.h"
#include "Log.h"

#include <string>
#include <memory>

#include <lua.hpp>
#include <cassert>

namespace Sprocket {
namespace lua {

class Script
{
    std::unique_ptr<lua_State, void(*)(lua_State*)> d_L;

public:
    explicit Script();
    explicit Script(const std::string& file);

    // TODO: Remove
    void print_globals();

    template <typename Return, typename... Args>
    Return call_function(const std::string& function, Args&&... args);

    template <typename Type>
    void set_value(const std::string& name, Type&& value);

    void on_event(ev::Event& event);

    lua_State* native_handle() const { return d_L.get(); }

private:
    void push_value(bool val);
    void push_value(char val);
    void push_value(int val);
    void push_value(float val);
    void push_value(double val);
    void push_value(const char* val);
    void push_value(const std::string& val);
    void push_value(void* val);
    template <typename T> void push_value(T* val);
    template <typename T> void push_value(const T& val);

    void print_errors(int rc) const;

    void* allocate(std::size_t size);
};

template <typename T> void Script::push_value(T* val)
{
    push_value(static_cast<void*>(val));
}

template <typename T> void Script::push_value(const T& val)
{
    static_assert(std::is_copy_assignable_v<T>);
    static_assert(std::is_trivially_destructible_v<T>);
    *static_cast<T*>(allocate(sizeof(T))) = val;
}

template <typename Return, typename... Args>
Return Script::call_function(const std::string& function, Args&&... args)
{
    lua_State* L = d_L.get();

    lua_getglobal(L, function.c_str());
    if (!lua_isfunction(L, -1)) {
        log::error("Could not find function '{}'", function);
        lua_pop(L, -1);
        if constexpr (!std::is_void_v<Return>) {
            return {};
        } else {
            return;
        }
    }

    (push_value(std::forward<Args>(args)), ...);

    if constexpr (std::is_void_v<Return>) {
        int rc = lua_pcall(L, sizeof...(Args), 0, 0);
        print_errors(rc);
    }
    else {
        int rc = lua_pcall(L, sizeof...(Args), 1, 0);
        print_errors(rc);
        if (rc != LUA_OK) {
            return {};
        }
        if constexpr (std::is_same_v<Return, bool>) {
            assert(lua_isboolean(L, -1));
            return static_cast<bool>(lua_toboolean(L, -1));
        }
        else if constexpr (std::is_integral_v<Return>) {
            assert(lua_isinteger(L, -1));
            return lua_tointeger(L, -1);
        }
        else if constexpr (std::is_floating_point_v<Return>) {
            assert(lua_isnumber(L, -1));
            return static_cast<Return>(lua_tonumber(L, -1));
        }
        else if constexpr (std::is_same_v<Return, const char*>) {
            assert(lua_isstring(L, -1));
            return static_cast<const char*>(lua_tostring(L, -1));
        }
        else if constexpr (std::is_same_v<Return, std::string>) {
            assert(lua_isstring(L, -1));
            return std::string(lua_tostring(L, -1));
        }
        else if constexpr (std::is_pointer_v<Return>) {
            assert(lua_islightuserdata(L, -1));
            return static_cast<Return>(lua_touserdata(L, -1));
        }
        else {
            static_assert(false, "Cannot convert to this type from a lua call!");
        }
    }
}

template <typename Type>
void Script::set_value(const std::string& name, Type&& value)
{
    push_value(std::forward<Type>(value));
    lua_setglobal(d_L.get(), name.c_str());
}

}
}