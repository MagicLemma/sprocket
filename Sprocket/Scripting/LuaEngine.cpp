#include "LuaEngine.h"
#include "LuaGlobals.h"
#include "LuaEntity.h"
#include "LuaTransform.h"
#include "LuaInput.h"
#include "LuaComponents.h"
#include "Log.h"
#include "Entity.h"
#include "Components.h"

#include <lua.hpp>

namespace Sprocket {

namespace {

void DoFile(lua_State* L, const char* file)
{
    if (luaL_dofile(L, file)) {
        SPKT_LOG_ERROR("[Lua]: Could not load {}", lua_tostring(L, -1));
    }
}

void PrintErrors(lua_State* L, int rc)
{
    Entity* e = GetEntity(L);

    std::string name = "Unnamed";
    if (e->Has<NameComponent>()) {
        name = e->Get<NameComponent>().name;
    }

    if (rc == LUA_ERRRUN) {
        std::string err = lua_tostring(L, -1);
        SPKT_LOG_ERROR("[Lua]: Runtime error for {}: {}", name, err);
    }
    else if (rc == LUA_ERRMEM) {
        std::string err = lua_tostring(L, -1);
        SPKT_LOG_ERROR("[Lua]: Memory allocation error for {}: {}", name, err);
    }
    else if (rc == LUA_ERRERR) {
        std::string err = lua_tostring(L, -1);
        SPKT_LOG_ERROR("[Lua]: Error handler func failed for {}: {}", name, err);
    }
    else if (rc != LUA_OK) {
        std::string err = lua_tostring(L, -1);
        SPKT_LOG_ERROR("[Lua]: Unknown error for {}: {}", name, err);
    }
}

template<typename T> int Lua_Has(lua_State* L)
{
    if (!CheckArgCount(L, 0)) { return luaL_error(L, "Bad number of args"); }

    Entity* entity = GetEntity(L);
    lua_pushboolean(L, entity->Has<T>());
    return 0;
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

    RegisterTransformFunctions(d_L);
    RegisterInputFunctions(d_L);

    lua_register(d_L, "HasCamera", &Lua_Has<CameraComponent>);

    RegisterComponentFunctions(d_L);
    RegisterEntityFunctions(d_L);

    lua_pushnil(d_L);
    lua_setglobal(d_L, "__keyboard__");

    lua_pushnil(d_L);
    lua_setglobal(d_L, "__mouse__");

    lua_pushnil(d_L);
    lua_setglobal(d_L, "__entity__");

    lua_pushlightuserdata(d_L, &d_spawnedEntities);
    lua_setglobal(d_L, "__spawned_entities__");
}

LuaEngine::~LuaEngine()
{
    lua_close(d_L);
}

void LuaEngine::RunScript(const std::string& filename)
{
    if (filename.empty()) {
        SPKT_LOG_WARN("Tried to start an empty script!");
        return;
    }
    
    DoFile(d_L, filename.c_str());
}

void LuaEngine::CallInitFunction()
{
    SPKT_LOG_INFO("LuaEngine: Calling init!");

    lua_getglobal(d_L, "Init");
    
    if (!lua_isfunction(d_L, -1)) {
        lua_pop(d_L, -1);
        return;
    }

    lua_pushlightuserdata(d_L, &d_entity);

    int rc = lua_pcall(d_L, 1, 0, 0);
    PrintErrors(d_L, rc);
}

void LuaEngine::CallOnUpdateFunction(double dt)
{
    if (!d_entity.Get<ScriptComponent>().active) { return; }
    lua_getglobal(d_L, "OnUpdate");
    
    if (!lua_isfunction(d_L, -1)) {
        lua_pop(d_L, -1);
        return;
    }

    lua_pushlightuserdata(d_L, &d_entity);
    lua_pushnumber(d_L, dt);

    int rc = lua_pcall(d_L, 2, 0, 0);
    PrintErrors(d_L, rc);
}

void LuaEngine::CallOnWindowResizeEvent(WindowResizeEvent* e)
{
    if (!d_entity.Get<ScriptComponent>().active) { return; }
    lua_getglobal(d_L, "OnWindowResizeEvent");

    if (!lua_isfunction(d_L, -1)) {
        lua_pop(d_L, -1);
        return;
    }

    lua_pushboolean(d_L, e->IsConsumed());
    lua_pushnumber(d_L, (float)e->Width());
    lua_pushnumber(d_L, (float)e->Height());

    int rc = lua_pcall(d_L, 3, 1, 0);
    PrintErrors(d_L, rc);

    if (lua_toboolean(d_L, -1)) { e->Consume(); }
}

void LuaEngine::CallOnWindowClosedEvent(WindowClosedEvent* e)
{}

void LuaEngine::CallOnWindowGotFocusEvent(WindowGotFocusEvent* e)
{
    if (!d_entity.Get<ScriptComponent>().active) { return; }
    lua_getglobal(d_L, "OnWindowGotFocusEvent");

    if (!lua_isfunction(d_L, -1)) {
        lua_pop(d_L, -1);
        return;
    }
    
    lua_pushboolean(d_L, e->IsConsumed());

    int rc = lua_pcall(d_L, 1, 1, 0);
    PrintErrors(d_L, rc);

    if (lua_toboolean(d_L, -1)) { e->Consume(); }
}

void LuaEngine::CallOnWindowLostFocusEvent(WindowLostFocusEvent* e)
{
    if (!d_entity.Get<ScriptComponent>().active) { return; }
    lua_getglobal(d_L, "OnWindowLostFocusEvent");

    if (!lua_isfunction(d_L, -1)) {
        lua_pop(d_L, -1);
        return;
    }
    
    lua_pushboolean(d_L, e->IsConsumed());

    int rc = lua_pcall(d_L, 1, 1, 0);
    PrintErrors(d_L, rc);

    if (lua_toboolean(d_L, -1)) { e->Consume(); }
}

void LuaEngine::CallOnWindowMaximizeEvent(WindowMaximizeEvent* e)
{
    if (!d_entity.Get<ScriptComponent>().active) { return; }
    lua_getglobal(d_L, "OnWindowMaximizeEvent");

    if (!lua_isfunction(d_L, -1)) {
        lua_pop(d_L, -1);
        return;
    }
    
    lua_pushboolean(d_L, e->IsConsumed());

    int rc = lua_pcall(d_L, 1, 1, 0);
    PrintErrors(d_L, rc);

    if (lua_toboolean(d_L, -1)) { e->Consume(); }
}

void LuaEngine::CallOnWindowMinimizeEvent(WindowMinimizeEvent* e)
{
        if (!d_entity.Get<ScriptComponent>().active) { return; }
    lua_getglobal(d_L, "OnWindowMinimizeEvent");

    if (!lua_isfunction(d_L, -1)) {
        lua_pop(d_L, -1);
        return;
    }
    
    lua_pushboolean(d_L, e->IsConsumed());

    int rc = lua_pcall(d_L, 1, 1, 0);
    PrintErrors(d_L, rc);

    if (lua_toboolean(d_L, -1)) { e->Consume(); }
}

void LuaEngine::CallOnMouseButtonPressedEvent(MouseButtonPressedEvent* e)
{
    if (!d_entity.Get<ScriptComponent>().active) { return; }
    lua_getglobal(d_L, "OnMouseButtonPressedEvent");

    if (!lua_isfunction(d_L, -1)) {
        lua_pop(d_L, -1);
        return;
    }
    
    lua_pushboolean(d_L, e->IsConsumed());
    lua_pushnumber(d_L, e->Button());
    lua_pushnumber(d_L, e->Action());
    lua_pushnumber(d_L, e->Mods());

    int rc = lua_pcall(d_L, 4, 1, 0);
    PrintErrors(d_L, rc);

    if (lua_toboolean(d_L, -1)) { e->Consume(); }
}


void LuaEngine::CallOnMouseButtonReleasedEvent(MouseButtonReleasedEvent* e)
{
    if (!d_entity.Get<ScriptComponent>().active) { return; }
    lua_getglobal(d_L, "MouseButtonReleasedEvent");

    if (!lua_isfunction(d_L, -1)) {
        lua_pop(d_L, -1);
        return;
    }
    
    lua_pushboolean(d_L, e->IsConsumed());
    lua_pushnumber(d_L, e->Button());
    lua_pushnumber(d_L, e->Action());
    lua_pushnumber(d_L, e->Mods());

    int rc = lua_pcall(d_L, 4, 1, 0);
    PrintErrors(d_L, rc);

    if (lua_toboolean(d_L, -1)) { e->Consume(); }
}

void LuaEngine::CallOnMouseMovedEvent(MouseMovedEvent* e)
{
    if (!d_entity.Get<ScriptComponent>().active) { return; }
    lua_getglobal(d_L, "OnMouseMovedEvent");

    if (!lua_isfunction(d_L, -1)) {
        lua_pop(d_L, -1);
        return;
    }
    
    lua_pushboolean(d_L, e->IsConsumed());
    lua_pushnumber(d_L, e->XPos());
    lua_pushnumber(d_L, e->YPos());

    int rc = lua_pcall(d_L, 3, 1, 0);
    PrintErrors(d_L, rc);

    if (lua_toboolean(d_L, -1)) { e->Consume(); }
}

void LuaEngine::CallOnMouseScrolledEvent(MouseScrolledEvent* e)
{
    if (!d_entity.Get<ScriptComponent>().active) { return; }
    lua_getglobal(d_L, "OnMouseScrolledEvent");

    if (!lua_isfunction(d_L, -1)) {
        lua_pop(d_L, -1);
        return;
    }
    
    lua_pushboolean(d_L, e->IsConsumed());
    lua_pushnumber(d_L, e->XOffset());
    lua_pushnumber(d_L, e->YOffset());

    int rc = lua_pcall(d_L, 3, 1, 0);
    PrintErrors(d_L, rc);

    if (lua_toboolean(d_L, -1)) { e->Consume(); }
}

void LuaEngine::CallOnKeyboardButtonPressedEvent(KeyboardButtonPressedEvent* e)
{
    if (!d_entity.Get<ScriptComponent>().active) { return; }
    lua_getglobal(d_L, "OnKeyboardButtonPressedEvent");

    if (!lua_isfunction(d_L, -1)) {
        lua_pop(d_L, -1);
        return;
    }

    lua_pushboolean(d_L, e->IsConsumed());
    lua_pushnumber(d_L, e->Key());
    lua_pushnumber(d_L, e->Scancode());
    lua_pushnumber(d_L, e->Mods());

    int rc = lua_pcall(d_L, 4, 1, 0);
    PrintErrors(d_L, rc);

    if (lua_toboolean(d_L, -1)) { e->Consume(); }
}

void LuaEngine::CallOnKeyboardButtonReleasedEvent(KeyboardButtonReleasedEvent* e)
{
    if (!d_entity.Get<ScriptComponent>().active) { return; }
    lua_getglobal(d_L, "OnKeyboardButtonReleasedEvent");

    if (!lua_isfunction(d_L, -1)) {
        lua_pop(d_L, -1);
        return;
    }

    lua_pushboolean(d_L, e->IsConsumed());
    lua_pushnumber(d_L, e->Key());
    lua_pushnumber(d_L, e->Scancode());
    lua_pushnumber(d_L, e->Mods());

    int rc = lua_pcall(d_L, 4, 1, 0);
    PrintErrors(d_L, rc);

    if (lua_toboolean(d_L, -1)) { e->Consume(); }
}

void LuaEngine::CallOnKeyboardButtonHeldEvent(KeyboardButtonHeldEvent* e)
{
    if (!d_entity.Get<ScriptComponent>().active) { return; }
    lua_getglobal(d_L, "OnKeyboardButtonHeldEvent");

    if (!lua_isfunction(d_L, -1)) {
        lua_pop(d_L, -1);
        return;
    }

    lua_pushboolean(d_L, e->IsConsumed());
    lua_pushnumber(d_L, e->Key());
    lua_pushnumber(d_L, e->Scancode());
    lua_pushnumber(d_L, e->Mods());

    int rc = lua_pcall(d_L, 4, 1, 0);
    PrintErrors(d_L, rc);

    if (lua_toboolean(d_L, -1)) { e->Consume(); }
}

void LuaEngine::CallOnKeyboardKeyTypedEvent(KeyboardKeyTypedEvent* e)
{
    if (!d_entity.Get<ScriptComponent>().active) { return; }
    lua_getglobal(d_L, "OnKeyboardKeyTypedEvent");

    if (!lua_isfunction(d_L, -1)) {
        lua_pop(d_L, -1);
        return;
    }

    lua_pushboolean(d_L, e->IsConsumed());
    lua_pushnumber(d_L, e->Key());

    int rc = lua_pcall(d_L, 2, 1, 0);
    PrintErrors(d_L, rc);

    if (lua_toboolean(d_L, -1)) { e->Consume(); }
}

void LuaEngine::SetEntity(const Entity& e)
{
    d_entity = e;
    lua_pushlightuserdata(d_L, (void*)&d_entity);
    lua_setglobal(d_L, "__entity__");
}

void LuaEngine::SetKeyboard(KeyboardProxy* k)
{
    lua_pushlightuserdata(d_L, (void*)k);
    lua_setglobal(d_L, "__keyboard__");
}

void LuaEngine::SetMouse(MouseProxy* m)
{
    lua_pushlightuserdata(d_L, (void*)m);
    lua_setglobal(d_L, "__mouse__");
}

}