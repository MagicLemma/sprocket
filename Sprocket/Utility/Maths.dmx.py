"""
A module for loading in the Maths types to Datamatic.
"""
from Datamatic.Types import register, parse_type

@register("glm::vec2")
def parse(obj) -> str:
    [x, y] = obj
    x_repr = parse_type("float", x)
    y_repr = parse_type("float", y)
    return f"glm::vec2{{{x_repr}, {y_repr}}}"


@register("glm::vec3")
def parse(obj) -> str:
    [x, y, z] = obj
    x_repr = parse_type("float", x)
    y_repr = parse_type("float", y)
    z_repr = parse_type("float", z)
    return f"glm::vec3{{{x_repr}, {y_repr}, {z_repr}}}"


@register("glm::vec4")
def parse(obj) -> str:
    [x, y, z, w] = obj
    x_repr = parse_type("float", x)
    y_repr = parse_type("float", y)
    z_repr = parse_type("float", z)
    w_repr = parse_type("float", w)
    return f"glm::vec4{{{x_repr}, {y_repr}, {z_repr}, {w_repr}}}"


@register("glm::quat")
def parse(obj) -> str:
    [x, y, z, w] = obj
    x_repr = parse_type("float", x)
    y_repr = parse_type("float", y)
    z_repr = parse_type("float", z)
    w_repr = parse_type("float", w)
    return f"glm::quat{{{x_repr}, {y_repr}, {z_repr}, {w_repr}}}"


@register("ecs::Identifier")
def parse(obj) -> str:
    assert isinstance(obj, int)
    return f'static_cast<ecs::Identifier>({obj})'


@register("glm::mat4") # TODO: Implement
def parse(obj) -> str:
    return "glm::mat4{1.0}"


@register("std::queue<glm::vec3>") # TODO: Implement
def parse(obj) -> str:
    return "std::queue<glm::vec3>{}"
