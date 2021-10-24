function init(entity)
    DISTANCE = 8
    MOVEMENT_SPEED = 10
    ROTATION_SPEED = 90

    ABS_VERT = nil
    ABS_VERT_LOW = 2
    ABS_VERT_HIGH = 10

    -- Target
    TARGET = vec3.new(0, 0, 0)

    HORIZ = 0 -- Parametrized yaw
end

function on_update(entity, dt)
    local pos = GetTransform3DComponent(entity).position

    if ABS_VERT == nil then ABS_VERT = pos.y end

    local horizSpeed = ROTATION_SPEED * dt
    local moveSpeed = MOVEMENT_SPEED * dt

    -- forwards vec3.new
    local f = TARGET - pos

    f.y = 0
    f = Normalised(f)

    -- right vec3.new
    local up = vec3.new(0, 1, 0)
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

    local offset = GetMouseScrolled()
    if offset.y ~= 0 and ABS_VERT ~= nil then
        ABS_VERT = Clamp(ABS_VERT - offset.y, ABS_VERT_LOW, ABS_VERT_HIGH)
    end
end