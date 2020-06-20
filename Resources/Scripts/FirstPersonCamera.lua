function Normalise(x, y)
    local size = math.sqrt(x * x + y * y)
    return x/size, y/size
end

function Init()
    ASPECT_RATIO = 16 / 9
    FOV = 70
    NEAR_PLANE = 0.1
    FAR_PLANE = 1000
    SetPerspectiveCamera(ASPECT_RATIO, FOV, NEAR_PLANE, FAR_PLANE)
end

function OnUpdate(dt)
    local x, y, z = GetPosition()

    local fx, fy, fz = GetForwardsDir()
    fx, fz = Normalise(fx, fz)

    local speed = 10 * dt

    local rx, ry, rz = GetRightDir()

    if IsKeyDown(KEYBOARD_W) then
        x = x + speed * fx
        z = z + speed * fz
    end
    if IsKeyDown(KEYBOARD_S) then
        x = x - speed * fx
        z = z - speed * fz
    end
    if IsKeyDown(KEYBOARD_D) then
        x = x + speed * rx
        z = z + speed * rz
    end
    if IsKeyDown(KEYBOARD_A) then
        x = x - speed * rx
        z = z - speed * rz
    end
    if IsKeyDown(KEYBOARD_SPACE) then
        y = y + speed
    end
    if IsKeyDown(KEYBOARD_LSHIFT) then
        y = y - speed
    end

    local dx, dy = GetMouseOffset()
    RotateY(-10 * dx)

    local pitch = GetPitch()
    pitch = pitch - 0.15 * dy
    if pitch > 89 then
        pitch = 89
    elseif pitch < -89 then
        pitch = -89
    end
    SetPitch(pitch)
    
    SetPosition(x, y, z)
end

function OnMouseButtonPressedEvent(consumed, button, action, mods) end

function OnMouseScrolledEvent(consumed, xOffset, yOffset) end

function OnWindowResizeEvent(consumed, width, height)
    ASPECT_RATIO = width / height
    SetPerspectiveCamera(ASPECT_RATIO, FOV, NEAR_PLANE, FAR_PLANE)
    return false
end
