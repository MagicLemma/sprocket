function Init()
    YAW = 0.0

    -- Projection matrix setup
    ASPECT_RATIO = 16 / 9
    FOV = 70
    NEAR_PLANE = 0.1
    FAR_PLANE = 1000
    SetPerspectiveCamera(ASPECT_RATIO, FOV, NEAR_PLANE, FAR_PLANE)
end

function OnUpdate(dt)
    local dx, dy = GetMouseOffset()
    
    YAW = YAW - dx * 0.15

    local pitch = GetPitch()
    pitch = Clamp(pitch - 0.15 * dy, -89, 89)
    SetPitch(pitch)

    MakeUpright(math.rad(YAW))

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

    local physics = GetPhysicsComponent()
    local dv = Vec3(0, 0, 0)

    if Mag(dir) > 0 or physics.onFloor then
        dv = dv + 3 * dir - physics.velocity
        dv.y = 0
    end

    if physics.onFloor and IsKeyDown(KEYBOARD_SPACE) then 
        dv.y = (6 - physics.velocity.y)
    end

    physics.force = physics.force + (physics.mass * dv)
    SetPhysicsComponent(physics)
end

function OnMouseButtonPressedEvent(consumed, button, action, mods) end

function OnMouseScrolledEvent(consumed, xOffset, yOffset) end

function OnWindowResizeEvent(consumed, width, height)
    ASPECT_RATIO = width / height
    SetPerspectiveCamera(ASPECT_RATIO, FOV, NEAR_PLANE, FAR_PLANE)
    return false
end
