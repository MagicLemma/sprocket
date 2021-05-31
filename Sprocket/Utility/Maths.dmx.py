"""
A module for loading in the Maths types to Datamatic.
"""

def main(context):

    @context.types.register("glm::vec2", length=2)
    @context.types.register("glm::vec3", length=3)
    @context.types.register("glm::vec4", length=4)
    @context.types.register("glm::quat", length=4)
    def _(typename, obj, length) -> str:
        assert isinstance(obj, list)
        assert len(obj) == length
        rep = ", ".join(context.types.parse("float", val) for val in obj)
        return f"{typename}{{{rep}}}"


    @context.types.register("ecs::Identifier")
    def _(typename, obj) -> str:
        assert isinstance(obj, int)
        return f'static_cast<{typename}>({obj})'


    @context.types.register("glm::mat4") # TODO: Implement
    def _(typename, obj) -> str:
        return f"{typename}{{1.0}}"
