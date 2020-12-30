function Init(entity)
    TIME = 0.0
    EXPLODED = false
end

function OnUpdate(entity, dt)
    TIME = TIME + dt

    if TIME > 3 and EXPLODED == false then
        print("Boom")
        for entity in AllEntities() do
            if HasRigidBody3DComponent(entity) and not HasCameraComponent(entity) then
                local rbc = GetRigidBody3DComponent(entity)
                if rbc.frozen == false then
                    rbc.velocity.y = rbc.velocity.y + 10
                end
                SetRigidBody3DComponent(entity, rbc)
            end
        end
        EXPLODED = true
        DeleteEntity(entity)
    end

end