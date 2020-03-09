#pragma once
#include "Events/Event.h"
#include "Utility/Maths.h"

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
    Z = 90,
    C = 67,
    Q = 81,
    E = 69,
    R = 82,
    F = 70,
    SPACE = 32,
    LSHIFT = 340,
    ESC = 256
};

using EventCallback = std::function<void(const Event&)>;

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

	std::map<std::string, EventCallback> d_callbacks;
		// A name -> callback map for anything that may be
		// interested in events. Other objects can register themselves
		// by providing callbacks of their own. Objects may also
		// deregister themselves to stop receiving events.

	Maths::vec2 d_mouseOffset;
		// Updated every frame. This is the mouse offset between the
		// current frame and the one before it.

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
	void registerCallback(const std::string& name, EventCallback cb);
		// Registers an additional callback with the given name. If a
		// callback for the given name is already provided, it is overwritten.

	void deregisterCallback(const std::string& name);
		// Removes the callback corresponding to the given name. If no
		// callback exists for the given name, this is a noop.

	// Window Utilities
	void setCursorVisibility(bool visibility);
	float getTime();

	// Keyboard/Mouse Utilities
	bool isKeyDown(int key);
	bool isMouseButtonDown(int button);
	Maths::vec2 getMousePos();
	Maths::vec2 getMouseOffset();
};

}