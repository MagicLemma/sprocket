#include "LuaEngine.h"
#include "LuaGlobals.h"
#include "LuaEntity.h"
#include "LuaTransform.h"
#include "LuaInput.h"
#include "LuaComponents.h"
#include "Log.h"
#include "ECS.h"
#include "Components.h"

#include <lua.hpp>

namespace Sprocket {

namespace {

void DoFile(lua_State* L, const char* file)
{
    if (luaL_dofile(L, file)) {
        log::error("[Lua]: Could not load {}", lua_tostring(L, -1));
    }
}

}

LuaEngine::LuaEngine()
    : d_L(luaL_newstate())
{
    luaL_openlibs(d_L);

    // Core Sprocket Constants and Helper Functions
    DoFile(d_L, "Sprocket/Scripting/Sprocket_Base.lua");
    DoFile(d_L, "Sprocket/Scripting/Sprocket_Maths.lua");
    DoFile(d_L, "Sprocket/Scripting/Sprocket_Bindings.lua");
    DoFile(d_L, "Sprocket/Scripting/Sprocket_Components.lua");
    DoFile(d_L, "Sprocket/Scripting/Sprocket_Scene.lua");

    RegisterTransformFunctions(d_L);
    RegisterInputFunctions(d_L);
    RegisterComponentFunctions(d_L);
    RegisterEntityFunctions(d_L);
}

LuaEngine::~LuaEngine()
{
    lua_close(d_L);
}

void LuaEngine::PrintErrors(int rc) const
{
    if (rc == LUA_OK) { return; } // No error
    
    std::string err = lua_tostring(d_L, -1);
    switch (rc) {
        case LUA_ERRRUN:
            log::error("[Lua]: Runtime error: {}", err);
            return;
        case LUA_ERRMEM:
            log::error("[Lua]: Memory allocation error: {}", err);
            return;
        case LUA_ERRERR:
            log::error("[Lua]: Error handler func failed: {}", err);
            return;
        default:
            log::error("[Lua]: Unknown error: {}", err);
    }
}

void LuaEngine::RunScript(const std::string& filename)
{
    if (filename.empty()) {
        log::warn("Tried to start an empty script!");
        return;
    }
    
    DoFile(d_L, filename.c_str());
}

void LuaEngine::CallOnWindowResizeEvent(ev::Event& event)
{
    assert(event.is<ev::WindowResize>());
    lua_getglobal(d_L, "OnWindowResizeEvent");

    if (!lua_isfunction(d_L, -1)) {
        lua_pop(d_L, -1);
        return;
    }

    const auto& data = event.get<ev::WindowResize>();
    Push(event.is_consumed());
    Push(data.width);
    Push(data.height);

    int rc = lua_pcall(d_L, 3, 1, 0);
    PrintErrors(rc);

    if (lua_toboolean(d_L, -1)) { event.consume(); }
}

void LuaEngine::CallOnWindowGotFocusEvent(ev::Event& event)
{
    assert(event.is<ev::WindowGotFocus>());
    lua_getglobal(d_L, "OnWindowGotFocusEvent");

    if (!lua_isfunction(d_L, -1)) {
        lua_pop(d_L, -1);
        return;
    }
    
    Push(event.is_consumed());

    int rc = lua_pcall(d_L, 1, 1, 0);
    PrintErrors(rc);

    if (lua_toboolean(d_L, -1)) { event.consume(); }
}

void LuaEngine::CallOnWindowLostFocusEvent(ev::Event& event)
{
    assert(event.is<ev::WindowLostFocus>());
    lua_getglobal(d_L, "OnWindowLostFocusEvent");

    if (!lua_isfunction(d_L, -1)) {
        lua_pop(d_L, -1);
        return;
    }
    
    Push(event.is_consumed());

    int rc = lua_pcall(d_L, 1, 1, 0);
    PrintErrors(rc);

    if (lua_toboolean(d_L, -1)) { event.consume(); }
}

void LuaEngine::CallOnWindowMaximizeEvent(ev::Event& event)
{
    assert(event.is<ev::WindowMaximize>());
    lua_getglobal(d_L, "OnWindowMaximizeEvent");

    if (!lua_isfunction(d_L, -1)) {
        lua_pop(d_L, -1);
        return;
    }
    
    Push(event.is_consumed());

    int rc = lua_pcall(d_L, 1, 1, 0);
    PrintErrors(rc);

    if (lua_toboolean(d_L, -1)) { event.consume(); }
}

void LuaEngine::CallOnWindowMinimizeEvent(ev::Event& event)
{
    assert(event.is<ev::WindowMinimize>());
    lua_getglobal(d_L, "OnWindowMinimizeEvent");

    if (!lua_isfunction(d_L, -1)) {
        lua_pop(d_L, -1);
        return;
    }
    
    Push(event.is_consumed());

    int rc = lua_pcall(d_L, 1, 1, 0);
    PrintErrors(rc);

    if (lua_toboolean(d_L, -1)) { event.consume(); }
}

void LuaEngine::CallOnMouseButtonPressedEvent(ev::Event& event)
{
    assert(event.is<ev::MouseButtonPressed>());
    lua_getglobal(d_L, "OnMouseButtonPressedEvent");

    if (!lua_isfunction(d_L, -1)) {
        lua_pop(d_L, -1);
        return;
    }
    
    const auto& data = event.get<ev::MouseButtonPressed>();
    Push(event.is_consumed());
    Push(data.button);
    Push(data.action);
    Push(data.mods);

    int rc = lua_pcall(d_L, 4, 1, 0);
    PrintErrors(rc);

    if (lua_toboolean(d_L, -1)) { event.consume(); }
}


void LuaEngine::CallOnMouseButtonReleasedEvent(ev::Event& event)
{
    assert(event.is<ev::MouseButtonReleased>());
    lua_getglobal(d_L, "MouseButtonReleasedEvent");

    if (!lua_isfunction(d_L, -1)) {
        lua_pop(d_L, -1);
        return;
    }
    
    const auto& data = event.get<ev::MouseButtonReleased>();
    Push(event.is_consumed());
    Push(data.button);
    Push(data.action);
    Push(data.mods);

    int rc = lua_pcall(d_L, 4, 1, 0);
    PrintErrors(rc);

    if (lua_toboolean(d_L, -1)) { event.consume(); }
}

void LuaEngine::CallOnMouseMovedEvent(ev::Event& event)
{
    assert(event.is<ev::MouseMoved>());
    lua_getglobal(d_L, "OnMouseMovedEvent");

    if (!lua_isfunction(d_L, -1)) {
        lua_pop(d_L, -1);
        return;
    }
    
    const auto& data = event.get<ev::MouseMoved>();
    Push(event.is_consumed());
    Push(data.x_pos);
    Push(data.y_pos);

    int rc = lua_pcall(d_L, 3, 1, 0);
    PrintErrors(rc);

    if (lua_toboolean(d_L, -1)) { event.consume(); }
}

void LuaEngine::CallOnMouseScrolledEvent(ev::Event& event)
{
    assert(event.is<ev::MouseScrolled>());
    lua_getglobal(d_L, "OnMouseScrolledEvent");

    if (!lua_isfunction(d_L, -1)) {
        lua_pop(d_L, -1);
        return;
    }
    
    const auto& data = event.get<ev::MouseScrolled>();
    Push(event.is_consumed());
    Push(data.x_offset);
    Push(data.y_offset);

    int rc = lua_pcall(d_L, 3, 1, 0);
    PrintErrors(rc);

    if (lua_toboolean(d_L, -1)) { event.consume(); }
}

void LuaEngine::CallOnKeyboardButtonPressedEvent(ev::Event& event)
{
    assert(event.is<ev::KeyboardButtonPressed>());
    lua_getglobal(d_L, "OnKeyboardButtonPressedEvent");

    if (!lua_isfunction(d_L, -1)) {
        lua_pop(d_L, -1);
        return;
    }

    const auto& data = event.get<ev::KeyboardButtonPressed>();
    Push(event.is_consumed());
    Push(data.key);
    Push(data.scancode);
    Push(data.mods);

    int rc = lua_pcall(d_L, 4, 1, 0);
    PrintErrors(rc);

    if (lua_toboolean(d_L, -1)) { event.consume(); }
}

void LuaEngine::CallOnKeyboardButtonReleasedEvent(ev::Event& event)
{
    assert(event.is<ev::KeyboardButtonReleased>());
    lua_getglobal(d_L, "OnKeyboardButtonReleasedEvent");

    if (!lua_isfunction(d_L, -1)) {
        lua_pop(d_L, -1);
        return;
    }

    const auto& data = event.get<ev::KeyboardButtonReleased>();
    Push(event.is_consumed());
    Push(data.key);
    Push(data.scancode);
    Push(data.mods);

    int rc = lua_pcall(d_L, 4, 1, 0);
    PrintErrors(rc);

    if (lua_toboolean(d_L, -1)) { event.consume(); }
}

void LuaEngine::CallOnKeyboardButtonHeldEvent(ev::Event& event)
{
    assert(event.is<ev::KeyboardButtonHeld>());
    lua_getglobal(d_L, "OnKeyboardButtonHeldEvent");

    if (!lua_isfunction(d_L, -1)) {
        lua_pop(d_L, -1);
        return;
    }

    const auto& data = event.get<ev::KeyboardButtonHeld>();
    Push(event.is_consumed());
    Push(data.key);
    Push(data.scancode);
    Push(data.mods);

    int rc = lua_pcall(d_L, 4, 1, 0);
    PrintErrors(rc);

    if (lua_toboolean(d_L, -1)) { event.consume(); }
}

void LuaEngine::CallOnKeyboardKeyTypedEvent(ev::Event& event)
{
    assert(event.is<ev::KeyboardTyped>());
    lua_getglobal(d_L, "OnKeyboardKeyTypedEvent");

    if (!lua_isfunction(d_L, -1)) {
        lua_pop(d_L, -1);
        return;
    }

    const auto& data = event.get<ev::KeyboardTyped>();
    Push(event.is_consumed());
    Push(data.key);

    int rc = lua_pcall(d_L, 2, 1, 0);
    PrintErrors(rc);

    if (lua_toboolean(d_L, -1)) { event.consume(); }
}

void LuaEngine::PrintGlobals()
{
    log::info("Starting globals");
    lua_pushglobaltable(d_L);
    lua_pushnil(d_L);
    while (lua_next(d_L, -2) != 0) {
        if (lua_isnumber(d_L, -1)) {
            log::info("{} = {}", lua_tostring(d_L, -2), lua_tonumber(d_L, -1));
        }
        lua_pop(d_L, 1);
    }
    lua_pop(d_L, 1);
    log::info("Ending globals");
}

}