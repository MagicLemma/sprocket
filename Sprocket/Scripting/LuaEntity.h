#pragma once

struct lua_State;

namespace Sprocket {

class InputProxy;
namespace ecs { class Scene; }

namespace lua {

// Loads the given scene into the given lua state, and provides functions for
// creating/deleting entities as well as iterating entities. These assume that there
// is a global called __scene__ that points to an ecs::Scene.
void register_scene_functions(lua_State* L);

// Loads a bunch of helper functions to ease entity manipulation.
void register_entity_transformation_functions(lua_State* L);

// Loads functions for accessing keyboard and mouse input. These assume that there
// are globals called __input__ and __window__ that pointer to an InputProxy.
void register_input_functions(lua_State* L);

}
}