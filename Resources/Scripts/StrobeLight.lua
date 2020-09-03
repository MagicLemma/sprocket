function Init(entity)
    TIME = 0.0
end

function OnUpdate(entity, dt)
    TIME = TIME + dt

    light = GetLightComponent(entity)
    light.brightness = 5 * math.sin(10 * TIME)
    SetLightComponent(entity, light)
end