#pragma once
#include "ecs.h"

#include <Sprocket/Scripting/lua_script.h>
#include <Sprocket/Scripting/lua_converter.h>

namespace spkt::lua {

template <>
struct converter<spkt::entity>
{
    static void push(lua_State* L, const entity& value);
    static entity read(lua_State* L, int index);
};

}

namespace spkt {

// Loads the given scene into the given lua state, and provides functions for
// creating/deleting entities as well as iterating entities. Also provides functions
// for access the singleton entity, which in turn gives access to the keyboard, mouse
// and window via the InputSingleton.
void load_registry(spkt::lua::script& script, registry& registry);

}
