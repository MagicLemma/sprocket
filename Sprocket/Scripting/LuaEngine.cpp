#include "LuaEngine.h"
#include "Log.h"
#include "Entity.h"

namespace Sprocket {

namespace {

bool CheckLua(lua_State* L, int rc)
{
    if (rc != LUA_OK) {
        const char* error = lua_tostring(L, -1);
        SPKT_LOG_ERROR("[Lua]: {}", error);
        return false;
    }
    return true;
}

bool CheckLuaArgs(lua_State* L, int argc)
{
    int args = lua_gettop(L);
    if (args != argc) {
        SPKT_LOG_ERROR("[Lua]: Expected {} args, got {}", argc, args);
        return false;
    }
    return true;
}

KeyboardProxy* GetKeyboard(lua_State* L)
{
    lua_getglobal(L, "Keyboard");
    KeyboardProxy* k = nullptr;
    if (!lua_isnil(L, -1)) {
        k = (KeyboardProxy*)lua_touserdata(L, -1);
    }
    lua_pop(L, 1);
    return k;
}

MouseProxy* GetMouse(lua_State* L)
{
    lua_getglobal(L, "Mouse");
    MouseProxy* m = nullptr;
    if (!lua_isnil(L, -1)) {
        m = (MouseProxy*)lua_touserdata(L, -1);
    }
    lua_pop(L, 1);
    return m;
}

Entity* GetEntity(lua_State* L)
{
    lua_getglobal(L, "Entity");
    Entity* e = (Entity*)lua_touserdata(L, -1);
    lua_pop(L, 1);
    return e;
}

int Lua_GetPosition(lua_State* L)
{
    if (!CheckLuaArgs(L, 0)) { return luaL_error(L, "Bad number of args"); }

    Entity* entity = GetEntity(L);
    lua_pushnumber(L, entity->Position().x);
    lua_pushnumber(L, entity->Position().y);
    lua_pushnumber(L, entity->Position().z);
    return 3;
}

int Lua_SetPosition(lua_State* L)
{
    if (!CheckLuaArgs(L, 3)) { return luaL_error(L, "Bad number of args"); }

    Entity* entity = GetEntity(L);
    float x = (float)lua_tonumber(L, 1);
    float y = (float)lua_tonumber(L, 2);
    float z = (float)lua_tonumber(L, 3);
    entity->SetPosition({x, y, z});
    return 0;
}

int Lua_RotateY(lua_State* L)
{
    if (!CheckLuaArgs(L, 1)) { return luaL_error(L, "Bad number of args"); };

    Entity* entity = GetEntity(L);
    float yaw = (float)lua_tonumber(L, 1);
    auto orientation = entity->Orientation();
    orientation = Maths::Rotate(orientation, {0, 1, 0}, Maths::Radians(yaw));
    entity->Orientation() = orientation;
    return 0;
}

template<typename T> int Lua_Has(lua_State* L)
{
    if (!CheckLuaArgs(L, 0)) { return luaL_error(L, "Bad number of args"); }

    Entity* entity = GetEntity(L);
    lua_pushboolean(L, entity->Has<T>());
    return 0;
}

int Lua_IsKeyDown(lua_State* L)
{
    if (!CheckLuaArgs(L, 1)) { return luaL_error(L, "Bad number of args"); }

    auto k = GetKeyboard(L);
    if (k == nullptr) {
        lua_pushboolean(L, false);
    }
    else {
        int x = (int)lua_tointeger(L, 1);
        lua_pushboolean(L, k->IsKeyDown(x));
    }
    
    return 1;
}

int Lua_IsButtonDown(lua_State* L)
{
    if (!CheckLuaArgs(L, 1)) { return luaL_error(L, "Bad number of args"); }

    auto m = GetMouse(L);
    if (m == nullptr) {
        lua_pushboolean(L, false);
    }
    else {
        int x = (int)lua_tointeger(L, 1);
        lua_pushboolean(L, m->IsButtonDown(x));
    }

    return 1;
}

int Lua_GetMouseOffset(lua_State* L)
{
    if (!CheckLuaArgs(L, 0)) { return luaL_error(L, "Bad number of args"); }

    auto m = GetMouse(L);
    if (m == nullptr) {
        lua_pushnumber(L, 0);
        lua_pushnumber(L, 0);
    }
    else {
        auto offset = m->GetMouseOffset();
        lua_pushnumber(L, offset.x);
        lua_pushnumber(L, offset.y);
    }
    return 2;
}

int Lua_GetForwardsDir(lua_State* L)
{
    if (!CheckLuaArgs(L, 0)) { return luaL_error(L, "Bad number of args"); }

    auto forwards = Maths::Forwards(GetEntity(L)->Orientation());
    lua_pushnumber(L, forwards.x);
    lua_pushnumber(L, forwards.y);
    lua_pushnumber(L, forwards.z);
    return 3;
}

int Lua_GetRightDir(lua_State* L)
{
    if (!CheckLuaArgs(L, 0)) { return luaL_error(L, "Bad number of args"); }

    auto right = Maths::Right(GetEntity(L)->Orientation());
    lua_pushnumber(L, right.x);
    lua_pushnumber(L, right.y);
    lua_pushnumber(L, right.z);
    return 3;
}

int Lua_GetPitch(lua_State* L)
{
    if (!CheckLuaArgs(L, 0)) { return luaL_error(L, "Bad number of args"); }

    auto c = GetEntity(L)->Get<CameraComponent>();
    lua_pushnumber(L, c.pitch);
    return 1;
}

int Lua_SetPitch(lua_State* L)
{
    if (!CheckLuaArgs(L, 1)) { return luaL_error(L, "Bad number of args"); }

    float x = (float)lua_tonumber(L, 1);
    auto& c = GetEntity(L)->Get<CameraComponent>();
    c.pitch = x;
    return 0;
}

}

LuaEngine::LuaEngine()
    : d_L(luaL_newstate())
{
    luaL_openlibs(d_L);
    luaL_dofile(d_L, "Resources/Scripts/Sprocket.lua");

    lua_register(d_L, "GetPosition", &Lua_GetPosition);
    lua_register(d_L, "SetPosition", &Lua_SetPosition);
    lua_register(d_L, "RotateY", &Lua_RotateY);
    lua_register(d_L, "IsKeyDown", &Lua_IsKeyDown);
    lua_register(d_L, "IsButtonDown", &Lua_IsButtonDown);
    lua_register(d_L, "GetMouseOffset", &Lua_GetMouseOffset);

    lua_register(d_L, "GetForwardsDir", &Lua_GetForwardsDir);
    lua_register(d_L, "GetRightDir", &Lua_GetRightDir);

    lua_register(d_L, "HasCamera", &Lua_Has<CameraComponent>);
    lua_register(d_L, "GetPitch", &Lua_GetPitch);
    lua_register(d_L, "SetPitch", &Lua_SetPitch);

    lua_pushnil(d_L);
    lua_setglobal(d_L, "Keyboard");

    lua_pushnil(d_L);
    lua_setglobal(d_L, "Mouse");
}

LuaEngine::~LuaEngine()
{
    lua_close(d_L);
}

void LuaEngine::RunScript(const std::string& filename)
{
    luaL_dofile(d_L, filename.c_str());
}

void LuaEngine::CallOnUpdateFunction(double dt)
{
    lua_getglobal(d_L, "OnUpdate");
    // TODO: Check that the OnUpdate function exists.
    lua_pushnumber(d_L, dt);
    lua_pcall(d_L, 1, 0, 0);
}

void LuaEngine::CallOnMouseButtonPressedEvent(MouseButtonPressedEvent* e)
{
    lua_getglobal(d_L, "OnMouseButtonPressedEvent");
    // TODO: Check that the OnMouseButtonPressedEvent function exists.
    lua_pushboolean(d_L, e->IsConsumed());
    lua_pushnumber(d_L, e->Button());
    lua_pushnumber(d_L, e->Action());
    lua_pushnumber(d_L, e->Mods());
    lua_pcall(d_L, 4, 1, 0);
    if (lua_toboolean(d_L, -1)) { e->Consume(); }
}

void LuaEngine::SetEntity(const Entity& e)
{
    lua_pushlightuserdata(d_L, (void*)&e);
    lua_setglobal(d_L, "Entity");
}

void LuaEngine::SetKeyboard(KeyboardProxy* k)
{
    lua_pushlightuserdata(d_L, (void*)k);
    lua_setglobal(d_L, "Keyboard");
}

void LuaEngine::SetMouse(MouseProxy* m)
{
    lua_pushlightuserdata(d_L, (void*)m);
    lua_setglobal(d_L, "Mouse");
}

}