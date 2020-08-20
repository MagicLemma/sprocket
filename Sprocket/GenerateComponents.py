import json
from pprint import pprint

def default_cpp_repr(cpp_type, value):
    if cpp_type == "Maths::vec3":
        x, y, z = value
        return f'{{{x}, {y}, {z}}}'
    
    if cpp_type == "std::string":
        return f'"{value}"'

    return str(value)

def print_attr(attr):
    cpp_name = attr["Name"]
    cpp_type = attr["Type"]
    default = attr.get("Default")
    savable = attr.get("Savable", True)
    
    line = f"    {cpp_type} {cpp_name}"
    if default is not None:
        line += " = " + default_cpp_repr(cpp_type, default)
    line += ";"
    print(line)

with open("ComponentSpec.json") as specfile:
    spec = json.loads(specfile.read())

enums = spec["Enums"]

print("#pragma once")
print('#include "Maths.h"')
print("#include <string>")
print()
print("namespace Sprocket {")
print()
for component in spec["Components"]:
    print(f"struct {component['Name']} {{")
    for attr in component["Attributes"]:
        print_attr(attr)
    print("};")
    print()
print("}")