function Init()
    ASPECT_RATIO = 16 / 9
    FOV = 70
    NEAR_PLANE = 0.1
    FAR_PLANE = 1000
    SetPerspectiveCamera(ASPECT_RATIO, FOV, NEAR_PLANE, FAR_PLANE)
end

function OnUpdate(dt)
    local pos = SpktGetPosition()

    local f = SpktGetForwardsDir()
    f.x, f.z = Normalise2(f.x, f.z)

    local speed = 10 * dt

    local r = SpktGetRightDir()

    if IsKeyDown(KEYBOARD_W) then
        pos.x = pos.x + speed * f.x
        pos.z = pos.z + speed * f.z
    end
    if IsKeyDown(KEYBOARD_S) then
        pos.x = pos.x - speed * f.x
        pos.z = pos.z - speed * f.z
    end
    if IsKeyDown(KEYBOARD_D) then
        pos.x = pos.x + speed * r.x
        pos.z = pos.z + speed * r.z
    end
    if IsKeyDown(KEYBOARD_A) then
        pos.x = pos.x - speed * r.x
        pos.z = pos.z - speed * r.z
    end
    if IsKeyDown(KEYBOARD_SPACE) then
        pos.y = pos.y + speed
    end
    if IsKeyDown(KEYBOARD_LSHIFT) then
        pos.y = pos.y - speed
    end

    local dx, dy = GetMouseOffset()
    RotateY(-10 * dx)

    local pitch = GetPitch()
    pitch = Clamp(pitch - 0.15 * dy, -89, 89)
    SetPitch(pitch)
    
    SpktSetPosition(pos)
end

function OnMouseButtonPressedEvent(consumed, button, action, mods) end

function OnMouseScrolledEvent(consumed, xOffset, yOffset) end

function OnWindowResizeEvent(consumed, width, height)
    ASPECT_RATIO = width / height
    SetPerspectiveCamera(ASPECT_RATIO, FOV, NEAR_PLANE, FAR_PLANE)
    return false
end
