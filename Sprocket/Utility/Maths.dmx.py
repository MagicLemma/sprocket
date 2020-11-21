"""
A module for loading in the Maths types to Datamatic.
"""
from Datamatic.Types import Type, Float


class Vec2(Type):
    def __init__(self, val):
        assert isinstance(val, list)
        assert all([isinstance(x, float) for x in val])
        self.x, self.y = [Float(t) for t in val]

    def __repr__(self):
        return f'{self.typename()}{{{self.x}, {self.y}}}'

    @staticmethod
    def typename():
        return "glm::vec2"


class Vec3(Type):
    def __init__(self, val):
        assert isinstance(val, list)
        assert all([isinstance(x, float) for x in val])
        self.x, self.y, self.z = [Float(t) for t in val]

    def __repr__(self):
        return f'{self.typename()}{{{self.x}, {self.y}, {self.z}}}'

    @staticmethod
    def typename():
        return "glm::vec3"


class Vec4(Type):
    def __init__(self, val):
        assert isinstance(val, list)
        assert all([isinstance(x, float) for x in val])
        self.x, self.y, self.z, self.w = [Float(t) for t in val]

    def __repr__(self):
        return f'{self.typename()}{{{self.x}, {self.y}, {self.z}, {self.w}}}'

    @staticmethod
    def typename():
        return "glm::vec4"


class Quat(Type):
    def __init__(self, val):
        assert isinstance(val, list)
        assert all([isinstance(x, float) for x in val])
        self.x, self.y, self.z, self.w = [Float(t) for t in val]

    def __repr__(self):
        return f'{self.typename()}{{{self.x}, {self.y}, {self.z}, {self.w}}}'

    @staticmethod
    def typename():
        return "glm::quat"


class Mat4(Type): # TODO: Implement
    def __init__(self, val):
        pass

    def __repr__(self):
        return f"{self.typename()}{{1.0}}"

    @staticmethod
    def typename():
        return "glm::mat4"


class QueueVec3(Type): # TODO: Implement
    def __init__(self, val):
        pass

    def __repr__(self):
        return f"{self.typename()}{{}}"

    @staticmethod
    def typename():
        return "std::queue<glm::vec3>"
