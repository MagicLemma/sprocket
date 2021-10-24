#pragma once
#include "ecs.h"

#include <sprocket/Scripting/lua_script.h>
#include <sprocket/Scripting/lua_converter.h>

namespace spkt::lua {

template <>
struct converter<{{Global::namespace}}::entity>
{
    static void push(lua_State* L, const {{Global::namespace}}::entity& value);
    static {{Global::namespace}}::entity read(lua_State* L, int index);
};

}

namespace {{Global::namespace}} {

// Loads the given scene into the given lua state, and provides functions for
// creating/deleting entities as well as iterating entities. Also provides functions
// for access the singleton entity, which in turn gives access to the keyboard, mouse
// and window via the InputSingleton.
void load_registry(spkt::lua::script& script, registry& registry);

}