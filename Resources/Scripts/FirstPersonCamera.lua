function Init(entity) end

function OnUpdate(entity, dt)
    local f = GetForwardsDir(entity)
    f.y = 0
    f = Normalised(f)
    
    local speed = 10 * dt
    local r = GetRightDir(entity)
    
    local transform = GetTransformComponent(entity)
    local new_pos = transform.position
    if IsKeyDown(KEYBOARD_W) then new_pos = new_pos + speed * f end
    if IsKeyDown(KEYBOARD_S) then new_pos = new_pos - speed * f end
    if IsKeyDown(KEYBOARD_D) then new_pos = new_pos + speed * r end
    if IsKeyDown(KEYBOARD_A) then new_pos = new_pos - speed * r end
    if IsKeyDown(KEYBOARD_SPACE) then new_pos.y = new_pos.y + speed end
    if IsKeyDown(KEYBOARD_LSHIFT) then new_pos.y = new_pos.y - speed end
    transform.position = new_pos
    SetTransformComponent(entity, transform)

    local dx, dy = GetMouseOffset()
    RotateY(entity, 10 * dx)

    local camera = GetCameraComponent(entity)
    camera.pitch = Clamp(camera.pitch - 0.15 * dy, -89, 89)
    SetCameraComponent(entity, camera)
end
