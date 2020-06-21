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
    local fx = TARGET.x - pos.x
    local fy = TARGET.y - pos.y
    local fz = TARGET.z - pos.z

    -- get horizonal component
    fy = 0
    fx, fy, fz = Normalise3(fx, fy, fz)

    -- right vector
    local r = Cross({fx, fy, fz}, {0, 1, 0})

    if IsKeyDown(KEYBOARD_W) then
        TARGET.x = TARGET.x + moveSpeed * fx
        TARGET.z = TARGET.z + moveSpeed * fz
    end
    if IsKeyDown(KEYBOARD_S) then
        TARGET.x = TARGET.x - moveSpeed * fx
        TARGET.z = TARGET.z - moveSpeed * fz
    end
    if IsKeyDown(KEYBOARD_D) then
        TARGET.x = TARGET.x + moveSpeed * r.x
        TARGET.z = TARGET.z + moveSpeed * r.z
    end
    if IsKeyDown(KEYBOARD_A) then
        TARGET.x = TARGET.x - moveSpeed * r.x
        TARGET.z = TARGET.z - moveSpeed * r.z
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
        pos.y = pos.y + distance * 0.1
    end

    Lua_SetLookAt(pos.x, pos.y, pos.z, TARGET.x, TARGET.y, TARGET.z)
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
    --SetPerspectiveCamera(ASPECT_RATIO, FOV, NEAR_PLANE, FAR_PLANE)
    return false
end