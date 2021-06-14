#pragma once
#include "LuaConverter.h"
#include "Log.h"

#include <string>
#include <memory>

#include <lua.hpp>
#include <cassert>

namespace Sprocket {
namespace lua {

class Script
{
    std::unique_ptr<lua_State, void(*)(lua_State*)> d_L;

    void print_errors(int rc) const;

public:
    explicit Script();
    explicit Script(const std::string& file);

    // TODO: Remove
    void print_globals();

    bool has_function(const std::string& function);

    template <typename Return, typename... Args>
    Return call_function(const std::string& function, Args&&... args);

    template <typename Type>
    void set_value(const std::string& name, Type&& value);

    lua_State* native_handle() const { return d_L.get(); }
};

template <typename Return, typename... Args>
Return Script::call_function(const std::string& function, Args&&... args)
{
    lua_State* L = d_L.get();

    lua_getglobal(L, function.c_str());
    assert(lua_isfunction(L, -1));

    (Converter<std::decay_t<Args>>::push(L, args), ...);

    if constexpr (std::is_void_v<Return>) {
        int rc = lua_pcall(L, sizeof...(Args), 0, 0);
        print_errors(rc);
    }
    else {
        int rc = lua_pcall(L, sizeof...(Args), 1, 0);
        print_errors(rc);
        if (rc != LUA_OK) {
            return {};
        }
        Return ret_val = lua::Converter<Return>::read(L, -1);
        lua_pop(L, 1);
        return ret_val;
    }
}

template <typename Type>
void Script::set_value(const std::string& name, Type&& value)
{
    lua_State* L = d_L.get();
    lua::Converter<Type>::push(L, value);
    lua_setglobal(L, name.c_str());
}

}
}