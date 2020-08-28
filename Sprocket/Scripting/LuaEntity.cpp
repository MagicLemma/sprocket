#include "LuaEntity.h"
#include "LuaGlobals.h"
#include "KeyboardProxy.h"
#include "MouseProxy.h"
#include "Entity.h"
#include "Components.h"

#include <lua.hpp>

namespace Sprocket {

void RegisterEntityFunctions(lua_State* L)
{
    lua_register(L, "NewEntity", &Lua::NewEntity);
}

namespace Lua {

int NewEntity(lua_State* L)
{
    if (!CheckArgCount(L, 0)) { return luaL_error(L, "Bad number of args"); }

    Entity newEntity = GetEntity(L)->NewEntity();
    auto& spawnedEntities = *GetSpawnedEntities(L);
    auto it = spawnedEntities.emplace(newEntity.Id(), newEntity);
    lua_pushlightuserdata(L, &it.first->second);
    return 1;
}

}
}