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

HORIZ = 0

function OnUpdate(dt)
    local x, y, z = GetPosition()
    local fx, fy, fz = GetForwardsDir() -- Position to Target unit vector

    fy = 0
    fx, fy, fz = Normalise(fx, fy, fz)

    --local horizSpeed = ROTATION_SPEED * dt
    --local moveSpeed = MOVEMENT_SPEED * dt

    --local up = {0, 1, 0}
    --local rx, ry, rz = Cross(fx, fy, fz, 0, 1, 0)

    --if IsKeyDown(KEYBOARD_W) then
    --    TARGET[1] = TARGET[1] + speed * fx
    --    TARGET[3] = TARGET[3] + speed * fz
    --end
    --if IsKeyDown(KEYBOARD_S) then
    --    TARGET[1] = TARGET[1] - speed * fx
    --    TARGET[3] = TARGET[3] - speed * fz
    --end
    --if IsKeyDown(KEYBOARD_D) then
    --    TARGET[1] = TARGET[1] + speed * rx
    --    TARGET[3] = TARGET[3] + speed * rz
    --end
    --if IsKeyDown(KEYBOARD_A) then
    --    TARGET[1] = TARGET[1] - speed * rx
    --    TARGET[3] = TARGET[3] - speed * rz
    --end

    --x = TARGET[1] - DISTANCE * fx
    --y = TARGET[1] - DISTANCE * fy
    --z = TARGET[1] - DISTANCE * fz

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
    print("Update 3pc");
    SetPosition(x, y, z)
end

function OnMouseButtonPressedEvent(consumed, button, action, mods)
    print("Received ! button down event!");
end
