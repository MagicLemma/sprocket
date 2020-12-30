#pragma once

class lua_State;

namespace Sprocket {

void RegisterEntityFunctions(lua_State* L);

namespace Lua {

int NewEntity(lua_State* L);

int FastBegin(lua_State* L);
int IteratorAtEnd(lua_State* L);
int GetEntityFromIterator(lua_State* L);
int IteratorAdvance(lua_State* L);

}
}