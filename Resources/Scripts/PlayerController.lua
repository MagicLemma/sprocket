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

    local camera = GetCameraComponent(entity)
    camera.pitch = Clamp(camera.pitch - rotateSpeed * dy, math.rad(-89), math.rad(89))
    SetCameraComponent(entity, camera)

    MakeUpright(entity, YAW)

    local cosYaw = math.cos(YAW)
    local sinYaw = math.sin(YAW)
    
    local forwards = Vec3(-sinYaw, 0, -cosYaw)
    local right = Vec3(cosYaw, 0, -sinYaw)

    local dir = Vec3(0, 0, 0)
    if IsKeyDown(KEYBOARD_W) then dir = dir + forwards end
    if IsKeyDown(KEYBOARD_S) then dir = dir - forwards end
    if IsKeyDown(KEYBOARD_D) then dir = dir + right end
    if IsKeyDown(KEYBOARD_A) then dir = dir - right end
    dir = Normalised(dir)

    local physics = GetRigidBody3DComponent(entity)
    local dv = Vec3(0, 0, 0)

    if Mag(dir) > 0 or physics.onFloor then
        dv = dv + 6 * dir - physics.velocity
        dv.y = 0
    end

    if physics.onFloor and IsKeyDown(KEYBOARD_SPACE) then 
        dv.y = (6 - physics.velocity.y)
    end

    physics.force = physics.force + (10 * dv) -- TODO: Replace 10 with mass
    SetRigidBody3DComponent(entity, physics)

    local transform = GetTransformComponent(entity)
    if transform.position.y < -1 then
        transform.position = SPAWN_POINT
        SetTransformComponent(entity, transform)
    end

end


function OnMouseButtonPressedEvent(consumed, button, action, mods)

    if consumed then return false end

    local newEntity = NewEntity()

    local sc = ScriptComponent("Resources/Scripts/Grenade.lua", true)
    AddScriptComponent(newEntity, sc)

    return true
end
