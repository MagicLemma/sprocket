function Init(entity)
    ENTITY = entity
    FLIP = true
end

function OnUpdate(entity, dt) end

function ToggleLight(entity)
    local light = GetLightComponent(entity)
    if FLIP then
        light.colour = Vec3(0, 1, 0)
    else
        light.colour = Vec3(1, 0, 0)
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