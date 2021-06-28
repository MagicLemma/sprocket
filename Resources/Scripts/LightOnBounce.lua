function Init(entity)
    ENTITY = entity
    FLIP = true
end

function OnUpdate(entity, dt)
    local singleton = entity_singleton()
    local cs = GetPhysicsSingleton(singleton)
    for k, collision in pairs(cs.collisions) do
        local left = entity_from_id(collision[1])
        local right = entity_from_id(collision[2])
        if AreEntitiesEqual(ENTITY, left) or AreEntitiesEqual(ENTITY, right) then
            toggle_light(ENTITY)
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