#include "LuaEngine.h"
#include "LuaGlobals.h"
#include "LuaTransform.h"
#include "LuaInput.h"
#include "LuaCamera.h"
#include "Log.h"
#include "Entity.h"
#include "Components.h"

#include <lua.hpp>

namespace Sprocket {

namespace {

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
    luaL_dofile(d_L, "Resources/Scripts/Class.lua");
    luaL_dofile(d_L, "Resources/Scripts/Vec3.lua");
    luaL_dofile(d_L, "Resources/Scripts/Sprocket.lua");

    RegisterTransformFunctions(d_L);
    RegisterInputFunctions(d_L);

    lua_register(d_L, "HasCamera", &Lua_Has<CameraComponent>);
    RegisterCameraFunctions(d_L);

    lua_pushnil(d_L);
    lua_setglobal(d_L, "__keyboard__");

    lua_pushnil(d_L);
    lua_setglobal(d_L, "__mouse__");

    lua_pushnil(d_L);
    lua_setglobal(d_L, "__entity__");
}

LuaEngine::~LuaEngine()
{
    lua_close(d_L);
}

void LuaEngine::RunScript(const std::string& filename)
{
    luaL_dofile(d_L, filename.c_str());
}

void LuaEngine::CallInitFunction()
{
    SPKT_LOG_INFO("LuaEngine: Calling init!");

    lua_getglobal(d_L, "Init");
    
    if (!lua_isfunction(d_L, -1)) {
        lua_pop(d_L, -1);
        //SPKT_LOG_TRACE("[Lua]: Init not implemented for {}",
        //               GetEntity(d_L)->Name());
        return;
    }

    lua_pcall(d_L, 0, 0, 0);
}

void LuaEngine::CallOnUpdateFunction(double dt)
{
    if (!GetEntity(d_L)->Get<ScriptComponent>().active) {
        return;
    }

    lua_getglobal(d_L, "OnUpdate");
    
    if (!lua_isfunction(d_L, -1)) {
        lua_pop(d_L, -1);
        //SPKT_LOG_TRACE("[Lua]: OnUpdate not implemented for {}",
        //               GetEntity(d_L)->Name());
        return;
    }

    lua_pushnumber(d_L, dt);
    lua_pcall(d_L, 1, 0, 0);
}

void LuaEngine::CallOnWindowResizeEvent(WindowResizeEvent* e)
{
    if (!GetEntity(d_L)->Get<ScriptComponent>().active) {
        return;
    }

    lua_getglobal(d_L, "OnWindowResizeEvent");

    if (!lua_isfunction(d_L, -1)) {
        lua_pop(d_L, -1);
        //SPKT_LOG_TRACE("[Lua]: OnWindowResizeEvent not "
        //               "implemented for {}",
        //               GetEntity(d_L)->Name());
        return;
    }

    lua_pushboolean(d_L, e->IsConsumed());
    lua_pushnumber(d_L, (float)e->Width());
    lua_pushnumber(d_L, (float)e->Height());

    int rc = lua_pcall(d_L, 3, 1, 0);
    PrintErrors(d_L, rc);

    if (lua_toboolean(d_L, -1)) { e->Consume(); }
}

void LuaEngine::CallOnMouseButtonPressedEvent(MouseButtonPressedEvent* e)
{
    if (!GetEntity(d_L)->Get<ScriptComponent>().active) {
        return;
    }

    lua_getglobal(d_L, "OnMouseButtonPressedEvent");

    if (!lua_isfunction(d_L, -1)) {
        lua_pop(d_L, -1);
        //SPKT_LOG_TRACE("[Lua]: OnMouseButtonPressedEvent not "
        //               "implemented for {}",
        //               GetEntity(d_L)->Name());
    }
    
    lua_pushboolean(d_L, e->IsConsumed());
    lua_pushnumber(d_L, e->Button());
    lua_pushnumber(d_L, e->Action());
    lua_pushnumber(d_L, e->Mods());

    int rc = lua_pcall(d_L, 4, 1, 0);
    PrintErrors(d_L, rc);

    if (lua_toboolean(d_L, -1)) { e->Consume(); }
}

void LuaEngine::CallOnMouseScrolledEvent(MouseScrolledEvent* e)
{
    if (!GetEntity(d_L)->Get<ScriptComponent>().active) {
        return;
    }

    lua_getglobal(d_L, "OnMouseScrolledEvent");

    if (!lua_isfunction(d_L, -1)) {
        //SPKT_LOG_TRACE("[Lua]: OnMouseScrolledEvent not "
        //               "implemented for {}",
        //               GetEntity(d_L)->Name());
    }
    
    lua_pushboolean(d_L, e->IsConsumed());
    lua_pushnumber(d_L, e->XOffset());
    lua_pushnumber(d_L, e->YOffset());

    int rc = lua_pcall(d_L, 3, 1, 0);
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