#include "LuaEntity.h"
#include "LuaGlobals.h"
#include "ECS.h"
#include "Scene.h"

#include <lua.hpp>

namespace Sprocket {

void RegisterEntityFunctions(lua_State* L)
{
    lua_register(L, "NewEntity", &Lua::NewEntity);
    lua_register(L, "DeleteEntity", &Lua::DeleteEntity);

    lua_register(L, "Each_New", &Lua::Each_New);
    lua_register(L, "Each_Delete", &Lua::Each_Delete);
    lua_register(L, "Each_Iter_Start", &Lua::Each_Iter_Start);
    lua_register(L, "Each_Iter_Valid", &Lua::Each_Iter_Valid);
    lua_register(L, "Each_Iter_Next", &Lua::Each_Iter_Next);
    lua_register(L, "Each_Iter_Get", &Lua::Each_Iter_Get);
}

namespace Lua {
namespace {

using Generator = cppcoro::generator<ecs::Entity>;
using Iterator = typename Generator::iterator;

}

int NewEntity(lua_State* L)
{
    if (!CheckArgCount(L, 0)) { return luaL_error(L, "Bad number of args"); }
    auto luaEntity = static_cast<ecs::Entity*>(lua_newuserdata(L, sizeof(ecs::Entity)));
    *luaEntity = GetScene(L)->Entities().New();
    return 1;
}

int DeleteEntity(lua_State* L)
{
    if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }
    auto luaEntity = static_cast<ecs::Entity*>(lua_touserdata(L, 1));
    luaEntity->Delete();
    return 0;
}

int Each_New(lua_State* L)
{
    if (!CheckArgCount(L, 0)) { return luaL_error(L, "Bad number of args"); }
    auto gen = new Generator(GetScene(L)->Entities().Each());
    lua_pushlightuserdata(L, static_cast<void*>(gen));
    return 1;
}

int Each_Delete(lua_State* L)
{
    if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }
    delete static_cast<Generator*>(lua_touserdata(L, 1));
    return 0;
}

int Each_Iter_Start(lua_State* L)
{
    if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }
    auto gen = static_cast<Generator*>(lua_touserdata(L, 1));

    auto iter = static_cast<Iterator*>(lua_newuserdata(L, sizeof(Iterator)));
    *iter = gen->begin();
    return 1;
}

int Each_Iter_Valid(lua_State* L)
{
    if (!CheckArgCount(L, 2)) { return luaL_error(L, "Bad number of args"); }
    auto gen = static_cast<Generator*>(lua_touserdata(L, 1));
    auto iter = static_cast<Iterator*>(lua_touserdata(L, 2));

    lua_pushboolean(L, *iter != gen->end());
    return 1;
}

int Each_Iter_Next(lua_State* L)
{
    if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }
    auto iter = static_cast<Iterator*>(lua_touserdata(L, 1));
    ++(*iter);
    return 0;   
}

int Each_Iter_Get(lua_State* L)
{
    if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }
    auto iterator = static_cast<Iterator*>(lua_touserdata(L, 1));

    auto luaEntity = static_cast<ecs::Entity*>(lua_newuserdata(L, sizeof(ecs::Entity)));
    *luaEntity = **iterator;
    return 1;
}

}
}