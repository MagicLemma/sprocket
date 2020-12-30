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

    lua_register(L, "FastIteration_New", &Lua::NewFast);
    lua_register(L, "FastIteration_Delete", &Lua::DeleteFast);
    lua_register(L, "FastIteration_IterStart", &Lua::IterStart);
    lua_register(L, "FastIteration_IterValid", &Lua::IterValid);
    lua_register(L, "FastIteration_IterNext", &Lua::IterNext);
    lua_register(L, "FastIteration_IterGet", &Lua::IterGet);
}

namespace Lua {
namespace {

using generator_t = cppcoro::generator<ECS::Entity>;
using iterator_t = typename generator_t::iterator;

}

int NewEntity(lua_State* L)
{
    if (!CheckArgCount(L, 0)) { return luaL_error(L, "Bad number of args"); }
    ECS::Entity* luaEntity = (ECS::Entity*)lua_newuserdata(L, sizeof(ECS::Entity));
    *luaEntity = GetScene(L)->Entities().New();
    return 1;
}

int DeleteEntity(lua_State* L)
{
    if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }
    ECS::Entity luaEntity = *static_cast<ECS::Entity*>(lua_touserdata(L, 1));
    luaEntity.Delete();
    return 0;
}

int NewFast(lua_State* L)
{
    if (!CheckArgCount(L, 0)) { return luaL_error(L, "Bad number of args"); }
    auto gen = new generator_t(GetScene(L)->Entities().Fast());
    lua_pushlightuserdata(L, static_cast<void*>(gen));
    return 1;
}

int DeleteFast(lua_State* L)
{
    if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }
    delete static_cast<generator_t*>(lua_touserdata(L, 1));
    return 0;
}

int IterStart(lua_State* L)
{
    if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }
    generator_t* gen = (generator_t*)lua_touserdata(L, 1);

    iterator_t* iter = (iterator_t*)lua_newuserdata(L, sizeof(iterator_t));
    *iter = gen->begin();
    return 1;
}

int IterValid(lua_State* L)
{
    if (!CheckArgCount(L, 2)) { return luaL_error(L, "Bad number of args"); }
    generator_t* gen = (generator_t*)lua_touserdata(L, 1);
    iterator_t* iter = (iterator_t*)lua_touserdata(L, 2);

    lua_pushboolean(L, *iter != gen->end());
    return 1;
}

int IterNext(lua_State* L)
{
    if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }
    iterator_t* iter = (iterator_t*)lua_touserdata(L, 1);
    ++(*iter);
    return 0;   
}

int IterGet(lua_State* L)
{
    if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }
    iterator_t* iterator = (iterator_t*)lua_touserdata(L, 1);

    ECS::Entity* luaEntity = (ECS::Entity*)lua_newuserdata(L, sizeof(ECS::Entity));
    *luaEntity = **iterator;
    return 1;
}

}
}