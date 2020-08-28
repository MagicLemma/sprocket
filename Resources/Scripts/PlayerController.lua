function Init(entity)
    local f = GetForwardsDir(entity)
    YAW = math.deg(-math.asin(f.x))

    TIME = 0.0

    SPAWN_POINT = Vec3(-11, 2, 3)
end

function OnUpdate(entity, dt)
    TIME = TIME + dt

    local dx = 0
    local dy = 0
    if TIME > 0.2 then
        dx, dy = GetMouseOffset()
    end
    
    YAW = YAW - dx * 0.15

    local camera = GetCameraComponent(entity)
    camera.pitch = Clamp(camera.pitch - 0.15 * dy, -89, 89)
    SetCameraComponent(entity, camera)

    MakeUpright(entity, math.rad(YAW))

    local cosYaw = math.cos(math.rad(YAW))
    local sinYaw = math.sin(math.rad(YAW))
    
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

    if IsKeyDown(KEYBOARD_Z) then
        local newEntity = NewEntity()
    end
end

function OnMouseButtonPressedEvent(consumed, button, action, mods) end

function OnMouseScrolledEvent(consumed, xOffset, yOffset) end

function OnWindowResizeEvent(consumed, width, height) end
