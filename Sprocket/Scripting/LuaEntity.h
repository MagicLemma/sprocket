#pragma once

class lua_State;

namespace Sprocket {

void RegisterEntityFunctions(lua_State* L);

namespace Lua {

int NewEntity(lua_State* L);

}
}