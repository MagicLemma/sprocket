#pragma once

class lua_State;

namespace Sprocket {

void RegisterTransformFunctions(lua_State* L);

namespace Lua {

// Functions that will be reigstered with all Lua engines for accessing
// and settings the position/orientation of the Entity corresponding to
// this lua state.

int SetLookAt(lua_State* L);
    // Args: 6 - Position x, y, z, and Target x, y, z.
    // Rets: 0
    // Sets the position of the entity to the given Position and alters
    // the orientation so that it is looking at the Target.

int RotateY(lua_State* L);
    // Args: 1 - Angle in degrees.
    // Rets: 0
    // Rotates the entity around the vertical {0, 1, 0} axis by the
    // given amount of degrees.

int GetForwardsDir(lua_State* L);
    // Args: 0
    // Rets: 3 - The x, y, z of the entity's forward vector.

int GetRightDir(lua_State* L);
    // Args: 0
    // Rets: 3- The x, y, z of the entity's right vector.

int MakeUpright(lua_State* L);
    // Args: 1 - yaw
    // Rets: 0

}
}