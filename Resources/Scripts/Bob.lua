function Init(entity)
    TIMER = 0
end

function OnUpdate(entity, dt)
    TIMER = TIMER + dt
    local pos = GetTransform3DComponent(entity)
    pos.position.y = 2 + math.sin(TIMER)
    SetTransform3DComponent(entity, pos)
end