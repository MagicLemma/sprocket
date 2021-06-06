"""
Plugin for generating Lua code.
"""

def get_attr_count_and_sig(comp):
    num_attrs = 0
    constructor_sig = []
    for attr in comp["attributes"]:
        if attr["type"]  == "glm::vec4":
            constructor_sig.append(f"Vec4(x{num_attrs}, x{num_attrs+1}, x{num_attrs+2}, x{num_attrs+3})")
            num_attrs += 4
        elif attr["type"] == "glm::vec3":
            constructor_sig.append(f"Vec3(x{num_attrs}, x{num_attrs+1}, x{num_attrs+2})")
            num_attrs += 3
        elif attr["type"] == "glm::vec2":
            constructor_sig.append(f"Vec3(x{num_attrs}, x{num_attrs+1})")
            num_attrs += 2
        else:
            constructor_sig.append(f"x{num_attrs}")
            num_attrs += 1
    return num_attrs, constructor_sig

def main(reg):

    @reg.compmethod("Lua.dimension")
    def _(_, comp):
        num_attrs, _ = get_attr_count_and_sig(comp)
        return str(num_attrs)
    
    @reg.compmethod("Lua.Sig")
    def _(_, comp):
        return ", ".join(attr['name'] for attr in comp['attributes'])

    @reg.compmethod("Lua.Getter")
    def _(_, comp):
        out = ""
        num_attrs, constructor_sig = get_attr_count_and_sig(comp)
        name = comp["name"]
        indent = " " * 8 # We indent extra to make the generated C++ file look nicer

        out += f'function Get{name}(entity)\n'
        pack = indent + "    " + ", ".join([f'x{i}' for i in range(num_attrs)])
        out += pack + f" = _Get{name}(entity)\n"
        out += indent + f'    return {name}({", ".join(constructor_sig)})\n'
        out += indent + "end"
        return out

    @reg.compmethod("Lua.Setter")
    def _(_, comp):
        out = ""
        name = comp["name"]
        indent = " " * 8 # We indent extra to make the generated C++ file look nicer

        out += f'function Set{name}(entity, c)\n'
        out += indent + f'    _Set{name}(entity, '
        args = []
        for attr in comp["attributes"]:
            n = attr["name"]
            if attr["type"] == "glm::vec3":
                args.extend([f'c.{n}.x', f'c.{n}.y', f'c.{n}.z'])
            else:
                args.append(f'c.{attr["name"]}')
        out += ", ".join(args)
        out += ')\n'
        out += indent + "end"
        return out

    @reg.compmethod("Lua.Adder")
    def _(_, comp):
        out = ""
        name = comp["name"]
        indent = " " * 8 # We indent extra to make the generated C++ file look nicer

        out += f'function Add{name}(entity, c)\n'
        out += indent + f'    _Add{name}(entity, '
        args = []
        for attr in comp["attributes"]:
            n = attr["name"]
            if attr["type"] == "glm::vec3":
                args.extend([f'c.{n}.x', f'c.{n}.y', f'c.{n}.z'])
            else:
                args.append(f'c.{attr["name"]}')
        out += ", ".join(args)
        out += ')\n'
        out += indent + "end"
        return out