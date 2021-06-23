function Init(entity)
    ENTITY = entity
    FLIP = true
end

function OnUpdate(entity, dt)
    local singleton = entity_singleton()
    local cs = GetCollisionSingleton(singleton)
    for k, collision in pairs(cs.collisions) do
        local left = collision[1]
        local right = collision[2]
        OnCollisionEvent(entity_from_id(left), entity_from_id(right))
    end
end

function ToggleLight(entity)
    local light = GetLightComponent(entity)
    if FLIP then
        light.colour = vec3.new(0, 1, 0)
    else
        light.colour = vec3.new(1, 0, 0)
    end
    FLIP = not FLIP
    SetLightComponent(entity, light)
end


function OnCollisionEvent(entity1, entity2)
    if AreEntitiesEqual(ENTITY, entity1) or AreEntitiesEqual(ENTITY, entity2) then
        ToggleLight(ENTITY)
        return true
    end
    return false
end