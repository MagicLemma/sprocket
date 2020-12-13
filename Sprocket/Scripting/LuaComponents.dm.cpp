#include "LuaComponents.h"
#include "LuaGlobals.h"
#include "ECS.h"
#include "Maths.h"
#include "Components.h"
#include "Log.h"

#include <lua.hpp>
#include <cassert>

namespace Sprocket {
namespace {

// PUSH
int Push(lua_State* L, int value)
{
    lua_pushnumber(L, value);
    return 1;
}

int Push(lua_State* L, float value)
{
    lua_pushnumber(L, value);
    return 1;
}

int Push(lua_State* L, const std::string& value)
{
    lua_pushstring(L, value.c_str());
    return 1;
}

int Push(lua_State* L, const bool& value)
{
    lua_pushboolean(L, value);
    return 1;
}

int Push(lua_State* L, const glm::vec3& value)
{
    lua_pushnumber(L, value.x);
    lua_pushnumber(L, value.y);
    lua_pushnumber(L, value.z);
    return 3;
}

// PULL
template <typename T> T Pull(lua_State* L, int& count)
{
    static_assert(sizeof(T) == -1);
    return T();
}

template <> int Pull(lua_State* L, int& count)
{
    return (int)lua_tonumber(L, count++);
}

template <> float Pull(lua_State* L, int& count)
{
    return (float)lua_tonumber(L, count++);
}

template <> std::string Pull(lua_State* L, int& count)
{
    return std::string(lua_tostring(L, count++));
}

template <> bool Pull(lua_State* L, int& count)
{
    return (bool)lua_toboolean(L, count++);
}

template <> glm::vec3 Pull(lua_State* L, int& count)
{
    float x = (float)lua_tonumber(L, count++);
    float y = (float)lua_tonumber(L, count++);
    float z = (float)lua_tonumber(L, count++);
    return {x, y, z};
}

template <> glm::quat Pull(lua_State* L, int& count)
{
    float x = (float)lua_tonumber(L, count++);
    float y = (float)lua_tonumber(L, count++);
    float z = (float)lua_tonumber(L, count++);
    float w = (float)lua_tonumber(L, count++);
    return {x, y, z, w};
}

// DIMENSION
template <typename T> constexpr int Dimension()
{
    static_assert(sizeof(T) == -1);
    return 0;
}

template <> constexpr int Dimension<int>() { return 1; }
template <> constexpr int Dimension<float>() { return 1; }
template <> constexpr int Dimension<bool>() { return 1; }
template <> constexpr int Dimension<std::string>() { return 1; }
template <> constexpr int Dimension<glm::vec3>() { return 3; }
template <> constexpr int Dimension<glm::quat>() { return 4; }

#ifdef DATAMATIC_BLOCK SCRIPTABLE=true
constexpr int {{Comp.Name}}Dimension()
{
    int count = 0;
    count += Dimension<{{Attr.Type}}>(); // {{Attr.Name}}
    return count;
} 

#endif

template<typename T> int Lua_Has(lua_State* L)
{
    if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }

    Entity entity = *static_cast<Entity*>(lua_touserdata(L, 1));
    lua_pushboolean(L, entity.Has<T>());
    return 1;
}

}

void RegisterComponentFunctions(lua_State* L)
{
#ifdef DATAMATIC_BLOCK SCRIPTABLE=true
    lua_register(L, "Lua_Get{{Comp.Name}}", &Lua::Get{{Comp.Name}});
    lua_register(L, "Lua_Set{{Comp.Name}}", &Lua::Set{{Comp.Name}});
    lua_register(L, "Lua_Add{{Comp.Name}}", &Lua::Add{{Comp.Name}});
    lua_register(L, "Has{{Comp.Name}}", &Lua_Has<{{Comp.Name}}>);

#endif
}

namespace Lua {

#ifdef DATAMATIC_BLOCK SCRIPTABLE=true
int Get{{Comp.Name}}(lua_State* L)
{
    if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }

    Entity e = *static_cast<Entity*>(lua_touserdata(L, 1));
    assert(e.Has<{{Comp.Name}}>());

    int count = 0;
    const auto& c = e.Get<{{Comp.Name}}>();
    count += Push(L, c.{{Attr.Name}});
    return count;
}

int Set{{Comp.Name}}(lua_State* L)
{
    if (!CheckArgCount(L, {{Comp.Name}}Dimension() + 1)) { return luaL_error(L, "Bad number of args"); }

    int count = 2;
    Entity e = *static_cast<Entity*>(lua_touserdata(L, 1));
    auto& c = e.Get<{{Comp.Name}}>();
    c.{{Attr.Name}} = Pull<{{Attr.Type}}>(L, count);
    return 0;
}

int Add{{Comp.Name}}(lua_State* L)
{
    if (!CheckArgCount(L, {{Comp.Name}}Dimension() + 1)) { return luaL_error(L, "Bad number of args"); }

    int count = 2;
    Entity e = *static_cast<Entity*>(lua_touserdata(L, 1));
    assert(!e.Has<{{Comp.Name}}>());

    {{Comp.Name}} c;
    c.{{Attr.Name}} = Pull<{{Attr.Type}}>(L, count);
    e.Add(c);
    return 0;
}

#endif

}
}
