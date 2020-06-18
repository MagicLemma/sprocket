function Normalise(x, y)
    local size = math.sqrt(x * x + y * y)
    return x/size, y/size
end

function OnUpdate(dt)
    local x, y, z = GetPosition()

    local fx, fy, fz = GetForwardsDir()
    fx, fz = Normalise(fx, fz)
    fy = 0

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
    
    SetPosition(x, y, z)
end
