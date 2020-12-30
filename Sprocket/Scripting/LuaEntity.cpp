#include "LuaEntity.h"
#include "LuaGlobals.h"
#include "ECS.h"
#include "Scene.h"

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
    ECS::Entity* luaEntity = (ECS::Entity*)lua_newuserdata(L, sizeof(ECS::Entity));
    *luaEntity = GetScene(L)->Entities().New();
    return 1;
}

}
}