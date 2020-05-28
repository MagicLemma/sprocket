#pragma once
#include "Event.h"
#include "Maths.h"

#include <string>
#include <functional>
#include <memory>
#include <map>

namespace Sprocket {

enum Mouse {
    LEFT = 0,
    RIGHT = 1,
    MIDDLE = 2
};

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

using EventCallback = std::function<void(Event&)>;

struct WindowImpl;

struct WindowData
{
	std::string   name;
	unsigned int  width;
	unsigned int  height;

	bool running   = true;
	bool focused   = true;

	EventCallback callback;
};

class Window
{
	std::shared_ptr<WindowImpl> d_impl;
	WindowData                  d_data;

	EventCallback d_callback;
		// A callback to be called with every event from the window.

private:
	// Deleted Constructors
	Window(Window&&) = delete;
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;

public:
	Window(const std::string& name = "Window",
	       unsigned int width = 1280, 
		   unsigned int height = 720);
	~Window();

	void onUpdate();
	void clear();

	// Getters
	unsigned int height() const { return d_data.height; }
	unsigned int width() const { return d_data.width; }
	float aspectRatio() const { return (float)d_data.width / (float)d_data.height; }
	bool running() const { return d_data.running; }
	bool focused() const { return d_data.focused; }

	// Callback Utilities
	void setCallback(EventCallback cb);
		// Sets the callback to be given events. If a callback is already
		// provided, it is overwritten.

	// Window Utilities
	void setCursorVisibility(bool visibility);

	// Clipboard Utilities
	const char* getClipboardData();
	void setClipboardData(const std::string& text);
};

}