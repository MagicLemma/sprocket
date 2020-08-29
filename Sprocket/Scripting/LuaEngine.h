#pragma once
#include "Event.h"
#include "KeyboardProxy.h"
#include "MouseProxy.h"
#include "MouseEvent.h"
#include "WindowEvent.h"
#include "KeyboardEvent.h"
#include "Entity.h"

#include <string>
#include <unordered_map>

class lua_State;

namespace Sprocket {

class LuaEngine
{
    lua_State* d_L;

    Entity d_entity;

    // Entities created in scripts by this entity. These are stored
    // as handles are needed to make them accessible in Lua.
    std::unordered_map<std::uint32_t, Entity> d_spawnedEntities;

public:
    LuaEngine();
    ~LuaEngine();

    void RunScript(const std::string& filename);

    void CallInitFunction();
    void CallOnUpdateFunction(double dt);

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
    void SetEntity(const Entity& e);
    void SetKeyboard(KeyboardProxy* k);
    void SetMouse(MouseProxy* m);

    // Do not copy these things
    LuaEngine(LuaEngine&&) = delete;
};

}