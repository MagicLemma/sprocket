Scene = Class(function(self)
    -- TODO: Generalise this so that is doesn't require the __scene__ global
end)

-- An iterator for looping over all entities in the associated scene. Under the
-- hood, this is done as a fast iteration, so should not be used for adding and
-- removing entities.
function Scene:Each()
    local generator = Each_New()
    local iter = Each_Iter_Start(generator)

    return function()
        if Each_Iter_Valid(generator, iter) then
            local entity = Each_Iter_Get(iter)
            Each_Iter_Next(iter)
            return entity
        else
            Each_Delete(generator)
        end
    end
end