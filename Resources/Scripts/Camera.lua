function Normalise(x, y)
    local size = math.sqrt(x * x + y * y)
    return x/size, y/size
end

function OnUpdate(dt)
    local x, y, z = GetPosition()

    local fx, fy, fz = GetForwardsDir()
    fx, fz = Normalise(fx, fz)
    fy = 0

    print(fx, fy, fz)

    local speed = 10 * dt

    local rx, ry, rz = GetRightDir()

    if IsKeyDown(KEYBOARD_C) then
        x = x + speed * fx
        y = y + speed * fy
        z = z + speed * fz
    end
    if IsKeyDown(KEYBOARD_X) then
        x = x - speed * fx
        y = y - speed * fy
        z = z - speed * fz
    end
    if IsKeyDown(KEYBOARD_V) then
        x = x + speed * rx
        y = y + speed * ry
        z = z + speed * rz
    end
    if IsKeyDown(KEYBOARD_Z) then
        x = x - speed * rx
        y = y - speed * ry
        z = z - speed * rz
    end

    if IsKeyDown(KEYBOARD_R) then
        RotateY(1000 * dt)
    end
    if IsKeyDown(KEYBOARD_F) then
        RotateY(-1000 * dt)
    end
    
    SetPosition(x, y, z)
end
