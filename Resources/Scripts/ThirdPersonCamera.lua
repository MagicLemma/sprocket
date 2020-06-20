function Normalise(x, y, z)
    local size = math.sqrt(x * x + y * y + z * z)
    return x/size, y/size, z/size
end

function Cross(a1, a2, a3, b1, b2, b3)
    return a2*b3 - a3*b2, a3*b1 - a1*b3, a1*b2 - a2*b1
end

DISTANCE = 10
MOVEMENT_SPEED = 10
ROTATION_SPEED = 90

-- Target
X = 0
Y = 0
Z = 0

HORIZ = 0

function OnUpdate(dt)
    local x, y, z = GetPosition()

    local horizSpeed = ROTATION_SPEED * dt
    local moveSpeed = MOVEMENT_SPEED * dt

    -- forwards vector
    local fx = X - x
    local fy = Y - y
    local fz = Z - z

    -- get horizonal component
    fy = 0
    fx, fy, fz = Normalise(fx, fy, fz)

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
    SetLookAt(x, y, z, X, Y, Z)

    print("all good")
end

function OnMouseButtonPressedEvent(consumed, button, action, mods)
    print("Received ! button down event!");
end
