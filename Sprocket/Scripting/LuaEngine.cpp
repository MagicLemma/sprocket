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

void LuaEngine::CallOnWindowResizeEvent(WindowResizeEvent* e)
{
    lua_getglobal(d_L, "OnWindowResizeEvent");

    if (!lua_isfunction(d_L, -1)) {
        lua_pop(d_L, -1);
        return;
    }

    Push(e->IsConsumed());
    Push(e->Width());
    Push(e->Height());

    int rc = lua_pcall(d_L, 3, 1, 0);
    PrintErrors(rc);

    if (lua_toboolean(d_L, -1)) { e->Consume(); }
}

void LuaEngine::CallOnWindowClosedEvent(WindowClosedEvent* e)
{}

void LuaEngine::CallOnWindowGotFocusEvent(WindowGotFocusEvent* e)
{
    lua_getglobal(d_L, "OnWindowGotFocusEvent");

    if (!lua_isfunction(d_L, -1)) {
        lua_pop(d_L, -1);
        return;
    }
    
    Push(e->IsConsumed());

    int rc = lua_pcall(d_L, 1, 1, 0);
    PrintErrors(rc);

    if (lua_toboolean(d_L, -1)) { e->Consume(); }
}

void LuaEngine::CallOnWindowLostFocusEvent(WindowLostFocusEvent* e)
{
    lua_getglobal(d_L, "OnWindowLostFocusEvent");

    if (!lua_isfunction(d_L, -1)) {
        lua_pop(d_L, -1);
        return;
    }
    
    Push(e->IsConsumed());

    int rc = lua_pcall(d_L, 1, 1, 0);
    PrintErrors(rc);

    if (lua_toboolean(d_L, -1)) { e->Consume(); }
}

void LuaEngine::CallOnWindowMaximizeEvent(WindowMaximizeEvent* e)
{
    lua_getglobal(d_L, "OnWindowMaximizeEvent");

    if (!lua_isfunction(d_L, -1)) {
        lua_pop(d_L, -1);
        return;
    }
    
    Push(e->IsConsumed());

    int rc = lua_pcall(d_L, 1, 1, 0);
    PrintErrors(rc);

    if (lua_toboolean(d_L, -1)) { e->Consume(); }
}

void LuaEngine::CallOnWindowMinimizeEvent(WindowMinimizeEvent* e)
{
    lua_getglobal(d_L, "OnWindowMinimizeEvent");

    if (!lua_isfunction(d_L, -1)) {
        lua_pop(d_L, -1);
        return;
    }
    
    Push(e->IsConsumed());

    int rc = lua_pcall(d_L, 1, 1, 0);
    PrintErrors(rc);

    if (lua_toboolean(d_L, -1)) { e->Consume(); }
}

void LuaEngine::CallOnMouseButtonPressedEvent(MouseButtonPressedEvent* e)
{
    lua_getglobal(d_L, "OnMouseButtonPressedEvent");

    if (!lua_isfunction(d_L, -1)) {
        lua_pop(d_L, -1);
        return;
    }
    
    Push(e->IsConsumed());
    Push(e->Button());
    Push(e->Action());
    Push(e->Mods());

    int rc = lua_pcall(d_L, 4, 1, 0);
    PrintErrors(rc);

    if (lua_toboolean(d_L, -1)) { e->Consume(); }
}


void LuaEngine::CallOnMouseButtonReleasedEvent(MouseButtonReleasedEvent* e)
{
    lua_getglobal(d_L, "MouseButtonReleasedEvent");

    if (!lua_isfunction(d_L, -1)) {
        lua_pop(d_L, -1);
        return;
    }
    
    Push(e->IsConsumed());
    Push(e->Button());
    Push(e->Action());
    Push(e->Mods());

    int rc = lua_pcall(d_L, 4, 1, 0);
    PrintErrors(rc);

    if (lua_toboolean(d_L, -1)) { e->Consume(); }
}

void LuaEngine::CallOnMouseMovedEvent(MouseMovedEvent* e)
{
    lua_getglobal(d_L, "OnMouseMovedEvent");

    if (!lua_isfunction(d_L, -1)) {
        lua_pop(d_L, -1);
        return;
    }
    
    Push(e->IsConsumed());
    Push(e->XPos());
    Push(e->YPos());

    int rc = lua_pcall(d_L, 3, 1, 0);
    PrintErrors(rc);

    if (lua_toboolean(d_L, -1)) { e->Consume(); }
}

void LuaEngine::CallOnMouseScrolledEvent(MouseScrolledEvent* e)
{
    lua_getglobal(d_L, "OnMouseScrolledEvent");

    if (!lua_isfunction(d_L, -1)) {
        lua_pop(d_L, -1);
        return;
    }
    
    Push(e->IsConsumed());
    Push(e->XOffset());
    Push(e->YOffset());

    int rc = lua_pcall(d_L, 3, 1, 0);
    PrintErrors(rc);

    if (lua_toboolean(d_L, -1)) { e->Consume(); }
}

void LuaEngine::CallOnKeyboardButtonPressedEvent(KeyboardButtonPressedEvent* e)
{
    lua_getglobal(d_L, "OnKeyboardButtonPressedEvent");

    if (!lua_isfunction(d_L, -1)) {
        lua_pop(d_L, -1);
        return;
    }

    Push(e->IsConsumed());
    Push(e->Key());
    Push(e->Scancode());
    Push(e->Mods());

    int rc = lua_pcall(d_L, 4, 1, 0);
    PrintErrors(rc);

    if (lua_toboolean(d_L, -1)) { e->Consume(); }
}

void LuaEngine::CallOnKeyboardButtonReleasedEvent(KeyboardButtonReleasedEvent* e)
{
    lua_getglobal(d_L, "OnKeyboardButtonReleasedEvent");

    if (!lua_isfunction(d_L, -1)) {
        lua_pop(d_L, -1);
        return;
    }

    Push(e->IsConsumed());
    Push(e->Key());
    Push(e->Scancode());
    Push(e->Mods());

    int rc = lua_pcall(d_L, 4, 1, 0);
    PrintErrors(rc);

    if (lua_toboolean(d_L, -1)) { e->Consume(); }
}

void LuaEngine::CallOnKeyboardButtonHeldEvent(KeyboardButtonHeldEvent* e)
{
    lua_getglobal(d_L, "OnKeyboardButtonHeldEvent");

    if (!lua_isfunction(d_L, -1)) {
        lua_pop(d_L, -1);
        return;
    }

    Push(e->IsConsumed());
    Push(e->Key());
    Push(e->Scancode());
    Push(e->Mods());

    int rc = lua_pcall(d_L, 4, 1, 0);
    PrintErrors(rc);

    if (lua_toboolean(d_L, -1)) { e->Consume(); }
}

void LuaEngine::CallOnKeyboardKeyTypedEvent(KeyboardKeyTypedEvent* e)
{
    lua_getglobal(d_L, "OnKeyboardKeyTypedEvent");

    if (!lua_isfunction(d_L, -1)) {
        lua_pop(d_L, -1);
        return;
    }

    Push(e->IsConsumed());
    Push(e->Key());

    int rc = lua_pcall(d_L, 2, 1, 0);
    PrintErrors(rc);

    if (lua_toboolean(d_L, -1)) { e->Consume(); }
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