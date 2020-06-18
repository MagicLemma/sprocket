
function OnUpdate(dt)
    local v_down = IsKeyDown(KEYBOARD_V)
    local c_down = IsKeyDown(67)

    local x, y, z = GetPosition()
    if (v_down) then
        y = y + dt
    end
    if (c_down) then
        y = y - dt
    end

    local dx, dy = GetMouseOffset()
    RotateY(dy)

    SetPosition(x, y, z)
end

function Hello()
    print("Hello!")
end