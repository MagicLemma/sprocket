#pragma once
#include "ecs.h"

#include <sprocket/scripting/lua_script.h>
#include <sprocket/scripting/lua_converter.h>

namespace spkt::lua {

template <>
struct converter<anvil::entity>
{
    static void push(lua_State* L, const anvil::entity& value);
    static anvil::entity read(lua_State* L, int index);
};

}

namespace anvil {

// Loads the given scene into the given lua state, and provides functions for
// creating/deleting entities as well as iterating entities. Also provides functions
// for access the singleton entity, which in turn gives access to the keyboard, mouse
// and window via the InputSingleton.
void load_registry(spkt::lua::script& script, registry& registry);

}
