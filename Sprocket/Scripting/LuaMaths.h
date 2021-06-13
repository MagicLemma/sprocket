#pragma once

struct lua_State;

namespace Sprocket {
namespace lua {

class Script;

void load_vec3_functions(lua::Script& script);
void load_vec2_functions(lua::Script& script);

}
}