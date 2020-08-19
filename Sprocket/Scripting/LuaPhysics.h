#pragma once

class lua_State;

namespace Sprocket {

void RegisterPhysicsFunctions(lua_State* L);
    // Register all the functions in this header with the given lua state.

namespace Lua {

int GetPhysicsComponent(lua_State* L);
    // Args: 0
    // Rets: 13 - All attributes of PhysicsComponent except Colliders
    // We will make the collider stuff accessible when we split the
    // component

int SetPhysicsComponent(lua_State* L);
    // Args: 13 - See above
    // Rets: 0

}

}