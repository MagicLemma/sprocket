function Init() end

function OnUpdate(dt)
    local pos = GetPosition()

    local f = GetForwardsDir()
    f.y = 0
    f = Normalised(f)

    local speed = 10 * dt

    local r = GetRightDir()

    if IsKeyDown(KEYBOARD_W) then
        pos = pos + speed * f
    end
    if IsKeyDown(KEYBOARD_S) then
        pos = pos - speed * f
    end
    if IsKeyDown(KEYBOARD_D) then
        pos = pos + speed * r
    end
    if IsKeyDown(KEYBOARD_A) then
        pos = pos - speed * r
    end
    if IsKeyDown(KEYBOARD_SPACE) then
        pos.y = pos.y + speed
    end
    if IsKeyDown(KEYBOARD_LSHIFT) then
        pos.y = pos.y - speed
    end

    local dx, dy = GetMouseOffset()
    RotateY(-10 * dx)

    local camera = GetCameraComponent()
    camera.pitch = Clamp(camera.pitch - 0.15 * dy, -89, 89)
    SetCameraComponent(camera)
    
    SetPosition(pos)
end

function OnMouseButtonPressedEvent(consumed, button, action, mods) end

function OnMouseScrolledEvent(consumed, xOffset, yOffset) end

function OnWindowResizeEvent(consumed, width, height) end
