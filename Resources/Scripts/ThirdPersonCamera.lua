function Init(entity)
    DISTANCE = 8
    MOVEMENT_SPEED = 10
    ROTATION_SPEED = 90

    ABS_VERT = nil
    ABS_VERT_LOW = 2
    ABS_VERT_HIGH = 10

    -- Target
    TARGET = Vec3(0, 0, 0)

    HORIZ = 0 -- Parametrized yaw
end

function OnUpdate(entity, dt)
    local pos = GetTransformComponent(entity).position

    if ABS_VERT == nil then ABS_VERT = pos.y end

    local horizSpeed = ROTATION_SPEED * dt
    local moveSpeed = MOVEMENT_SPEED * dt

    -- forwards Vec3
    local f = TARGET - pos

    f.y = 0
    f = Normalised(f)

    -- right Vec3
    local up = Vec3(0, 1, 0)
    local r = Cross(f, up)

    if IsKeyDown(Keyboard.W) then
        TARGET = TARGET + moveSpeed * f
    end
    if IsKeyDown(Keyboard.S) then
        TARGET = TARGET - moveSpeed * f
    end
    if IsKeyDown(Keyboard.D) then
        TARGET = TARGET + moveSpeed * r
    end
    if IsKeyDown(Keyboard.A) then
        TARGET = TARGET - moveSpeed * r
    end

    if IsKeyDown(Keyboard.E) then
        HORIZ = HORIZ - horizSpeed
    end
    if IsKeyDown(Keyboard.Q) then
        HORIZ = HORIZ + horizSpeed
    end

    pos.x = TARGET.x + DISTANCE * math.cos(math.rad(HORIZ))
    pos.z = TARGET.z + DISTANCE * math.sin(math.rad(HORIZ))

    if pos.y ~= ABS_VERT then
        local distance = ABS_VERT - pos.y
        pos.y = pos.y + distance * dt * 2
    end

    SetLookAt(entity, pos, TARGET)
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

function OnWindowResizeEvent(consumed, width, height) end