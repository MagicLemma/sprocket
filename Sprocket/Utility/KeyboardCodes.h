#pragma once

namespace Sprocket {

enum Keyboard {
    W = 87,
    A = 65,
    S = 83,
    D = 68,
    C = 67,
    Q = 81,
    E = 69,
    R = 82,
    F = 70,
	V = 86,
	X = 88,
	Y = 89,
	Z = 90,

    SPACE = 32,
	BACKSPACE = 259,
    LSHIFT = 340,
    ESC = 256,
	TAB = 258,
	ENTER = 257,

	KEYPAD_ENTER = 335,

	LEFT_ARROW = 263,
	RIGHT_ARROW = 262,
	UP_ARROW = 265,
	DOWN_ARROW = 264,

	PAGE_UP = 266,
	PAGE_DOWN = 267,

	HOME = 268,
	END = 269,
	INSERT = 260,
	DEL = 261
};

enum KeyModifier {
	SHIFT     = 0x0001,
	CTRL      = 0x0002,
	ALT       = 0x0004,
	SUPER     = 0x0008,
	CAPS_LOCK = 0x0010,
	NUM_LOCK  = 0x0020
};

}