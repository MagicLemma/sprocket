

function OnUpdate(dt)
    local yaw = 0

    if IsKeyDown(KEYBOARD_Q) then
        yaw = yaw + 10000 * dt
    end
    if IsKeyDown(KEYBOARD_E) then
        yaw = yaw - 10000 * dt
    end
    
    RotateY(yaw)
end
