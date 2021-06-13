"""
Plugin for generating Lua code.
"""

DIMENSION = {
    "glm::vec2": 2,
    "glm::vec3": 3,
    "glm::vec4": 4,
    "glm::quat": 4,
    "int": 1,
    "float": 1,
    "double": 1,
    "bool": 1,
    "std::string": 1
}


def signature(comp):
    constructor_sig = []
    for idx in len(comp["attributes"]):
        constructor_sig.append(f"x{idx}")
    return constructor_sig


def dimension(comp):
    count = 0
    for attr in comp["attributes"]:
        count += DIMENSION[attr["type"]]
    return count


def main(reg):

    @reg.compmethod
    def lua_dimension(ctx):
        return str(dimension(ctx.comp))
    
    @reg.compmethod
    def lua_sig(ctx):
        return ", ".join(attr['name'] for attr in ctx.comp['attributes'])

    @reg.compmethod
    def lua_getter(ctx):
        out = ""
        name = ctx.comp["name"]
        indent = " " * 8 # We indent extra to make the generated C++ file look nicer

        out += f'function Get{name}(entity)\n'
        pack = indent + "    " + ", ".join([f'x{i}' for i in range(dimension(ctx.comp))])
        out += pack + f" = _Get{name}(entity)\n"
        out += indent + f'    return {name}({", ".join(signature(ctx.comp))})\n'
        out += indent + "end"
        return out

    @reg.compmethod
    def lua_arglist(ctx):
        return ", ".join(f"x{i}" for i in range(len(ctx.comp['attributes'])))