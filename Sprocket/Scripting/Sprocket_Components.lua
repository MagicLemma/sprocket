-- GENERATED FILE
TemporaryComponent = Class(function(self, )
end)

IMPL

NameComponent = Class(function(self, name)
    self.name = name
end)

IMPL

TransformComponent = Class(function(self, position, orientation, scale)
    self.position = position
    self.scale = scale
end)

IMPL

ModelComponent = Class(function(self, model, texture, shineDamper, reflectivity)
    self.model = model
    self.texture = texture
    self.shineDamper = shineDamper
    self.reflectivity = reflectivity
end)

IMPL

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

IMPL

BoxCollider3DComponent = Class(function(self, position, orientation, mass, halfExtents, applyScale)
    self.position = position
    self.mass = mass
    self.halfExtents = halfExtents
    self.applyScale = applyScale
end)

IMPL

SphereCollider3DComponent = Class(function(self, position, orientation, mass, radius)
    self.position = position
    self.mass = mass
    self.radius = radius
end)

IMPL

CapsuleCollider3DComponent = Class(function(self, position, orientation, mass, radius, height)
    self.position = position
    self.mass = mass
    self.radius = radius
    self.height = height
end)

IMPL

ScriptComponent = Class(function(self, script, active)
    self.script = script
    self.active = active
end)

IMPL

CameraComponent = Class(function(self, projection, fov, pitch)
    self.fov = fov
    self.pitch = pitch
end)

IMPL

SelectComponent = Class(function(self, selected, hovered)
    self.selected = selected
    self.hovered = hovered
end)

IMPL

PathComponent = Class(function(self, markers, speed)
    self.speed = speed
end)

IMPL

GridComponent = Class(function(self, x, z)
    self.x = x
    self.z = z
end)

IMPL

LightComponent = Class(function(self, colour, attenuation, brightness)
    self.colour = colour
    self.attenuation = attenuation
    self.brightness = brightness
end)

IMPL

ParticleComponent = Class(function(self, interval, velocity, velocityNoise, acceleration, scale, life, accumulator)
    self.interval = interval
    self.velocity = velocity
    self.velocityNoise = velocityNoise
    self.acceleration = acceleration
    self.scale = scale
    self.life = life
end)

IMPL

