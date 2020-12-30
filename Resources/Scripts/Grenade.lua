function Init(entity)
    TIME = 0.0
end

function OnUpdate(entity, dt)
    TIME = TIME + dt

    if TIME > 3 then
        for entity in AllEntities() do
            if HasRigidBody3DComponent(entity) and not HasCameraComponent(entity) then
                local rbc = GetRigidBody3DComponent(entity)
                if rbc.frozen == false then
                    rbc.velocity.y = rbc.velocity.y + 10
                end
                SetRigidBody3DComponent(entity, rbc)
            end
        end
        DeleteEntity(entity)
    end
end