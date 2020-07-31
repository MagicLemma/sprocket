import enum

class Enum(enum.Enum):
    def __str__(self):
        return f"{type(self).__name__}::{self.name}"

class Component:
    pass

class Vec2:
    def __init__(self, x, y):
        self.x = x
        self.y = y

class Vec3:
    def __init__(self, x, y, z):
        self.x = x
        self.y = y
        self.z = z

class Quat:
    def __init__(self, w, x, y, z):
        self.w = w
        self.x = x
        self.y = y
        self.z = z

class Mat4:
    pass

class Vector:
    def __init__(self, element_type, elements = []):
        self.element_type = element_type

class Queue:
    def __init__(self, element_type, elements = []):
        self.element_type = element_type