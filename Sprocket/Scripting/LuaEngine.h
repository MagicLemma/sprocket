#pragma once
#include "Events.h"
#include "InputProxy.h"
#include "Scene.h"
#include "Window.h"

#include <string>
#include <unordered_map>

#include <lua.hpp>

namespace Sprocket {

class LuaEngine
{
    lua_State* d_L;

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

public:
    LuaEngine();
    ~LuaEngine();

    void RunScript(const std::string& filename);

    // TODO: Remove
    void PrintGlobals();

    template <typename... Args>
    void Call(const std::string& function, Args&&... args);

    template <typename Type>
    void Set(const std::string& name, Type&& value);

    void on_event(ev::Event& event);

    // Do not copy these things
    LuaEngine(LuaEngine&&) = delete;
};

template <typename T> void LuaEngine::push_value(T* val)
{
    push_value(static_cast<void*>(val));
}

template <typename T> void LuaEngine::push_value(const T& val)
{
    static_assert(std::is_copy_assignable_v<T>);
    static_assert(std::is_trivially_destructible_v<T>);
    *static_cast<T*>(allocate(sizeof(T))) = val;
}

template <typename... Args>
void LuaEngine::Call(const std::string& function, Args&&... args)
{
    lua_getglobal(d_L, function.c_str());
    if (!lua_isfunction(d_L, -1)) {
        lua_pop(d_L, -1);
        return;
    }

    (push_value(std::forward<Args>(args)), ...);

    int rc = lua_pcall(d_L, sizeof...(Args), 0, 0);
    print_errors(rc);
}

template <typename Type>
void LuaEngine::Set(const std::string& name, Type&& value)
{
    push_value(std::forward<Type>(value));
    lua_setglobal(d_L, name.c_str());
}

}