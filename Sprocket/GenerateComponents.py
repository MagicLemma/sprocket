import json
import os

from Datamatic import Declarations

sprocket = os.path.abspath(os.path.dirname(__file__))
components_h = os.path.join(sprocket, "Scene", "Components.h")
serialiser_h = os.path.join(sprocket, "Scene", "Serialiser.h")
serialiser_cpp = os.path.join(sprocket, "Scene", "Serialiser.cpp")

with open("ComponentSpec.json") as specfile:
    spec = json.loads(specfile.read())

ENUMS = spec["Enums"]


def cpp_float(value):
    assert type(value) in {int, float}
    if "." not in str(value):
        return f"{value}.0f"
    return f"{value}f"


def default_cpp_repr(cpp_type, value):
    if cpp_type in ENUMS:
        assert value in ENUMS[cpp_type]
        return f"{cpp_type}::{value}"

    if cpp_type == "Maths::vec2":
        x, y = value
        return f'{{{cpp_float(x)}, {cpp_float(y)}}}'

    if cpp_type == "Maths::vec3":
        x, y, z = value
        return f'{{{cpp_float(x)}, {cpp_float(y)}, {cpp_float(z)}}}'

    if cpp_type in {"Maths::vec4", "Maths::quat"}:
        x, y, z, w = value
        return f'{{{cpp_float(x)}, {cpp_float(y)}, {cpp_float(z)}, {cpp_float(w)}}}'

    if cpp_type == "bool":
        return "true" if value else "false"

    if cpp_type == "int":
        return str(value)

    if cpp_type == "float":
        return cpp_float(value)
    
    if cpp_type == "std::string":
        return f'"{value}"'

    return str(value)


def print_attr(attr):
    cpp_name = attr["Name"]
    cpp_type = attr["Type"]
    default = attr.get("Default")
    
    line = f"    {cpp_type} {cpp_name}"
    if default is not None:
        line += " = " + default_cpp_repr(cpp_type, default)
    line += ";"
    return line + "\n"

out = "// GENERATED FILE\n"
out += "#pragma once\n"
out += '#include "Maths.h"\n'
out += "#include <queue>\n"
out += "#include <string>\n\n"
out += "namespace Sprocket{\n\n"

out += "// Enums\n"
for enum_name, enum_values in spec["Enums"].items():
    out += f"enum class {enum_name}\n{{\n"
    for value in enum_values:
        out += f"    {value},\n"
    out += "};\n\n"

out += "// Components\n"
for component in spec["Components"]:
    out += f"struct {component['Name']}\n{{\n"
    for attr in component["Attributes"]:
        out += print_attr(attr)
    out += "};\n\n"

out += "}\n"

with open(components_h, "w") as components_h_file:
    components_h_file.write(out)

print(out)
print(Declarations.X)