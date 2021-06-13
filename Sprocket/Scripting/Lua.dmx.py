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
    num_attrs = 0
    constructor_sig = []
    for attr in comp["attributes"]:
        if attr["type"]  == "glm::vec4":
            constructor_sig.append(f"Vec4(x{num_attrs}, x{num_attrs+1}, x{num_attrs+2}, x{num_attrs+3})")
        elif attr["type"] == "glm::vec3":
            constructor_sig.append(f"Vec3(x{num_attrs}, x{num_attrs+1}, x{num_attrs+2})")
        elif attr["type"] == "glm::vec2":
            constructor_sig.append(f"Vec3(x{num_attrs}, x{num_attrs+1})")
        else:
            constructor_sig.append(f"x{num_attrs}")
        num_attrs += DIMENSION[attr["type"]]
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
    def lua_setter(ctx):
        out = ""
        name = ctx.comp["name"]
        indent = " " * 8 # We indent extra to make the generated C++ file look nicer

        out += f'function Set{name}(entity, c)\n'
        out += indent + f'    _Set{name}(entity, '
        args = []
        for attr in ctx.comp["attributes"]:
            n = attr["name"]
            if attr["type"] == "glm::vec3":
                args.extend([f'c.{n}.x', f'c.{n}.y', f'c.{n}.z'])
            else:
                args.append(f'c.{attr["name"]}')
        out += ", ".join(args)
        out += ')\n'
        out += indent + "end"
        return out

    @reg.compmethod
    def lua_adder(ctx):
        out = ""
        name = ctx.comp["name"]
        indent = " " * 8 # We indent extra to make the generated C++ file look nicer

        out += f'function Add{name}(entity, c)\n'
        out += indent + f'    _Add{name}(entity, '
        args = []
        for attr in ctx.comp["attributes"]:
            n = attr["name"]
            if attr["type"] == "glm::vec3":
                args.extend([f'c.{n}.x', f'c.{n}.y', f'c.{n}.z'])
            else:
                args.append(f'c.{attr["name"]}')
        out += ", ".join(args)
        out += ')\n'
        out += indent + "end"
        return out