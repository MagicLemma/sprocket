#pragma once

class lua_State;

namespace Sprocket {

void RegisterEntityFunctions(lua_State* L);

namespace Lua {

int NewEntity(lua_State* L);

int NewFast(lua_State* L);
int DeleteFast(lua_State* L);
int IterStart(lua_State* L);
int IterValid(lua_State* L);
int IterNext(lua_State* L);
int IterGet(lua_State* L);

}
}