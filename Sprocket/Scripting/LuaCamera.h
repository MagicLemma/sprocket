#pragma once

class lua_State;

namespace Sprocket {

void RegisterCameraFunctions(lua_State* L);
    // Register all the functions in this header with the given lua state.

namespace Lua {

// Functions that will be reigstered with all Lua engines for accessing
// and setting the camera component.

int GetPitch(lua_State* L);
    // Args: 0
    // Rets: 1 - The pitch value on the CameraComponent.

int SetPitch(lua_State* L);
    // Args: 1 - The new value for the pitch.
    // Rets: 0

}
}