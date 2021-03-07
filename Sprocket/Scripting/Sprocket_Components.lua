-- GENERATED FILE
NameComponent = Class(function(self, name)
    self.name = name
end)

function GetNameComponent(entity)
            x0 = _GetNameComponent(entity)
            return NameComponent(x0)
        end
function SetNameComponent(entity, c)
            _SetNameComponent(entity, c.name)
        end
function AddNameComponent(entity, c)
            _AddNameComponent(entity, c.name)
        end

Transform2DComponent = Class(function(self, position, rotation, scale)
    self.position = position
    self.rotation = rotation
    self.scale = scale
end)

function GetTransform2DComponent(entity)
            x0, x1, x2, x3, x4 = _GetTransform2DComponent(entity)
            return Transform2DComponent(Vec3(x0, x1), x2, Vec3(x3, x4))
        end
function SetTransform2DComponent(entity, c)
            _SetTransform2DComponent(entity, c.position, c.rotation, c.scale)
        end
function AddTransform2DComponent(entity, c)
            _AddTransform2DComponent(entity, c.position, c.rotation, c.scale)
        end

Transform3DComponent = Class(function(self, position, scale)
    self.position = position
    self.scale = scale
end)

function GetTransform3DComponent(entity)
            x0, x1, x2, x3, x4, x5 = _GetTransform3DComponent(entity)
            return Transform3DComponent(Vec3(x0, x1, x2), Vec3(x3, x4, x5))
        end
function SetTransform3DComponent(entity, c)
            _SetTransform3DComponent(entity, c.position.x, c.position.y, c.position.z, c.scale.x, c.scale.y, c.scale.z)
        end
function AddTransform3DComponent(entity, c)
            _AddTransform3DComponent(entity, c.position.x, c.position.y, c.position.z, c.scale.x, c.scale.y, c.scale.z)
        end

ModelComponent = Class(function(self, mesh, material)
    self.mesh = mesh
    self.material = material
end)

function GetModelComponent(entity)
            x0, x1 = _GetModelComponent(entity)
            return ModelComponent(x0, x1)
        end
function SetModelComponent(entity, c)
            _SetModelComponent(entity, c.mesh, c.material)
        end
function AddModelComponent(entity, c)
            _AddModelComponent(entity, c.mesh, c.material)
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
            x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11 = _GetRigidBody3DComponent(entity)
            return RigidBody3DComponent(Vec3(x0, x1, x2), x3, x4, x5, x6, x7, Vec3(x8, x9, x10), x11)
        end
function SetRigidBody3DComponent(entity, c)
            _SetRigidBody3DComponent(entity, c.velocity.x, c.velocity.y, c.velocity.z, c.gravity, c.frozen, c.bounciness, c.frictionCoefficient, c.rollingResistance, c.force.x, c.force.y, c.force.z, c.onFloor)
        end
function AddRigidBody3DComponent(entity, c)
            _AddRigidBody3DComponent(entity, c.velocity.x, c.velocity.y, c.velocity.z, c.gravity, c.frozen, c.bounciness, c.frictionCoefficient, c.rollingResistance, c.force.x, c.force.y, c.force.z, c.onFloor)
        end

BoxCollider3DComponent = Class(function(self, position, mass, halfExtents, applyScale)
    self.position = position
    self.mass = mass
    self.halfExtents = halfExtents
    self.applyScale = applyScale
end)

function GetBoxCollider3DComponent(entity)
            x0, x1, x2, x3, x4, x5, x6, x7 = _GetBoxCollider3DComponent(entity)
            return BoxCollider3DComponent(Vec3(x0, x1, x2), x3, Vec3(x4, x5, x6), x7)
        end
function SetBoxCollider3DComponent(entity, c)
            _SetBoxCollider3DComponent(entity, c.position.x, c.position.y, c.position.z, c.mass, c.halfExtents.x, c.halfExtents.y, c.halfExtents.z, c.applyScale)
        end
function AddBoxCollider3DComponent(entity, c)
            _AddBoxCollider3DComponent(entity, c.position.x, c.position.y, c.position.z, c.mass, c.halfExtents.x, c.halfExtents.y, c.halfExtents.z, c.applyScale)
        end

SphereCollider3DComponent = Class(function(self, position, mass, radius)
    self.position = position
    self.mass = mass
    self.radius = radius
end)

function GetSphereCollider3DComponent(entity)
            x0, x1, x2, x3, x4 = _GetSphereCollider3DComponent(entity)
            return SphereCollider3DComponent(Vec3(x0, x1, x2), x3, x4)
        end
function SetSphereCollider3DComponent(entity, c)
            _SetSphereCollider3DComponent(entity, c.position.x, c.position.y, c.position.z, c.mass, c.radius)
        end
function AddSphereCollider3DComponent(entity, c)
            _AddSphereCollider3DComponent(entity, c.position.x, c.position.y, c.position.z, c.mass, c.radius)
        end

CapsuleCollider3DComponent = Class(function(self, position, mass, radius, height)
    self.position = position
    self.mass = mass
    self.radius = radius
    self.height = height
end)

function GetCapsuleCollider3DComponent(entity)
            x0, x1, x2, x3, x4, x5 = _GetCapsuleCollider3DComponent(entity)
            return CapsuleCollider3DComponent(Vec3(x0, x1, x2), x3, x4, x5)
        end
function SetCapsuleCollider3DComponent(entity, c)
            _SetCapsuleCollider3DComponent(entity, c.position.x, c.position.y, c.position.z, c.mass, c.radius, c.height)
        end
function AddCapsuleCollider3DComponent(entity, c)
            _AddCapsuleCollider3DComponent(entity, c.position.x, c.position.y, c.position.z, c.mass, c.radius, c.height)
        end

