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