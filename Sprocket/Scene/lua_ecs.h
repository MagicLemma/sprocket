#pragma once
#include <Sprocket/Scene/ecs.h>
#include <Sprocket/Scripting/LuaConverter.h>

namespace spkt {
namespace lua {

class script;

template <>
struct Converter<spkt::entity>
{
    static void push(lua_State* L, const spkt::entity& value);
    static spkt::entity read(lua_State* L, int index);
};

// Loads the given scene into the given lua state, and provides functions for
// creating/deleting entities as well as iterating entities. Also provides functions
// for access the singleton entity, which in turn gives access to the keyboard, mouse
// and window via the InputSingleton.
void load_registry(lua::script& script, spkt::registry& registry);

}
}