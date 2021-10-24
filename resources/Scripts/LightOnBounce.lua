function init(entity)
    FLIP = true
end

function on_update(entity, dt)
    for event in view_CollisionEvent() do
        local collision = GetCollisionEvent(event)
        local a = collision.entity_a
        local b = collision.entity_b
        if entity == a or entity == b then
            toggle_light(entity)
        end
    end
end

function toggle_light(entity)
    local light = GetLightComponent(entity)
    if FLIP then
        light.colour = vec3.new(0, 1, 0)
    else
        light.colour = vec3.new(0, 0, 1)
    end
    FLIP = not FLIP
    SetLightComponent(entity, light)
end