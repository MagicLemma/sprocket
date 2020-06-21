function Init()
    DISTANCE = 8
    MOVEMENT_SPEED = 10
    ROTATION_SPEED = 90

    ABS_VERT = nil
    ABS_VERT_LOW = 2
    ABS_VERT_HIGH = 10

    -- Target
    TARGET = Vec3:New(0, 0, 0)

    HORIZ = 0 -- Parametrized yaw

    -- Projection matrix setup
    ASPECT_RATIO = 16 / 9
    FOV = 70
    NEAR_PLANE = 0.1
    FAR_PLANE = 1000
    SetPerspectiveCamera(ASPECT_RATIO, FOV, NEAR_PLANE, FAR_PLANE)
end

function OnUpdate(dt)
    local pos = GetPosition()

    if ABS_VERT == nil then ABS_VERT = pos.y end

    local horizSpeed = ROTATION_SPEED * dt
    local moveSpeed = MOVEMENT_SPEED * dt

    -- forwards vector
    local f = TARGET - pos

    f.y = 0
    f = f:Normalised()

    -- right vector
    local up = Vec3:New(0, 1, 0)
    local r = f:Cross(up)

    if IsKeyDown(KEYBOARD_W) then
        TARGET = TARGET + moveSpeed * f
    end
    if IsKeyDown(KEYBOARD_S) then
        TARGET = TARGET - moveSpeed * f
    end
    if IsKeyDown(KEYBOARD_D) then
        TARGET = TARGET + moveSpeed * r
    end
    if IsKeyDown(KEYBOARD_A) then
        TARGET = TARGET - moveSpeed * r
    end

    if IsKeyDown(KEYBOARD_E) then
        HORIZ = HORIZ - horizSpeed
    end
    if IsKeyDown(KEYBOARD_Q) then
        HORIZ = HORIZ + horizSpeed
    end

    pos.x = TARGET.x + DISTANCE * math.cos(math.rad(HORIZ))
    pos.z = TARGET.z + DISTANCE * math.sin(math.rad(HORIZ))

    if pos.y ~= ABS_VERT then
        local distance = ABS_VERT - pos.y
        pos.y = pos.y + distance * 0.01
    end

    SetLookAt(pos, TARGET)
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