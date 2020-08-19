function GetPosition()
    local x, y, z = Lua_GetPosition()
    return Vec3(x, y, z)
end

function GetForwardsDir()
    local x, y, z = Lua_GetForwardsDir()
    return Vec3(x, y, z)
end

function GetRightDir()
    local x, y, z = Lua_GetRightDir()
    return Vec3(x, y, z)
end

function SetPosition(vec)
    Lua_SetPosition(vec.x, vec.y, vec.z)
end

function SetLookAt(pos, target)
    Lua_SetLookAt(pos.x, pos.y, pos.z, target.x, target.y, target.z)
end