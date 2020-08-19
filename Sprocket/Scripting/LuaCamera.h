#pragma once

class lua_State;

namespace Sprocket {

void RegisterCameraFunctions(lua_State* L);
    // Register all the functions in this header with the given lua state.

namespace Lua {

int GetCameraComponent(lua_State* L);
    // Args: 0
    // Rets: 2 - Pitch and FOV

int SetCameraComponent(lua_State* L);
    // Args: 2 - See above
    // Rets: 0

}
}