function Init(entity)
    TIME = 0.0
    EXPLODED = false
end

function OnUpdate(entity, dt)
    TIME = TIME + dt

    if TIME > 5 and EXPLODED == false then
        print("Boom")
        EXPLODED = true
    end

end