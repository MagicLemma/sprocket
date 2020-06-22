local Vec3_meta = {}

Vec3_meta.__index = Vec3_meta

Vec3_meta.__add = function(a, b)
    return Vec3(a.x + b.x, a.y + b.y, a.z + b.z)
end

Vec3_meta.__sub = function(a, b)
    return Vec3(a.x - b.x, a.y - b.y, a.z - b.z)
end

Vec3_meta.__mul = function(a, b)
    if type(a) == 'number' then
        return Vec3(a * b.x, a * b.y, a * b.z)
    elseif type(b) == 'number' then
        return Vec3(a.x * b, a.y * b, a.z * b)
    else
        return Vec3(a.x * b.x, a.y * b.y, a.z * b.z)
    end
end

function Vec3(x, y, z)
    return setmetatable({x=x, y=y, z=z}, Vec3_meta)
end

function Mag(vector)
    return math.sqrt(Dot(vector, vector))
end

function Dot(a, b)
    return a.x * b.x + a.y * b.y + a.z * b.z
end

function Cross(a, b)
    return Vec3(
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    )
end

function Normalised(vector)
    local mag = Mag(vector)
    return Vec3(vector.x / mag, vector.y / mag, vector.z / mag)
end