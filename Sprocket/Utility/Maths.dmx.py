"""
A module for loading in the Maths types to Datamatic.
"""
from Datamatic.Types import parse

@parse.register("glm::vec2")
def _(typename, obj) -> str:
    assert isinstance(obj, list)
    assert len(obj) == 2
    rep = ", ".join(parse("float", val) for val in obj)
    return f"{typename}{{{rep}}}"


@parse.register("glm::vec3")
def _(typename, obj) -> str:
    assert isinstance(obj, list)
    assert len(obj) == 3
    rep = ", ".join(parse("float", val) for val in obj)
    return f"{typename}{{{rep}}}"


@parse.register("glm::vec4")
@parse.register("glm::quat")
def _(typename, obj) -> str:
    assert isinstance(obj, list)
    assert len(obj) == 4
    rep = ", ".join(parse("float", val) for val in obj)
    return f"{typename}{{{rep}}}"


@parse.register("ecs::Identifier")
def _(typename, obj) -> str:
    assert isinstance(obj, int)
    return f'static_cast<{typename}>({obj})'


@parse.register("glm::mat4") # TODO: Implement
def _(typename, obj) -> str:
    return f"{typename}{{1.0}}"


@parse.register("std::queue<glm::vec3>") # TODO: Implement
def _(typename, obj) -> str:
    return f"{typename}{{}}"
