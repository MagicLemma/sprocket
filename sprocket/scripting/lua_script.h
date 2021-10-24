#pragma once
#include <sprocket/Scripting/lua_converter.h>

#include <lua.hpp>

#include <string>
#include <memory>
#include <cassert>

namespace spkt {
namespace lua {

class script
{
    std::unique_ptr<lua_State, void(*)(lua_State*)> d_L;

    void print_errors(int rc) const;

public:
    explicit script();
    explicit script(const std::string& file);

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
Return script::call_function(const std::string& function, Args&&... args)
{
    lua_State* L = d_L.get();

    lua_getglobal(L, function.c_str());
    assert(lua_isfunction(L, -1));

    (converter<std::decay_t<Args>>::push(L, args), ...);

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
        Return ret_val = lua::converter<Return>::read(L, -1);
        lua_pop(L, 1);
        return ret_val;
    }
}

template <typename Type>
void script::set_value(const std::string& name, Type&& value)
{
    lua_State* L = d_L.get();
    lua::converter<Type>::push(L, value);
    lua_setglobal(L, name.c_str());
}

}
}