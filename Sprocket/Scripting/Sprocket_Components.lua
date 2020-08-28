-- GENERATED FILE @ 2020-08-28 22:24:01.689777
NameComponent = Class(function(self, name)
    self.name = name
end)

function GetNameComponent(entity)
    x0 = Lua_GetNameComponent(entity)
    return NameComponent(x0)
end

function SetNameComponent(entity, c)
    Lua_SetNameComponent(entity, c.name)
end

TransformComponent = Class(function(self, position)
    self.position = position
end)

function GetTransformComponent(entity)
    x0, x1, x2 = Lua_GetTransformComponent(entity)
    return TransformComponent(Vec3(x0, x1, x2))
end

function SetTransformComponent(entity, c)
    Lua_SetTransformComponent(entity, c.position.x, c.position.y, c.position.z)
end

ModelComponent = Class(function(self, model, texture, shineDamper, reflectivity)
    self.model = model
    self.texture = texture
    self.shineDamper = shineDamper
    self.reflectivity = reflectivity
end)

function GetModelComponent(entity)
    x0, x1, x2, x3 = Lua_GetModelComponent(entity)
    return ModelComponent(x0, x1, x2, x3)
end

function SetModelComponent(entity, c)
    Lua_SetModelComponent(entity, c.model, c.texture, c.shineDamper, c.reflectivity)
end

RigidBody3DComponent = Class(function(self, velocity, gravity, frozen, bounciness, frictionCoefficient, rollingResistance, force, onFloor)
    self.velocity = velocity
    self.gravity = gravity
    self.frozen = frozen
    self.bounciness = bounciness
    self.frictionCoefficient = frictionCoefficient
    self.rollingResistance = rollingResistance
    self.force = force
    self.onFloor = onFloor
end)

function GetRigidBody3DComponent(entity)
    x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11 = Lua_GetRigidBody3DComponent(entity)
    return RigidBody3DComponent(Vec3(x0, x1, x2), x3, x4, x5, x6, x7, Vec3(x8, x9, x10), x11)
end

function SetRigidBody3DComponent(entity, c)
    Lua_SetRigidBody3DComponent(entity, c.velocity.x, c.velocity.y, c.velocity.z, c.gravity, c.frozen, c.bounciness, c.frictionCoefficient, c.rollingResistance, c.force.x, c.force.y, c.force.z, c.onFloor)
end

BoxCollider3DComponent = Class(function(self, mass)
    self.mass = mass
end)

function GetBoxCollider3DComponent(entity)
    x0 = Lua_GetBoxCollider3DComponent(entity)
    return BoxCollider3DComponent(x0)
end

function SetBoxCollider3DComponent(entity, c)
    Lua_SetBoxCollider3DComponent(entity, c.mass)
end

SphereCollider3DComponent = Class(function(self, mass)
    self.mass = mass
end)

function GetSphereCollider3DComponent(entity)
    x0 = Lua_GetSphereCollider3DComponent(entity)
    return SphereCollider3DComponent(x0)
end

function SetSphereCollider3DComponent(entity, c)
    Lua_SetSphereCollider3DComponent(entity, c.mass)
end

CapsuleCollider3DComponent = Class(function(self, mass)
    self.mass = mass
end)

function GetCapsuleCollider3DComponent(entity)
    x0 = Lua_GetCapsuleCollider3DComponent(entity)
    return CapsuleCollider3DComponent(x0)
end

function SetCapsuleCollider3DComponent(entity, c)
    Lua_SetCapsuleCollider3DComponent(entity, c.mass)
end

CameraComponent = Class(function(self, fov, pitch)
    self.fov = fov
    self.pitch = pitch
end)

function GetCameraComponent(entity)
    x0, x1 = Lua_GetCameraComponent(entity)
    return CameraComponent(x0, x1)
end

function SetCameraComponent(entity, c)
    Lua_SetCameraComponent(entity, c.fov, c.pitch)
end

SelectComponent = Class(function(self, selected, hovered)
    self.selected = selected
    self.hovered = hovered
end)

function GetSelectComponent(entity)
    x0, x1 = Lua_GetSelectComponent(entity)
    return SelectComponent(x0, x1)
end

function SetSelectComponent(entity, c)
    Lua_SetSelectComponent(entity, c.selected, c.hovered)
end

PathComponent = Class(function(self, speed)
    self.speed = speed
end)

function GetPathComponent(entity)
    x0 = Lua_GetPathComponent(entity)
    return PathComponent(x0)
end

function SetPathComponent(entity, c)
    Lua_SetPathComponent(entity, c.speed)
end

GridComponent = Class(function(self, x, z)
    self.x = x
    self.z = z
end)

function GetGridComponent(entity)
    x0, x1 = Lua_GetGridComponent(entity)
    return GridComponent(x0, x1)
end

function SetGridComponent(entity, c)
    Lua_SetGridComponent(entity, c.x, c.z)
end

LightComponent = Class(function(self, colour, attenuation, brightness)
    self.colour = colour
    self.attenuation = attenuation
    self.brightness = brightness
end)

function GetLightComponent(entity)
    x0, x1, x2, x3, x4, x5, x6 = Lua_GetLightComponent(entity)
    return LightComponent(Vec3(x0, x1, x2), Vec3(x3, x4, x5), x6)
end

function SetLightComponent(entity, c)
    Lua_SetLightComponent(entity, c.colour.x, c.colour.y, c.colour.z, c.attenuation.x, c.attenuation.y, c.attenuation.z, c.brightness)
end

