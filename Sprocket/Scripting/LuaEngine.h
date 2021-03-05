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

    // Push primitives
    void Push(bool val);
    void Push(char val);
    void Push(int val);
    void Push(float val);
    void Push(double val);

    // Push strings
    void Push(const char* val);
    void Push(const std::string& val);

    // Push pointers
    void Push(void* val);
    template <typename T> void Push(T* val);

    // Push trivial objects
    template <typename T> void Push(const T& val);

    void PrintErrors(int rc) const;

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

    // Window Events
    void CallOnWindowResizeEvent(ev::Event& event);
    void CallOnWindowGotFocusEvent(ev::Event& event);
    void CallOnWindowLostFocusEvent(ev::Event& event);
    void CallOnWindowMaximizeEvent(ev::Event& event);
    void CallOnWindowMinimizeEvent(ev::Event& event);

    // Mouse Events
    void CallOnMouseButtonPressedEvent(ev::Event& event);
    void CallOnMouseButtonReleasedEvent(ev::Event& event);
    void CallOnMouseMovedEvent(ev::Event& event);
    void CallOnMouseScrolledEvent(ev::Event& event);

    // Keyboard Events
    void CallOnKeyboardButtonPressedEvent(ev::Event& event);
    void CallOnKeyboardButtonReleasedEvent(ev::Event& event);
    void CallOnKeyboardButtonHeldEvent(ev::Event& event);
    void CallOnKeyboardKeyTypedEvent(ev::Event& event);

    // Do not copy these things
    LuaEngine(LuaEngine&&) = delete;
};

template <typename T> void LuaEngine::Push(T* val)
{
    Push(static_cast<void*>(val));
}

template <typename T> void LuaEngine::Push(const T& val)
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

    (Push(std::forward<Args>(args)), ...);

    int rc = lua_pcall(d_L, sizeof...(Args), 0, 0);
    PrintErrors(rc);
}

template <typename Type>
void LuaEngine::Set(const std::string& name, Type&& value)
{
    Push(std::forward<Type>(value));
    lua_setglobal(d_L, name.c_str());
}

}