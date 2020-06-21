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

local Sprocket = {}
Sprocket.__index = Sprocket

function Normalise2(x, y)
    local size = math.sqrt(x * x + y * y)
    return x/size, y/size
end

function Normalise3(x, y, z)
    local size = math.sqrt(x * x + y * y + z * z)
    return x/size, y/size, z/size
end

function Cross(a, b)
    return {
        x = a[2]*b[3] - a[3]*b[2],
        y = a[3]*b[1] - a[1]*b[3],
        z = a[1]*b[2] - a[2]*b[1]
    }
end

function Clamp(value, low, high)
    return math.min(high, math.max(value, low))
end

local Vec3 = {}
Vec3.__index = Vec3

function Vec3:New(x, y, z)
    return setmetatable({x=x, y=y, z=z}, Vec3)
end

function Vec3:Mag()
    return math.sqrt(self:Dot(self))
end

function Vec3:Dot(other)
    return self.x ^ 2 + self.y ^ 2 + self.z ^ 2
end

function Vec3.__add(a, b)
    return Vec3:New(a.x + b.x, a.y + b.y, a.z + b.z)
end

function Vec3.__sub(a, b)
    return Vec3:New(a.x - b.x, a.y - b.y, a.z - b.z)
end

function Vec3.__mul(a, b)
    if type(a) == 'number' then
        return Vec3:New(a * b.x, a * b.y, a * b.z)
    elseif type(b) == 'number' then
        return Vec3:New(a.x * b, a.y * b, a.z * b)
    else
        return Vec3:New(a.x * b.x, a.y * b.y, a.z * b.z)
    end
end

function Vec3:Normalised()
    local mag = self:Mag()
    return Vec3:New(self.x / mag, self.y / mag, self.z / mag)
end

function GetPosition()
    local x, y, z = Lua_GetPosition()
    return Vec3:New(x, y, z)
end

function GetForwardsDir()
    local x, y, z = Lua_GetForwardsDir()
    return Vec3:New(x, y, z)
end

function GetRightDir()
    local x, y, z = Lua_GetRightDir()
    return Vec3:New(x, y, z)
end

function SetPosition(vec)
    Lua_SetPosition(vec.x, vec.y, vec.z)
end