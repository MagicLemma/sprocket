-- GENERATED FILE @ 2020-08-20 23:53:20.361987
NameComponent = Class(function(self, name)
    self.name = name
end)

function GetNameComponent()
    x0 = Lua_GetNameComponent()
    return NameComponent(x0)
end

function SetNameComponent(c)
    Lua_SetNameComponent(self.name)
end

TransformComponent = Class(function(self, position_x, position_y, position_z)
    self.position = Vec3(position_x, position_y, position_z)
end)

function GetTransformComponent()
    x0, x1, x2 = Lua_GetTransformComponent()
    return TransformComponent(x0, x1, x2)
end

function SetTransformComponent(c)
    Lua_SetTransformComponent(self.position.x, self.position.y, self.position.z)
end

ModelComponent = Class(function(self, model, scale, texture, shineDamper, reflectivity)
    self.model = model
    self.scale = scale
    self.texture = texture
    self.shineDamper = shineDamper
    self.reflectivity = reflectivity
end)

function GetModelComponent()
    x0, x1, x2, x3, x4 = Lua_GetModelComponent()
    return ModelComponent(x0, x1, x2, x3, x4)
end

function SetModelComponent(c)
    Lua_SetModelComponent(self.model, self.scale, self.texture, self.shineDamper, self.reflectivity)
end

PhysicsComponent = Class(function(self, velocity_x, velocity_y, velocity_z, gravity, frozen, mass, bounciness, frictionCoefficient, rollingResistance, force_x, force_y, force_z, onFloor)
    self.velocity = Vec3(velocity_x, velocity_y, velocity_z)
    self.gravity = gravity
    self.frozen = frozen
    self.mass = mass
    self.bounciness = bounciness
    self.frictionCoefficient = frictionCoefficient
    self.rollingResistance = rollingResistance
    self.force = Vec3(force_x, force_y, force_z)
    self.onFloor = onFloor
end)

function GetPhysicsComponent()
    x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12 = Lua_GetPhysicsComponent()
    return PhysicsComponent(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12)
end

function SetPhysicsComponent(c)
    Lua_SetPhysicsComponent(self.velocity.x, self.velocity.y, self.velocity.z, self.gravity, self.frozen, self.mass, self.bounciness, self.frictionCoefficient, self.rollingResistance, self.force.x, self.force.y, self.force.z, self.onFloor)
end

CameraComponent = Class(function(self, fov, pitch)
    self.fov = fov
    self.pitch = pitch
end)

function GetCameraComponent()
    x0, x1 = Lua_GetCameraComponent()
    return CameraComponent(x0, x1)
end

function SetCameraComponent(c)
    Lua_SetCameraComponent(self.fov, self.pitch)
end

SelectComponent = Class(function(self, selected, hovered)
    self.selected = selected
    self.hovered = hovered
end)

function GetSelectComponent()
    x0, x1 = Lua_GetSelectComponent()
    return SelectComponent(x0, x1)
end

function SetSelectComponent(c)
    Lua_SetSelectComponent(self.selected, self.hovered)
end

PathComponent = Class(function(self, speed)
    self.speed = speed
end)

function GetPathComponent()
    x0 = Lua_GetPathComponent()
    return PathComponent(x0)
end

function SetPathComponent(c)
    Lua_SetPathComponent(self.speed)
end

GridComponent = Class(function(self, x, z)
    self.x = x
    self.z = z
end)

function GetGridComponent()
    x0, x1 = Lua_GetGridComponent()
    return GridComponent(x0, x1)
end

function SetGridComponent(c)
    Lua_SetGridComponent(self.x, self.z)
end

