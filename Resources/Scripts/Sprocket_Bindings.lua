function GetPosition()
    local x, y, z = Lua_GetPosition()
    return Vec3(x, y, z)
end

function GetForwardsDir()
    local x, y, z = Lua_GetForwardsDir()
    return Vec3(x, y, z)
end

function GetRightDir()
    local x, y, z = Lua_GetRightDir()
    return Vec3(x, y, z)
end

function SetPosition(vec)
    Lua_SetPosition(vec.x, vec.y, vec.z)
end

function SetLookAt(pos, target)
    Lua_SetLookAt(pos.x, pos.y, pos.z, target.x, target.y, target.z)
end

PhysicsComponent = Class(function(self, velx, vely, velz, gravity, frozen, mass, bounciness, fric, roll, forcex, forcey, forcez, onFloor)
    self.velocity = Vec3(velx, vely, velz)
    self.gravity = gravity
    self.frozen = frozen
    self.mass = mass
    self.bounciness = bounciness
    self.frictionCoefficient = fric
    self.rollingResistance = roll
    self.force = Vec3(forcex, forcey, forcez)
    self.onFloor = onFloor
end)

function GetPhysicsComponent()
    velx, vely, velz, gravity, frozen, mass, bounciness, fric, roll, forcex, forcey, forcez, onFloor = Lua_GetPhysicsComponent()
    return PhysicsComponent(velx, vely, velz, gravity, frozen, mass, bounciness, fric, roll, forcex, forcey, forcez, onFloor)
end

function SetPhysicsComponent(pc)
    Lua_SetPhysicsComponent(pc.velocity.x, pc.velocity.y, pc.velocity.z, pc.gravity, pc.frozen, pc.mass, pc.bounciness, pc.frictionCoefficient, pc.rollingResistance, pc.force.x, pc.force.y, pc.force.z, pc.onFloor)
end