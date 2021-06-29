function Init(entity)
    FLIP = true
end

function OnUpdate(entity, dt)
    for event in CollisionEventView() do
        local collision = GetCollisionEvent(event)
        local a = entity_from_id(collision.entity_a)
        local b = entity_from_id(collision.entity_b)
        if AreEntitiesEqual(entity, a) or AreEntitiesEqual(entity, b) then
            toggle_light(entity)
        end
    end
end

function toggle_light(entity)
    local light = GetLightComponent(entity)
    if FLIP then
        light.colour = vec3.new(0, 1, 0)
    else
        light.colour = vec3.new(1, 0, 0)
    end
    FLIP = not FLIP
    SetLightComponent(entity, light)
end