ScriptComponent = Class(function(self, script, active)
    self.script = script
    self.active = active
end)

function GetScriptComponent(entity)
            x0, x1 = _GetScriptComponent(entity)
            return ScriptComponent(x0, x1)
        end
function SetScriptComponent(entity, c)
            _SetScriptComponent(entity, c.script, c.active)
        end
function AddScriptComponent(entity, c)
            _AddScriptComponent(entity, c.script, c.active)
        end

Camera3DComponent = Class(function(self, fov, pitch)
    self.fov = fov
    self.pitch = pitch
end)

function GetCamera3DComponent(entity)
            x0, x1 = _GetCamera3DComponent(entity)
            return Camera3DComponent(x0, x1)
        end
function SetCamera3DComponent(entity, c)
            _SetCamera3DComponent(entity, c.fov, c.pitch)
        end
function AddCamera3DComponent(entity, c)
            _AddCamera3DComponent(entity, c.fov, c.pitch)
        end

SelectComponent = Class(function(self, selected, hovered)
    self.selected = selected
    self.hovered = hovered
end)

function GetSelectComponent(entity)
            x0, x1 = _GetSelectComponent(entity)
            return SelectComponent(x0, x1)
        end
function SetSelectComponent(entity, c)
            _SetSelectComponent(entity, c.selected, c.hovered)
        end
function AddSelectComponent(entity, c)
            _AddSelectComponent(entity, c.selected, c.hovered)
        end

PathComponent = Class(function(self, speed)
    self.speed = speed
end)

function GetPathComponent(entity)
            x0 = _GetPathComponent(entity)
            return PathComponent(x0)
        end
function SetPathComponent(entity, c)
            _SetPathComponent(entity, c.speed)
        end
function AddPathComponent(entity, c)
            _AddPathComponent(entity, c.speed)
        end

GridComponent = Class(function(self, x, z)
    self.x = x
    self.z = z
end)

function GetGridComponent(entity)
            x0, x1 = _GetGridComponent(entity)
            return GridComponent(x0, x1)
        end
function SetGridComponent(entity, c)
            _SetGridComponent(entity, c.x, c.z)
        end
function AddGridComponent(entity, c)
            _AddGridComponent(entity, c.x, c.z)
        end

LightComponent = Class(function(self, colour, brightness)
    self.colour = colour
    self.brightness = brightness
end)

function GetLightComponent(entity)
            x0, x1, x2, x3 = _GetLightComponent(entity)
            return LightComponent(Vec3(x0, x1, x2), x3)
        end
function SetLightComponent(entity, c)
            _SetLightComponent(entity, c.colour.x, c.colour.y, c.colour.z, c.brightness)
        end
function AddLightComponent(entity, c)
            _AddLightComponent(entity, c.colour.x, c.colour.y, c.colour.z, c.brightness)
        end

SunComponent = Class(function(self, colour, brightness, direction, shadows)
    self.colour = colour
    self.brightness = brightness
    self.direction = direction
    self.shadows = shadows
end)

function GetSunComponent(entity)
            x0, x1, x2, x3, x4, x5, x6, x7 = _GetSunComponent(entity)
            return SunComponent(Vec3(x0, x1, x2), x3, Vec3(x4, x5, x6), x7)
        end
function SetSunComponent(entity, c)
            _SetSunComponent(entity, c.colour.x, c.colour.y, c.colour.z, c.brightness, c.direction.x, c.direction.y, c.direction.z, c.shadows)
        end
function AddSunComponent(entity, c)
            _AddSunComponent(entity, c.colour.x, c.colour.y, c.colour.z, c.brightness, c.direction.x, c.direction.y, c.direction.z, c.shadows)
        end

AmbienceComponent = Class(function(self, colour, brightness)
    self.colour = colour
    self.brightness = brightness
end)

function GetAmbienceComponent(entity)
            x0, x1, x2, x3 = _GetAmbienceComponent(entity)
            return AmbienceComponent(Vec3(x0, x1, x2), x3)
        end
function SetAmbienceComponent(entity, c)
            _SetAmbienceComponent(entity, c.colour.x, c.colour.y, c.colour.z, c.brightness)
        end
function AddAmbienceComponent(entity, c)
            _AddAmbienceComponent(entity, c.colour.x, c.colour.y, c.colour.z, c.brightness)
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
            x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11 = _GetParticleComponent(entity)
            return ParticleComponent(x0, Vec3(x1, x2, x3), x4, Vec3(x5, x6, x7), Vec3(x8, x9, x10), x11)
        end
function SetParticleComponent(entity, c)
            _SetParticleComponent(entity, c.interval, c.velocity.x, c.velocity.y, c.velocity.z, c.velocityNoise, c.acceleration.x, c.acceleration.y, c.acceleration.z, c.scale.x, c.scale.y, c.scale.z, c.life)
        end
function AddParticleComponent(entity, c)
            _AddParticleComponent(entity, c.interval, c.velocity.x, c.velocity.y, c.velocity.z, c.velocityNoise, c.acceleration.x, c.acceleration.y, c.acceleration.z, c.scale.x, c.scale.y, c.scale.z, c.life)
        end

MeshAnimationComponent = Class(function(self, name, time, speed)
    self.name = name
    self.time = time
    self.speed = speed
end)

function GetMeshAnimationComponent(entity)
            x0, x1, x2 = _GetMeshAnimationComponent(entity)
            return MeshAnimationComponent(x0, x1, x2)
        end
function SetMeshAnimationComponent(entity, c)
            _SetMeshAnimationComponent(entity, c.name, c.time, c.speed)
        end
function AddMeshAnimationComponent(entity, c)
            _AddMeshAnimationComponent(entity, c.name, c.time, c.speed)
        end

