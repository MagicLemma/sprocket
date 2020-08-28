#pragma once
#include "Event.h"
#include "KeyboardProxy.h"
#include "MouseProxy.h"
#include "MouseEvent.h"
#include "WindowEvent.h"
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

    // Mouse Events
    void CallOnMouseButtonPressedEvent(MouseButtonPressedEvent* e);
    void CallOnMouseScrolledEvent(MouseScrolledEvent* e);

    // Keyboard Events

    // Setters
    void SetEntity(const Entity& e);
    void SetKeyboard(KeyboardProxy* k);
    void SetMouse(MouseProxy* m);

    // Do not copy these things
    LuaEngine(LuaEngine&&) = delete;
};

}