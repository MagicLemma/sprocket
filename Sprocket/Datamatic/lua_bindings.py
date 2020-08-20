from datetime import datetime

def generate(spec, output):
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