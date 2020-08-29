function Init(entity)
    TIME = 0.0
end

function OnUpdate(entity, dt)
    TIME = TIME + dt

    light = GetLightComponent(entity)
    light.brightness = 15 * math.sin(10 * TIME) + 15
    SetLightComponent(entity, light)
end