from datetime import datetime

header = """
#include "LuaComponents.h"
#include "LuaGlobals.h"
#include "Entity.h"
#include "Maths.h"
#include "Components.h"

#include <lua.hpp>

namespace Sprocket {

void RegisterComponentFunctions(lua_State* L)
{
"""

middle = """}

namespace Lua {

"""

footer = """

}
}
"""

def generate_header(spec, output):
    out = f"// GENERATED FILE @ {datetime.now()}\n"
    out += "class lua_State;\n\n"
    out += "namespace Sprocket {\n\n"
    out += "void RegisterComponentFunctions(lua_State* L);\n"
    out += "    // Register all the functions in this header with the given lua state.\n\n"
    out += "namespace Lua {\n\n"
    for component in spec["Components"]:
        if not component.get("Scriptable", True):
            continue
        name = component["Name"]
        out += f"int Get{name}(lua_State* L);\n"
        out += f"int Set{name}(lua_State* L);\n\n"
    out += "}\n}"

    with open(output, "w") as outfile:
        outfile.write(out)

def generate_cpp(spec, output):
    out = f"// GENERATED FILE @ {datetime.now()}\n"
    out += header
    for component in spec["Components"]:
        if not component.get("Scriptable", True):
            continue
        name = component["Name"]
        out += f'    lua_register(L, "Lua_Get{name}", &Lua::Get{name});\n'
        out += f'    lua_register(L, "Lua_Set{name}", &Lua::Set{name});\n\n'
    out += middle
    for component in spec["Components"]:
        name = component["Name"]
        if not component.get("Scriptable", True):
            continue
        out += f"int Get{name}(lua_State* L)\n{{\n"
        out += "    x\n"
        out += "}\n\n"
        out += f"int Set{name}(lua_State* L)\n{{\n"
        out += "    x\n"
        out += "}\n\n"
    out += footer
    print(out)