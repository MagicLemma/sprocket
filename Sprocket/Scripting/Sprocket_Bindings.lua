function GetForwardsDir(entity)
    local x, y, z = Lua_GetForwardsDir(entity)
    return Vec3(x, y, z)
end

function GetRightDir(entity)
    local x, y, z = Lua_GetRightDir(entity)
    return Vec3(x, y, z)
end

function SetLookAt(entity, pos, target)
    Lua_SetLookAt(entity, pos.x, pos.y, pos.z, target.x, target.y, target.z)
end

-- An iterator for looping over all entities in the associated scene. Under the
-- hood, this is done as a fast iteration, so should not be used for adding and
-- removing entities.
Scene = Class(function(self)
end)

function Scene:Each()
    local generator = FastIteration_New()
    local iter = FastIteration_IterStart(generator)

    return function()
        if FastIteration_IterValid(generator, iter) then
            local entity = FastIteration_IterGet(iter)
            FastIteration_IterNext(iter)
            return entity
        else
            FastIteration_Delete(generator)
        end
    end
end