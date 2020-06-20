function Normalise(x, y, z)
    local size = math.sqrt(x * x + y * y + z * z)
    return x/size, y/size, z/size
end

function Cross(a1, a2, a3, b1, b2, b3)
    return a2*b3 - a3b2, a3b1 - a1b3, a1b2 - a2b1
end

DISTANCE = 10
MOVEMENT_SPEED = 10
ROTATION_SPEED = 90
TARGET = {0, 0, 0}

X = 10
Y = 10
Z = 10

HORIZ = 0

function OnUpdate(dt)
    --local x, y, z = GetPosition()
    --local fx, fy, fz = GetForwardsDir() -- Position to Target unit vector
    --local rx, ry, rz = GetRightDir()

    local speed = 10 * dt
    --fy = 0
    --fx, fy, fz = Normalise(fx, fy, fz)

    --local horizSpeed = ROTATION_SPEED * dt
    --local moveSpeed = MOVEMENT_SPEED * dt

    --local up = {0, 1, 0}
    --local rx, ry, rz = Cross(fx, fy, fz, 0, 1, 0)

    if IsKeyDown(KEYBOARD_W) then
        Z = Z + speed * -1
    end
    if IsKeyDown(KEYBOARD_S) then
        Z = Z + speed * 1
    end
    if IsKeyDown(KEYBOARD_D) then
        X = X + speed * 1
    end
    if IsKeyDown(KEYBOARD_A) then
        X = X + speed * -1
    end

    SetLookAt(X, Y, Z, 0, 0, 0)
    --x = TARGET[1] - DISTANCE * fx
    --y = TARGET[2] - DISTANCE * fy
    --z = TARGET[3] - DISTANCE * fz

    --f IsKeyDown(KEYBOARD_W) then
    --   x = x + speed * fx
    --   z = z + speed * fz
    --nd
    --f IsKeyDown(KEYBOARD_S) then
    --   x = x - speed * fx
    --   z = z - speed * fz
    --nd
    --f IsKeyDown(KEYBOARD_D) then
    --   x = x + speed * rx
    --   z = z + speed * rz
    --nd
    --f IsKeyDown(KEYBOARD_A) then
    --   x = x - speed * rx
    --   z = z - speed * rz
    --nd

    --if IsKeyDown(KEYBOARD_E) then
    --    HORIZ = HORIZ - horizSpeed
    --end
    --if IsKeyDown(KEYBOARD_Q) then
    --    HORIZ = HORIZ + horizSpeed
    --end

    --local dx, dy = GetMouseOffset()
    --RotateY(-10 * dx)

    --local pitch = GetPitch()
    --pitch = pitch - 0.15 * dy
    --SetPitch(pitch)

    --SetPosition(x, y, z)
end

function OnMouseButtonPressedEvent(consumed, button, action, mods)
    print("Received ! button down event!");
end
