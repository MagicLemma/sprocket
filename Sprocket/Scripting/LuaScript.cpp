#include "LuaScript.h"
#include "LuaLibrary.h"
#include "Log.h"
#include "ECS.h"
#include "Components.h"

#include <lua.hpp>

namespace Sprocket {
namespace lua {
namespace {

void DoFile(lua_State* L, const char* file)
{
    if (luaL_dofile(L, file)) {
        log::error("[Lua]: Could not load {}", lua_tostring(L, -1));
    }
}

}

Script::Script(const std::string& file)
    : d_L(luaL_newstate(), [](lua_State* L) { lua_close(L); })
{
    lua_State* L = d_L.get();
    luaL_openlibs(L);

    // Core Sprocket Constants and Helper Functions
    DoFile(L, "Sprocket/Scripting/Sprocket_Base.lua");
    DoFile(L, "Sprocket/Scripting/Sprocket_Maths.lua");
    DoFile(L, "Sprocket/Scripting/Sprocket_Bindings.lua");
    DoFile(L, "Sprocket/Scripting/Sprocket_Components.lua");
    DoFile(L, "Sprocket/Scripting/Sprocket_Scene.lua");

    lua::register_scene_functions(L);
    lua::register_entity_transformation_functions(L);
    lua::register_entity_component_functions(L);
    lua::register_input_functions(L);

    DoFile(d_L.get(), file.c_str());
}

Script::Script()
    : d_L(luaL_newstate(), [](lua_State* L) { lua_close(L); })
{
    lua_State* L = d_L.get();
    luaL_openlibs(L);

    // Core Sprocket Constants and Helper Functions
    DoFile(L, "Sprocket/Scripting/Sprocket_Base.lua");
    DoFile(L, "Sprocket/Scripting/Sprocket_Maths.lua");
    DoFile(L, "Sprocket/Scripting/Sprocket_Bindings.lua");
    DoFile(L, "Sprocket/Scripting/Sprocket_Components.lua");
    DoFile(L, "Sprocket/Scripting/Sprocket_Scene.lua");

    lua::register_scene_functions(L);
    lua::register_entity_transformation_functions(L);
    lua::register_entity_component_functions(L);
    lua::register_input_functions(L);
}

void* Script::allocate(std::size_t size)
{
    return lua_newuserdata(d_L.get(), size);
}

void Script::push_value(bool val)
{
    lua_pushboolean(d_L.get(), val);
}

void Script::push_value(char val)
{
    lua_pushstring(d_L.get(), std::string(1, val).c_str());
}

void Script::push_value(int val)
{
    lua_pushinteger(d_L.get(), val);
}

void Script::push_value(float val)
{
    lua_pushnumber(d_L.get(), val);
}

void Script::push_value(double val)
{
    lua_pushnumber(d_L.get(), val);
}

void Script::push_value(const char* val)
{
    lua_pushstring(d_L.get(), val);
}

void Script::push_value(const std::string& val)
{
    lua_pushstring(d_L.get(), val.c_str());
}

void Script::push_value(void* val)
{
    lua_pushlightuserdata(d_L.get(), val);
}

void Script::print_errors(int rc) const
{
    if (rc == LUA_OK) { return; } // No error
    
    std::string err = lua_tostring(d_L.get(), -1);
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

void Script::on_event(ev::Event& event)
{
    const auto handler = [this, &event](const char* f, auto&&... args) {
        lua_getglobal(d_L.get(), f);
        if (!lua_isfunction(d_L.get(), -1)) {
            lua_pop(d_L.get(), -1);
            return;
        }

        push_value(event.is_consumed());
        (push_value(std::forward<decltype(args)>(args)), ...);

        print_errors(lua_pcall(d_L.get(), 1 + sizeof...(args), 1, 0));

        if (lua_toboolean(d_L.get(), -1)) {
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
    else if (auto x = event.get_if<ev::WindowClosed>()) {
        // pass
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

void Script::print_globals()
{
    log::info("Starting globals");
    lua_pushglobaltable(d_L.get());
    lua_pushnil(d_L.get());
    while (lua_next(d_L.get(), -2) != 0) {
        if (lua_isnumber(d_L.get(), -1)) {
            log::info("{} = {}", lua_tostring(d_L.get(), -2), lua_tonumber(d_L.get(), -1));
        }
        lua_pop(d_L.get(), 1);
    }
    lua_pop(d_L.get(), 1);
    log::info("Ending globals");
}

}
}