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

function Normalise2(x, y)
    local size = math.sqrt(x * x + y * y)
    return x/size, y/size
end

function Normalise3(x, y, z)
    local size = math.sqrt(x * x + y * y + z * z)
    return x/size, y/size, z/size
end

function Cross(a1, a2, a3, b1, b2, b3)
    return a2*b3 - a3*b2, a3*b1 - a1*b3, a1*b2 - a2*b1
end

function Clamp(value, low, high)
    return math.min(high, math.max(value, low))
end