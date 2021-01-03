#pragma once
#include "Event.h"
#include "InputProxy.h"
#include "MouseEvent.h"
#include "WindowEvent.h"
#include "KeyboardEvent.h"
#include "ECS.h"
#include "Scene.h"
#include "Window.h"

#include <string>
#include <unordered_map>

class lua_State;

namespace Sprocket {

class LuaEngine
{
    lua_State* d_L;

    template <typename T>
    void Push(const T& val);

    template <typename T>
    void Push(T* val);

    template <typename Arg, typename... Args>
    void PushValues(Arg&& arg, Args&&... args);

public:
    LuaEngine();
    ~LuaEngine();

    void RunScript(const std::string& filename);

    template <typename... Args>
    void Call(const std::string& function, Args&&... args);

    void CallInitFunction(ecs::Entity entity);
    void CallOnUpdateFunction(ecs::Entity entity, double dt);

    // Window Events
    void CallOnWindowResizeEvent(WindowResizeEvent* e);
    void CallOnWindowClosedEvent(WindowClosedEvent* e);
    void CallOnWindowGotFocusEvent(WindowGotFocusEvent* e);
    void CallOnWindowLostFocusEvent(WindowLostFocusEvent* e);
    void CallOnWindowMaximizeEvent(WindowMaximizeEvent* e);
    void CallOnWindowMinimizeEvent(WindowMinimizeEvent* e);

    // Mouse Events
    void CallOnMouseButtonPressedEvent(MouseButtonPressedEvent* e);
    void CallOnMouseButtonReleasedEvent(MouseButtonReleasedEvent* e);
    void CallOnMouseMovedEvent(MouseMovedEvent* e);
    void CallOnMouseScrolledEvent(MouseScrolledEvent* e);

    // Keyboard Events
    void CallOnKeyboardButtonPressedEvent(KeyboardButtonPressedEvent* e);
    void CallOnKeyboardButtonReleasedEvent(KeyboardButtonReleasedEvent* e);
    void CallOnKeyboardButtonHeldEvent(KeyboardButtonHeldEvent* e);
    void CallOnKeyboardKeyTypedEvent(KeyboardKeyTypedEvent* e);

    // Setters
    void SetScene(Scene* s);
    void SetWindow(Window* w);
    void SetInput(InputProxy* ip);

    // Do not copy these things
    LuaEngine(LuaEngine&&) = delete;
};

template <typename T>
void LuaEngine::Push(const T& val)
{
    if constexpr (std::is_same<T, bool>()) {
        lua_pushboolean(d_L, val);
    }
    else if constexpr (std::is_integral<T>()) {
        lua_pushinteger(d_L, val);
    }
    else if constexpr (std::is_floating_point<T>()) {
        lua_pushnumber(d_L, val);
    }
    else if constexpr (std::is_same<T, std::string>()) {
        lua_pushstring(d_L, val);
    }
    else if constexpr (std::is_copy_assignable<T>() && std::is_trivially_destructible<T>()) {
        *(T*)lua_newuserdata(d_L, sizeof(T)) = val;
    }
    else {
        static_assert(false);
    }
}

template <typename T>
void LuaEngine::Push(T* val)
{
    lua_pushlightuserdata(d_L, (void*)val);
}

template <typename Arg, typename... Args>
void PushValues(Arg&& arg, Args&&... args)
{
    Push(std::forward<Arg>(arg));
    Push(std::forward<Args>(args)...);
}

template <typename... Args>
void Call(const std::string& function, Args&&... args)
{
    lua_getglobal(d_L, function.c_str());
    if (!lua_isfunction(d_L, -1)) {
        lua_pop(d_L, -1);
        return;
    }

    PushValues(std::forward<Args>(args)...);

    int rc = lua_pcall(d_L, sizeof...(Args), 0, 0);
    PrintErrors(d_L, rc);
}

}