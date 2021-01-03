#pragma once

struct lua_State;

namespace Sprocket {

void RegisterEntityFunctions(lua_State* L);

namespace Lua {

int NewEntity(lua_State* L);
int DeleteEntity(lua_State* L);

int Each_New(lua_State* L);
int Each_Delete(lua_State* L);
int Each_Iter_Start(lua_State* L);
int Each_Iter_Valid(lua_State* L);
int Each_Iter_Next(lua_State* L);
int Each_Iter_Get(lua_State* L);

}
}