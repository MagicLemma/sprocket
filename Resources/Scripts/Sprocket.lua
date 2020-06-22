KEYBOARD_W = 87
KEYBOARD_A = 65
KEYBOARD_S = 83
KEYBOARD_D = 68
KEYBOARD_C = 67
KEYBOARD_Q = 81
KEYBOARD_E = 69
KEYBOARD_R = 82
KEYBOARD_F = 70
KEYBOARD_V = 86
KEYBOARD_X = 88
KEYBOARD_Y = 89
KEYBOARD_Z = 90

KEYBOARD_SPACE = 32
KEYBOARD_BACKSPACE = 259
KEYBOARD_LSHIFT = 340
KEYBOARD_ESC = 256
KEYBOARD_TAB = 258
KEYBOARD_ENTER = 257

KEYBOARD_KEYPAD_ENTER = 335

KEYBOARD_LEFT_ARROW = 263
KEYBOARD_RIGHT_ARROW = 262
KEYBOARD_UP_ARROW = 265
KEYBOARD_DOWN_ARROW = 264

KEYBOARD_PAGE_UP = 266
KEYBOARD_PAGE_DOWN = 267

KEYBOARD_HOME = 268
KEYBOARD_END = 269
KEYBOARD_INSERT = 260
KEYBOARD_DEL = 26

function Clamp(value, low, high)
    return math.min(high, math.max(value, low))
end

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