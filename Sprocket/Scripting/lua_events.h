#pragma once
#include <Sprocket/Scripting/LuaScript.h>
#include <Sprocket/Utility/input_store.h>

struct lua_State;

namespace spkt {
namespace lua {

// Loads the given input_store into the scripts virtual machine, alongside a set
// of accessor functions. It is up to the user to keep the input_store state valid.
void load_input_store(lua::Script& script, input_store& input);

}
}