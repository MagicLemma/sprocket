-- GENERATED FILE
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

function AddNameComponent(entity, c)
    Lua_AddNameComponent(entity, c.name)
end


TransformComponent = Class(function(self, position, scale)
    self.position = position
    self.scale = scale
end)

function GetTransformComponent(entity)
    x0, x1, x2, x3, x4, x5 = Lua_GetTransformComponent(entity)
    return TransformComponent(Vec3(x0, x1, x2), Vec3(x3, x4, x5))
end

function SetTransformComponent(entity, c)
    Lua_SetTransformComponent(entity, c.position.x, c.position.y, c.position.z, c.scale.x, c.scale.y, c.scale.z)
end

function AddTransformComponent(entity, c)
    Lua_AddTransformComponent(entity, c.position.x, c.position.y, c.position.z, c.scale.x, c.scale.y, c.scale.z)
end


ModelComponent = Class(function(self, mesh, material)
    self.mesh = mesh
    self.material = material
end)

function GetModelComponent(entity)
    x0, x1 = Lua_GetModelComponent(entity)
    return ModelComponent(x0, x1)
end

function SetModelComponent(entity, c)
    Lua_SetModelComponent(entity, c.mesh, c.material)
end

function AddModelComponent(entity, c)
    Lua_AddModelComponent(entity, c.mesh, c.material)
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

function AddRigidBody3DComponent(entity, c)
    Lua_AddRigidBody3DComponent(entity, c.velocity.x, c.velocity.y, c.velocity.z, c.gravity, c.frozen, c.bounciness, c.frictionCoefficient, c.rollingResistance, c.force.x, c.force.y, c.force.z, c.onFloor)
end


BoxCollider3DComponent = Class(function(self, position, mass, halfExtents, applyScale)
    self.position = position
    self.mass = mass
    self.halfExtents = halfExtents
    self.applyScale = applyScale
end)

function GetBoxCollider3DComponent(entity)
    x0, x1, x2, x3, x4, x5, x6, x7 = Lua_GetBoxCollider3DComponent(entity)
    return BoxCollider3DComponent(Vec3(x0, x1, x2), x3, Vec3(x4, x5, x6), x7)
end

function SetBoxCollider3DComponent(entity, c)
    Lua_SetBoxCollider3DComponent(entity, c.position.x, c.position.y, c.position.z, c.mass, c.halfExtents.x, c.halfExtents.y, c.halfExtents.z, c.applyScale)
end

function AddBoxCollider3DComponent(entity, c)
    Lua_AddBoxCollider3DComponent(entity, c.position.x, c.position.y, c.position.z, c.mass, c.halfExtents.x, c.halfExtents.y, c.halfExtents.z, c.applyScale)
end


SphereCollider3DComponent = Class(function(self, position, mass, radius)
    self.position = position
    self.mass = mass
    self.radius = radius
end)

function GetSphereCollider3DComponent(entity)
    x0, x1, x2, x3, x4 = Lua_GetSphereCollider3DComponent(entity)
    return SphereCollider3DComponent(Vec3(x0, x1, x2), x3, x4)
end

function SetSphereCollider3DComponent(entity, c)
    Lua_SetSphereCollider3DComponent(entity, c.position.x, c.position.y, c.position.z, c.mass, c.radius)
end

function AddSphereCollider3DComponent(entity, c)
    Lua_AddSphereCollider3DComponent(entity, c.position.x, c.position.y, c.position.z, c.mass, c.radius)
end


CapsuleCollider3DComponent = Class(function(self, position, mass, radius, height)
    self.position = position
    self.mass = mass
    self.radius = radius
    self.height = height
end)

function GetCapsuleCollider3DComponent(entity)
    x0, x1, x2, x3, x4, x5 = Lua_GetCapsuleCollider3DComponent(entity)
    return CapsuleCollider3DComponent(Vec3(x0, x1, x2), x3, x4, x5)
end

function SetCapsuleCollider3DComponent(entity, c)
    Lua_SetCapsuleCollider3DComponent(entity, c.position.x, c.position.y, c.position.z, c.mass, c.radius, c.height)
end

function AddCapsuleCollider3DComponent(entity, c)
    Lua_AddCapsuleCollider3DComponent(entity, c.position.x, c.position.y, c.position.z, c.mass, c.radius, c.height)
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

function AddCameraComponent(entity, c)
    Lua_AddCameraComponent(entity, c.fov, c.pitch)
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

function AddSelectComponent(entity, c)
    Lua_AddSelectComponent(entity, c.selected, c.hovered)
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

function AddPathComponent(entity, c)
    Lua_AddPathComponent(entity, c.speed)
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

function AddGridComponent(entity, c)
    Lua_AddGridComponent(entity, c.x, c.z)
end


LightComponent = Class(function(self, colour, brightness)
    self.colour = colour
    self.brightness = brightness
end)

function GetLightComponent(entity)
    x0, x1, x2, x3 = Lua_GetLightComponent(entity)
    return LightComponent(Vec3(x0, x1, x2), x3)
end

function SetLightComponent(entity, c)
    Lua_SetLightComponent(entity, c.colour.x, c.colour.y, c.colour.z, c.brightness)
end

function AddLightComponent(entity, c)
    Lua_AddLightComponent(entity, c.colour.x, c.colour.y, c.colour.z, c.brightness)
end


ParticleComponent = Class(function(self, interval, velocity, velocityNoise, acceleration, scale, life)
    self.interval = interval
    self.velocity = velocity
    self.velocityNoise = velocityNoise
    self.acceleration = acceleration
    self.scale = scale
    self.life = life
end)

function GetParticleComponent(entity)
    x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11 = Lua_GetParticleComponent(entity)
    return ParticleComponent(x0, Vec3(x1, x2, x3), x4, Vec3(x5, x6, x7), Vec3(x8, x9, x10), x11)
end

function SetParticleComponent(entity, c)
    Lua_SetParticleComponent(entity, c.interval, c.velocity.x, c.velocity.y, c.velocity.z, c.velocityNoise, c.acceleration.x, c.acceleration.y, c.acceleration.z, c.scale.x, c.scale.y, c.scale.z, c.life)
end

function AddParticleComponent(entity, c)
    Lua_AddParticleComponent(entity, c.interval, c.velocity.x, c.velocity.y, c.velocity.z, c.velocityNoise, c.acceleration.x, c.acceleration.y, c.acceleration.z, c.scale.x, c.scale.y, c.scale.z, c.life)
end


AnimationComponent = Class(function(self, name, time)
    self.name = name
    self.time = time
end)

function GetAnimationComponent(entity)
    x0, x1 = Lua_GetAnimationComponent(entity)
    return AnimationComponent(x0, x1)
end

function SetAnimationComponent(entity, c)
    Lua_SetAnimationComponent(entity, c.name, c.time)
end

function AddAnimationComponent(entity, c)
    Lua_AddAnimationComponent(entity, c.name, c.time)
end


