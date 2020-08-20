function Init() end

function OnUpdate(dt)
    local f = GetForwardsDir()
    f.y = 0
    f = Normalised(f)
    
    local speed = 10 * dt
    local r = GetRightDir()
    
    local transform = GetTransformComponent()
    local new_pos = transform.position
    if IsKeyDown(KEYBOARD_W) then
        new_pos = new_pos + speed * f
    end
    if IsKeyDown(KEYBOARD_S) then
        new_pos = new_pos - speed * f
    end
    if IsKeyDown(KEYBOARD_D) then
        new_pos = new_pos + speed * r
    end
    if IsKeyDown(KEYBOARD_A) then
        new_pos = new_pos - speed * r
    end
    if IsKeyDown(KEYBOARD_SPACE) then
        new_pos.y = new_pos.y + speed
    end
    if IsKeyDown(KEYBOARD_LSHIFT) then
        new_pos.y = new_pos.y - speed
    end
    transform.position = new_pos
    SetTransformComponent(transform)

    local dx, dy = GetMouseOffset()
    RotateY(-10 * dx)

    local camera = GetCameraComponent()
    camera.pitch = Clamp(camera.pitch - 0.15 * dy, -89, 89)
    SetCameraComponent(camera)
    
end

function OnMouseButtonPressedEvent(consumed, button, action, mods) end

function OnMouseScrolledEvent(consumed, xOffset, yOffset) end

function OnWindowResizeEvent(consumed, width, height) end
