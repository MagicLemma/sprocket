#pragma once
#include <Sprocket/Scene/ecs.h>

namespace spkt {
namespace lua {

class Script;

// Loads the given scene into the given lua state, and provides functions for
// creating/deleting entities as well as iterating entities. Also provides functions
// for access the singleton entity, which in turn gives access to the keyboard, mouse
// and window via the InputSingleton.
void load_registry(lua::Script& script, spkt::registry& registry);

}
}