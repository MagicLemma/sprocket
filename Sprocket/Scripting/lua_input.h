#pragma once

namespace spkt {

class input_store;

namespace lua {

class Script;

// Loads the given input_store into the scripts virtual machine, alongside a set
// of accessor functions. It is up to the user to keep the input_store state valid.
void load_input_store(lua::Script& script, input_store& input);

}
}