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
    KeyboardProxy* k = (KeyboardProxy*)lua_touserdata(L, -1);
    lua_pop(L, 1);
    return k;
}

MouseProxy* GetMouse(lua_State* L)
{
    lua_getglobal(L, "Mouse");
    MouseProxy* m = (MouseProxy*)lua_touserdata(L, -1);
    lua_pop(L, 1);
    return m;
}

void SetEntity(lua_State* L, Entity* entity)
{
    lua_pushlightuserdata(L, (void*)entity);
    lua_setglobal(L, "Entity");
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

    int x = (int)lua_tointeger(L, 1);
    lua_pushboolean(L, GetKeyboard(L)->IsKeyDown(x));
    return 1;
}

int Lua_IsButtonDown(lua_State* L)
{
    if (!CheckLuaArgs(L, 1)) { return luaL_error(L, "Bad number of args"); }

    int x = (int)lua_tointeger(L, 1);
    lua_pushboolean(L, GetMouse(L)->IsButtonDown(x));
    return 1;
}

int Lua_GetMouseOffset(lua_State* L)
{
    if (!CheckLuaArgs(L, 0)) { return luaL_error(L, "Bad number of args"); }

    auto offset = GetMouse(L)->GetMouseOffset();
    lua_pushnumber(L, offset.x);
    lua_pushnumber(L, offset.y);
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

    lua_pushlightuserdata(d_L, (void*)&d_keyboard);
    lua_setglobal(d_L, "Keyboard");

    lua_pushlightuserdata(d_L, (void*)&d_mouse);
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

void LuaEngine::RunOnUpdateScript(double dt, Entity& entity)
{
    if (CheckLua(d_L, luaL_dofile(d_L, entity.Get<ScriptComponent>().script.c_str()))) {
        SetEntity(d_L, &entity);
        lua_getglobal(d_L, "OnUpdate");
        lua_pushnumber(d_L, dt);
        lua_pcall(d_L, 1, 0, 0);
        SetEntity(d_L, nullptr);
    } 
}

void LuaEngine::OnUpdate(double dt)
{
    d_mouse.OnUpdate();
}

void LuaEngine::OnEvent(Event& event)
{
    d_mouse.OnEvent(event);
    d_keyboard.OnEvent(event);
}

}