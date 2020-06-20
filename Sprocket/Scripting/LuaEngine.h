#pragma once
#include "Event.h"
#include "KeyboardProxy.h"
#include "MouseProxy.h"
#include "MouseEvent.h"
#include "WindowEvent.h"

#include <string>

class lua_State;

namespace Sprocket {

class Entity;

class LuaEngine
{
    lua_State* d_L;

public:
    LuaEngine();
    ~LuaEngine();

    void RunScript(const std::string& filename);

    void CallInitFunction();
    void CallOnUpdateFunction(double dt);

    // Window Events
    void CallOnWindowResizeEvent(WindowResizeEvent* e);

    // Mouse Events
    void CallOnMouseButtonPressedEvent(MouseButtonPressedEvent* e);
    void CallOnMouseScrolledEvent(MouseScrolledEvent* e);

    // Keyboard Events

    void SetEntity(const Entity& e);
    void SetKeyboard(KeyboardProxy* k);
    void SetMouse(MouseProxy* m);
};

}