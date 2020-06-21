function Init()
    ASPECT_RATIO = 16 / 9
    FOV = 70
    NEAR_PLANE = 0.1
    FAR_PLANE = 1000
    SetPerspectiveCamera(ASPECT_RATIO, FOV, NEAR_PLANE, FAR_PLANE)
end

function OnUpdate(dt)
    local pos = GetPosition()

    local f = GetForwardsDir()
    
    f.y = 0
    f = f:Normalised()

    local speed = 10 * dt

    local r = GetRightDir()

    if IsKeyDown(KEYBOARD_W) then
        pos = pos + speed * f
    end
    if IsKeyDown(KEYBOARD_S) then
        pos = pos - speed * f
    end
    if IsKeyDown(KEYBOARD_D) then
        pos = pos + speed * r
    end
    if IsKeyDown(KEYBOARD_A) then
        pos = pos - speed * r
    end
    if IsKeyDown(KEYBOARD_SPACE) then
        pos.y = pos.y + speed
    end
    if IsKeyDown(KEYBOARD_LSHIFT) then
        pos.y = pos.y - speed
    end

    local dx, dy = GetMouseOffset()
    Lua_RotateY(-10 * dx)

    local pitch = GetPitch()
    pitch = Clamp(pitch - 0.15 * dy, -89, 89)
    SetPitch(pitch)
    
    SetPosition(pos)
end

function OnMouseButtonPressedEvent(consumed, button, action, mods) end

function OnMouseScrolledEvent(consumed, xOffset, yOffset) end

function OnWindowResizeEvent(consumed, width, height)
    ASPECT_RATIO = width / height
    SetPerspectiveCamera(ASPECT_RATIO, FOV, NEAR_PLANE, FAR_PLANE)
    return false
end
