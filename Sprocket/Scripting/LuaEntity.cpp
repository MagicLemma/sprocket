#include "LuaEntity.h"
#include "LuaGlobals.h"
#include "ECS.h"
#include "Scene.h"

#include <lua.hpp>

namespace Sprocket {

void RegisterEntityFunctions(lua_State* L)
{
    lua_register(L, "NewEntity", &Lua::NewEntity);
    lua_register(L, "FastBegin", &Lua::FastBegin);
    lua_register(L, "IteratorAtEnd", &Lua::IteratorAtEnd);
    lua_register(L, "GetEntityFromIterator", &Lua::GetEntityFromIterator);
    lua_register(L, "IteratorAdvance", &Lua::IteratorAdvance);
    lua_register(L, "FastBeginFree", &Lua::FastBeginFree);
}

namespace Lua {

int NewEntity(lua_State* L)
{
    if (!CheckArgCount(L, 0)) { return luaL_error(L, "Bad number of args"); }
    ECS::Entity* luaEntity = (ECS::Entity*)lua_newuserdata(L, sizeof(ECS::Entity));
    *luaEntity = GetScene(L)->Entities().New();
    return 1;
}

int FastBegin(lua_State* L)
{
    if (!CheckArgCount(L, 0)) { return luaL_error(L, "Bad number of args"); }
    using generator_type = cppcoro::generator<ECS::Entity>;
    using iterator = typename generator_type::iterator;

    generator_type** gen = (generator_type**)lua_newuserdata(L, sizeof(generator_type*));
    iterator* iter = (iterator*)lua_newuserdata(L, sizeof(iterator));

    *gen = new generator_type(GetScene(L)->Entities().Fast());
    *iter = (*gen)->begin();

    return 2;
}

int IteratorAtEnd(lua_State* L)
{
    if (!CheckArgCount(L, 2)) { return luaL_error(L, "Bad number of args"); }
    using generator_type = cppcoro::generator<ECS::Entity>;
    using iterator = typename generator_type::iterator;

    generator_type* gen = *(generator_type**)lua_touserdata(L, 1);
    iterator* iter = (iterator*)lua_touserdata(L, 2);

    lua_pushboolean(L, *iter == gen->end());
    return 1;
}

int GetEntityFromIterator(lua_State* L)
{
    if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }
    using iter = cppcoro::generator<ECS::Entity>::iterator;
    iter iterator = *(iter*)lua_touserdata(L, 1);

    ECS::Entity* luaEntity = (ECS::Entity*)lua_newuserdata(L, sizeof(ECS::Entity));
    *luaEntity = *iterator;
    return 1;
}

int IteratorAdvance(lua_State* L)
{
    if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }
    using iter = cppcoro::generator<ECS::Entity>::iterator;
    iter* iterator = (iter*)lua_touserdata(L, 1);
    ++(*iterator);
    return 0;
}

int FastBeginFree(lua_State* L)
{
    if (!CheckArgCount(L, 1)) { return luaL_error(L, "Bad number of args"); }
    using generator_type = cppcoro::generator<ECS::Entity>;
    using iterator = typename generator_type::iterator;

    generator_type** gen = (generator_type**)lua_touserdata(L, 1);
    delete *gen;
    return 0;
}

}
}