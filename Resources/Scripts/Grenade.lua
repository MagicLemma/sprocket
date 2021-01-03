function Init(entity)
    TIME = 0.0
end

function OnUpdate(entity, dt)
    TIME = TIME + dt

    local scene = Scene()

    if TIME > 3 then
        for e in scene:Each() do
            local pos = GetTransformComponent(entity).position
            if HasRigidBody3DComponent(e) then
                local rbc = GetRigidBody3DComponent(e)
                if rbc.frozen == false then
                    local e_pos = GetTransformComponent(e).position
                    if Mag(e_pos - pos) < 10 then
                        rbc.velocity = rbc.velocity + 5 * (e_pos - pos)
                    end
                end
                SetRigidBody3DComponent(e, rbc)
            end
        end
        DeleteEntity(entity)
    end
end