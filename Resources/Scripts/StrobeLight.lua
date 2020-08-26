function Init()
    TIME = 0.0
end

function OnUpdate(dt)
    TIME = TIME + dt

    light = GetLightComponent()
    light.brightness = 15 * math.sin(10 * TIME) + 15
    SetLightComponent(light)
end