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

void* LuaEngine::allocate(std::size_t size)
{
    return lua_newuserdata(d_L, size);
}

void LuaEngine::Push(bool val)
{
    lua_pushboolean(d_L, val);
}

void LuaEngine::Push(char val)
{
    lua_pushstring(d_L, std::string(1, val).c_str());
}

void LuaEngine::Push(int val)
{
    lua_pushinteger(d_L, val);
}

void LuaEngine::Push(float val)
{
    lua_pushnumber(d_L, val);
}

void LuaEngine::Push(double val)
{
    lua_pushnumber(d_L, val);
}

void LuaEngine::Push(const char* val)
{
    lua_pushstring(d_L, val);
}

void LuaEngine::Push(const std::string& val)
{
    lua_pushstring(d_L, val.c_str());
}

void LuaEngine::Push(void* val)
{
    lua_pushlightuserdata(d_L, val);
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

void LuaEngine::on_event(ev::Event& event)
{
    auto handler = [this, &event](const char* f, auto&&... args)
    {
        lua_getglobal(d_L, f);
        if (!lua_isfunction(d_L, -1)) {
            lua_pop(d_L, -1);
            return;
        }

        Push(event.is_consumed());
        (Push(args), ...);

        int rc = lua_pcall(d_L, 1 + sizeof...(args), 1, 0);

        PrintErrors(rc);
        if (lua_toboolean(d_L, -1)) {
            event.consume();
        }
    };

    if (auto x = event.get_if<ev::WindowResize>()) {
        handler("OnWindowResizeEvent", x->width, x->height);
    }
    else if (auto x = event.get_if<ev::WindowGotFocus>()) {
        handler("OnWindowGotFocusEvent");
    }
    else if (auto x = event.get_if<ev::WindowLostFocus>()) {
        handler("OnWindowLostFocusEvent");
    }
    else if (auto x = event.get_if<ev::WindowMaximize>()) {
        handler("OnWindowMaximizeEvent");
    }
    else if (auto x = event.get_if<ev::WindowMinimize>()) {
        handler("OnWindowMinimizeEvent");
    }
    else if (auto x = event.get_if<ev::MouseButtonPressed>()) {
        handler("OnMouseButtonPressedEvent", x->button, x->action, x->mods);
    }
    else if (auto x = event.get_if<ev::MouseButtonReleased>()) {
        handler("OnMouseButtonReleasedEvent", x->button, x->action, x->mods);
    }
    else if (auto x = event.get_if<ev::MouseMoved>()) {
        handler("OnMouseMovedEvent", x->x_pos, x->y_pos);
    }
    else if (auto x = event.get_if<ev::MouseScrolled>()) {
        handler("OnMouseScrolledEvent", x->x_offset, x->y_offset);
    }
    else if (auto x = event.get_if<ev::KeyboardButtonPressed>()) {
        handler("OnKeyboardButtonPressedEvent", x->key, x->scancode, x->mods);
    }
    else if (auto x = event.get_if<ev::KeyboardButtonReleased>()) {
        handler("OnKeyboardButtonReleasedEvent", x->key, x->scancode, x->mods);
    }
    else if (auto x = event.get_if<ev::KeyboardButtonHeld>()) {
        handler("OnKeyboardButtonHeldEvent", x->key, x->scancode, x->mods);
    }
    else if (auto x = event.get_if<ev::KeyboardTyped>()) {
        handler("OnKeyboardKeyTypedEvent", x->key);
    }
    else {
        log::warn("Event with unknown type {}", event.type_name());
        return;
    }
}

void LuaEngine::CallOnWindowResizeEvent(ev::Event& event)
{
    on_event(event);
}

void LuaEngine::CallOnWindowGotFocusEvent(ev::Event& event)
{
    on_event(event);
}

void LuaEngine::CallOnWindowLostFocusEvent(ev::Event& event)
{
    on_event(event);
}

void LuaEngine::CallOnWindowMaximizeEvent(ev::Event& event)
{
    on_event(event);
}

void LuaEngine::CallOnWindowMinimizeEvent(ev::Event& event)
{
    on_event(event);
}

void LuaEngine::CallOnMouseButtonPressedEvent(ev::Event& event)
{
    on_event(event);
}

void LuaEngine::CallOnMouseButtonReleasedEvent(ev::Event& event)
{
    on_event(event);
}

void LuaEngine::CallOnMouseMovedEvent(ev::Event& event)
{
    on_event(event);
}

void LuaEngine::CallOnMouseScrolledEvent(ev::Event& event)
{
    on_event(event);
}

void LuaEngine::CallOnKeyboardButtonPressedEvent(ev::Event& event)
{
    on_event(event);
}

void LuaEngine::CallOnKeyboardButtonReleasedEvent(ev::Event& event)
{
    on_event(event);
}

void LuaEngine::CallOnKeyboardButtonHeldEvent(ev::Event& event)
{
    on_event(event);
}

void LuaEngine::CallOnKeyboardKeyTypedEvent(ev::Event& event)
{
    on_event(event);
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