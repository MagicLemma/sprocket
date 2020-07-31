from ComponentBuilder import Enum, Component, Vec3, Mat4, Quat, Vector, Queue
import ComponentSpec
from collections import Iterable
import os

def cpp_type(py_type):
    if py_type == str:
        return "std::string"
    if py_type == bool:
        return "bool"
    if py_type == int:
        return "int"
    if py_type == float:
        return "float"
    if py_type == Vec3:
        return "Maths::vec3"
    if py_type == Mat4:
        return "Maths::mat4"
    if py_type == Quat:
        return "Maths::quat"
    if isinstance(py_type, Vector):
        return f"std::vector<{cpp_type(py_type.element_type)}>"
    if isinstance(py_type, Queue):
        return f"std::queue<{cpp_type(py_type.element_type)}>"
    if issubclass(py_type, Enum):
        return py_type.__name__

def cpp_val(py_val):
    if isinstance(py_val, str):
        return f'"{py_val}"'
    if isinstance(py_val, bool):
        return "true" if py_val else "false"
    if isinstance(py_val, int):
        return str(py_val)
    if isinstance(py_val, float):
        return f"{py_val}f"
    if isinstance(py_val, Vec3):
        return f"{{{py_val.x}, {py_val.y}, {py_val.z}}}"
    if isinstance(py_val, Quat):
        return f"{{{py_val.w}, {py_val.x}, {py_val.y}, {py_val.z}}}"
    if isinstance(py_val, Iterable):
        return "{" + ", ".join([cpp_val(x) for x in py_val]) + "}"
    return py_val

def cpp_struct(cls):
    ret = f"struct {cls.__name__} {{\n"
    for name, py_type in cls.__annotations__.items():
        py_val = getattr(cls, name)
        if py_val is not None:
            ret += f"    {cpp_type(py_type)} {name} = {cpp_val(py_val)};\n"
        else:
            ret += f"    {cpp_type(py_type)} {name};\n"
    ret += "};\n"
    return ret

def cpp_enum(cls):
    ret = f"enum class {cls.__name__} {{\n"
    for name, val in cls.__members__.items():
        ret += f"    {name} = {val.value},\n"
    ret += "};\n"
    return ret

def generate_components():
    ret = "// GENERATED - DO NOT EDIT\n"
    ret += "#pragma once\n"
    ret += '#include "Maths.h"\n'
    ret += '#include <vector>\n'
    ret += '#include <string>\n'
    ret += '#include <queue>\n\n'
    ret += "namespace Sprocket {\n\n"

    for cls in Enum.__subclasses__():
        ret += cpp_enum(cls) + "\n"

    for cls in Component.__subclasses__():
        ret += cpp_struct(cls) + "\n"

    ret += "}"
    return ret

COMP_HEADER = os.path.dirname(os.path.abspath(__file__))
COMP_HEADER = os.path.join(COMP_HEADER, "Components.h")
with open(COMP_HEADER, "w") as component_header:
    component_header.write(generate_components())