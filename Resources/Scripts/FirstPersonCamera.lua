function Init(entity) end

function OnUpdate(entity, dt)
    local f = GetForwardsDir(entity)
    f.y = 0
    f = Normalised(f)
    
    local speed = 10 * dt
    local r = GetRightDir(entity)
    
    local transform = GetTransform3DComponent(entity)
    local new_pos = transform.position
    if IsKeyDown(Keyboard.W) then new_pos = new_pos + speed * f end
    if IsKeyDown(Keyboard.S) then new_pos = new_pos - speed * f end
    if IsKeyDown(Keyboard.D) then new_pos = new_pos + speed * r end
    if IsKeyDown(Keyboard.A) then new_pos = new_pos - speed * r end
    if IsKeyDown(Keyboard.SPACE) then new_pos.y = new_pos.y + speed end
    if IsKeyDown(Keyboard.LSHIFT) then new_pos.y = new_pos.y - speed end
    transform.position = new_pos
    SetTransform3DComponent(entity, transform)

    local mouse = GetMouseOffset()
    RotateY(entity, 10 * mouse.x)

    local camera = GetCameraComponent(entity)
    camera.pitch = Clamp(camera.pitch - 0.15 * mouse.y, -89, 89)
    SetCameraComponent(entity, camera)
end
