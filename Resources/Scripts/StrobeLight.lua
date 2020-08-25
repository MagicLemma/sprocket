function Init()
    TIME = 0.0
end

function OnUpdate(dt)
    TIME = TIME + dt

    light = GetLightComponent()
    light.brightness = math.sin(10 * TIME)
    SetLightComponent(light)
end