#pragma once

class lua_State;

namespace Sprocket {

void RegisterEntityFunctions(lua_State* L);

namespace Lua {

int NewEntity(lua_State* L);

// Removes the reference to the specified entity from the spawned
// entity map. The entity will become inaccessible to the script after.
int Forget(lua_State* L);

}
}