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

public:
    LuaEngine();
    ~LuaEngine();

    void RunScript(const std::string& filename);

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

}