from ComponentBuilder import Enum, Component, Vec3, Mat4, Quat, Vector

class TransformComponent(Component):
    position:    Vec3 = Vec3(0, 0, 0)
    orientation: Quat = Quat(0, 0, 0, 1)

class ModelComponent(Component):
    model:        str   = ""
    scale:        float = 1.0

    texture:      str   = ""
    shineDamper:  float = 1.0
    reflectivity: float = 0.0

class Collider(Enum):
    NONE    = 0
    BOX     = 1
    SPHERE  = 2
    CAPSULE = 3

class PhysicsComponent(Component):
    velocity:            Vec3     = Vec3(0, 0, 0)
    gravity:             bool     = True
    frozen:              bool     = False

    collider:            Collider = Collider.NONE
    halfExtents:         Vec3     = Vec3(0, 0, 0)
    radius:              float    = 1.0
    height:              float    = 1.0

    mass:                float    = 1.0
    bounciness:          float    = 0.5
    frictionCoefficient: float    = 0.3
    rollingResistance:   float    = 0.0

class NameComponent(Component):
    name: str = None

class ScriptComponent(Component):
    script: str  = None
    active: bool = True

class PlayerComponent(Component):
    movingForwards:  bool  = False
    movingBackwards: bool  = False
    movingLeft:      bool  = False
    movingRight:     bool  = False
    jumping:         bool  = False

    direction:       Vec3  = Vec3(0, 0, 0)
    yaw:             float = 0.0
    pitch:           float = 0.0

class CameraComponent(Component):
    projection: Mat4  = None
    pitch:      float = 0.0

class SelectComponent(Component):
    selected: bool = False
    hovered:  bool = False
