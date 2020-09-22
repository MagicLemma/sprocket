function Init(entity)
    TIMER = 0
end

function OnUpdate(entity, dt)
    TIMER = TIMER + dt
    local pos = GetTransformComponent(entity)
    pos.position.y = 2 + math.sin(TIMER)
    SetTransformComponent(entity, pos)
end