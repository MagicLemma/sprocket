function Init()
    DISTANCE = 8
    MOVEMENT_SPEED = 10
    ROTATION_SPEED = 90

    ABS_VERT = nil
    ABS_VERT_LOW = 2
    ABS_VERT_HIGH = 10

    -- Target
    X = 0
    Y = 0
    Z = 0

    HORIZ = 0 -- Parametrized yaw

    -- Projection matrix setup
    ASPECT_RATIO = 16 / 9
    FOV = 70
    NEAR_PLANE = 0.1
    FAR_PLANE = 1000
    SetPerspectiveCamera(ASPECT_RATIO, FOV, NEAR_PLANE, FAR_PLANE)
end

function OnUpdate(dt)
    local x, y, z = GetPosition()

    if ABS_VERT == nil then
        ABS_VERT = y
    end

    local horizSpeed = ROTATION_SPEED * dt
    local moveSpeed = MOVEMENT_SPEED * dt

    -- forwards vector
    local fx = X - x
    local fy = Y - y
    local fz = Z - z

    -- get horizonal component
    fy = 0
    fx, fy, fz = Normalise3(fx, fy, fz)

    -- right vector
    local rx, ry, rz = Cross(fx, fy, fz, 0, 1, 0)

    if IsKeyDown(KEYBOARD_W) then
        X = X + moveSpeed * fx
        Z = Z + moveSpeed * fz
    end
    if IsKeyDown(KEYBOARD_S) then
        X = X - moveSpeed * fx
        Z = Z - moveSpeed * fz
    end
    if IsKeyDown(KEYBOARD_D) then
        X = X + moveSpeed * rx
        Z = Z + moveSpeed * rz
    end
    if IsKeyDown(KEYBOARD_A) then
        X = X - moveSpeed * rx
        Z = Z - moveSpeed * rz
    end

    if IsKeyDown(KEYBOARD_E) then
        HORIZ = HORIZ - horizSpeed
    end
    if IsKeyDown(KEYBOARD_Q) then
        HORIZ = HORIZ + horizSpeed
    end

    x = X + DISTANCE * math.cos(math.rad(HORIZ))
    z = Z + DISTANCE * math.sin(math.rad(HORIZ))

    if y ~= ABS_VERT then
        local distance = ABS_VERT - y
        y = y + distance * 0.1
    end

    SetLookAt(x, y, z, X, Y, Z)
end

function OnMouseButtonPressedEvent(consumed, button, action, mods) end

function OnMouseScrolledEvent(consumed, xOffset, yOffset)
    if consumed then return false end
        -- Don't react to consumed events

    if ABS_VERT == nil then return false end
        -- If we receive an event before update, just ignore it

    ABS_VERT = Clamp(ABS_VERT - yOffset, ABS_VERT_LOW, ABS_VERT_HIGH)
    return true
end

function OnWindowResizeEvent(consumed, width, height)
    ASPECT_RATIO = width / height
    SetPerspectiveCamera(ASPECT_RATIO, FOV, NEAR_PLANE, FAR_PLANE)
    return false
end