function Init(entity)
    local f = GetForwardsDir(entity)
    YAW = -math.asin(f.x)

    TIME = 0.0

    SPAWN_POINT = Vec3(-11, 2, 3)
    ENTITY = entity
end

function OnUpdate(entity, dt)
    TIME = TIME + dt

    local rotateSpeed = 0.001

    local dx = 0
    local dy = 0
    if TIME > 0.2 then
        dx, dy = GetMouseOffset()
    end
    
    YAW = YAW - dx * rotateSpeed

    local camera = GetCamera3DComponent(entity)
    camera.pitch = Clamp(camera.pitch - rotateSpeed * dy, math.rad(-89), math.rad(89))
    SetCamera3DComponent(entity, camera)

    MakeUpright(entity, YAW)

    local cosYaw = math.cos(YAW)
    local sinYaw = math.sin(YAW)
    
    local forwards = Vec3(-sinYaw, 0, -cosYaw)
    local right = Vec3(cosYaw, 0, -sinYaw)

    local dir = Vec3(0, 0, 0)
    if IsKeyDown(Keyboard.W) then dir = dir + forwards end
    if IsKeyDown(Keyboard.S) then dir = dir - forwards end
    if IsKeyDown(Keyboard.D) then dir = dir + right end
    if IsKeyDown(Keyboard.A) then dir = dir - right end
    dir = Normalised(dir)

    local physics = GetRigidBody3DComponent(entity)
    local dv = Vec3(0, 0, 0)

    if Mag(dir) > 0 or physics.onFloor then
        dv = dv + 6 * dir - physics.velocity
        dv.y = 0
    end

    if physics.onFloor and IsKeyDown(Keyboard.SPACE) then 
        dv.y = (6 - physics.velocity.y)
    end

    physics.force = physics.force + (10 * dv) -- TODO: Replace 10 with mass
    SetRigidBody3DComponent(entity, physics)

    local transform = GetTransform3DComponent(entity)
    if transform.position.y < -1 then
        transform.position = SPAWN_POINT
        SetTransform3DComponent(entity, transform)
    end

end


function OnMouseButtonPressedEvent(consumed, button, action, mods)
    if consumed then return false end

    local newEntity = NewEntity()

    local dir = GetForwardsDir(ENTITY)
    local pos = GetTransform3DComponent(ENTITY).position
    local vel = GetRigidBody3DComponent(ENTITY).velocity
    
    local tc = Transform3DComponent(pos + dir, Vec3(0.1, 0.1, 0.1))
    AddTransform3DComponent(newEntity, tc)

    --local mc = ModelComponent(
    --    "Resources/Models/Sphere.obj", 
    --    "Resources/Materials/grey.yaml"
    --)
    --AddModelComponent(newEntity, mc)

    local rbc = RigidBody3DComponent(10 * dir + vel, true, false, 0.65, 0.3, 1, Vec3(0, 0, 0), false)
    AddRigidBody3DComponent(newEntity, rbc)

    local sc = SphereCollider3DComponent(Vec3(0, 0, 0), 20, 0.1)
    AddSphereCollider3DComponent(newEntity, sc)

    local lc = LightComponent(Vec3(1, 1, 1), 20)
    AddLightComponent(newEntity, lc)

    local sc = ScriptComponent("Resources/Scripts/Grenade.lua", true)
    AddScriptComponent(newEntity, sc)

    local pc = ParticleComponent(0.01, Vec3(0, 0, 0), 1, Vec3(0, 0, 0), Vec3(1, 1, 1), 1)
    AddParticleComponent(newEntity, pc)

    return true
end
