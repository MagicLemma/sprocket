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
        y = y + speed * fy
        z = z + speed * fz
    end
    if IsKeyDown(KEYBOARD_S) then
        x = x - speed * fx
        y = y - speed * fy
        z = z - speed * fz
    end
    if IsKeyDown(KEYBOARD_D) then
        x = x + speed * rx
        y = y + speed * ry
        z = z + speed * rz
    end
    if IsKeyDown(KEYBOARD_A) then
        x = x - speed * rx
        y = y - speed * ry
        z = z - speed * rz
    end
    
    SetPosition(x, y, z)
end
