"""
A module for loading in the Maths types to Datamatic.
"""
from Datamatic.Types import register, parse

@register("glm::vec2")
def _(typename, obj) -> str:
    assert isinstance(obj, list)
    assert len(obj) == 2
    rep = ", ".join(parse("float", val) for val in obj)
    return f"{typename}{{{rep}}}"


@register("glm::vec3")
def _(typename, obj) -> str:
    assert isinstance(obj, list)
    assert len(obj) == 3
    rep = ", ".join(parse("float", val) for val in obj)
    return f"{typename}{{{rep}}}"


@register("glm::vec4")
@register("glm::quat")
def _(typename, obj) -> str:
    assert isinstance(obj, list)
    assert len(obj) == 4
    rep = ", ".join(parse("float", val) for val in obj)
    return f"{typename}{{{rep}}}"


@register("ecs::Identifier")
def _(typename, obj) -> str:
    assert isinstance(obj, int)
    return f'static_cast<{typename}>({obj})'


@register("glm::mat4") # TODO: Implement
def _(typename, obj) -> str:
    return f"{typename}{{1.0}}"


@register("std::queue<glm::vec3>") # TODO: Implement
def _(typename, obj) -> str:
    return f"{typename}{{}}"
