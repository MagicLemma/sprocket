function Init()
    YAW = 0

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

    MakeUpright(YAW) -- TODO

    local cosYaw = math.cos(math.radians(YAW))
    local sinYaw = math.sin(math.radians(YAW))
    
    local forwards = Vec3(-sinYaw, 0, -cosYaw)
    local right = Vec3(cosYaw, 0, -sinYaw)

    local dir = Vec3(0, 0, 0)
    if IsKeyDown(KEYBOARD_W) then dir = dir + forwards end
    if IsKeyDown(KEYBOARD_S) then dir = dir - forwards end
    if IsKeyDown(KEYBOARD_D) then dir = dir + right end
    if IsKeyDown(KEYBOARD_A) then dir = dir - right end
    dir = Normalised(dir)

    local onFloor = IsOnFloor() -- TODO

    local dv = Vec3(0, 0, 0)

    if Mag(dv) ~= 0 or onFloor then
        dv = dv + 3 * dir - GetVelocity() -- TODO
        dy.y = 0 -- Only consider horizontal movement
    end

    if onFloor and IsKeyDown(KEYBOARD_SPACE) then 
        dv = dv + (6 - GetVelocity().y) * Vec3(0, 1, 0) 
    end

    ApplyForce(GetMass() * dv) -- TODO
end

function OnMouseButtonPressedEvent(consumed, button, action, mods) end

function OnMouseScrolledEvent(consumed, xOffset, yOffset) end

function OnWindowResizeEvent(consumed, width, height)
    ASPECT_RATIO = width / height
    SetPerspectiveCamera(ASPECT_RATIO, FOV, NEAR_PLANE, FAR_PLANE)
    return false
end
