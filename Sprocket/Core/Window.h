#pragma once
#include "Events/Event.h"

#include <string>
#include <functional>
#include <memory>
#include <map>

namespace Sprocket {

using EventCallback = std::function<void(const Event&)>;

struct WindowImpl;

struct WindowData
{
	std::string   name;
	unsigned int  width;
	unsigned int  height;

	bool          running;
	bool          focused;

	EventCallback callback;
};

class Window
{
	std::shared_ptr<WindowImpl> d_impl;
	WindowData                  d_data;

	// Callbacks for window events.
	EventCallback                        d_keyboardCallback;
		// Callback specifically for the keyboard. This will only
		// be called for keyboard events.

	EventCallback                        d_mouseCallback;
		// Callback specifically for the mouse. This will only be
		// called for mouse events.

	std::map<std::string, EventCallback> d_extraCallbacks;
		// A name -> callback map for anything else that may be
		// interested in events. Other objects can register themselves
		// by providing callbacks of their own. Objects may also
		// deregister themselves to stop receiving events.

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

	// Getters
	unsigned int height() const { return d_data.height; }
	unsigned int width() const { return d_data.width; }
	float aspectRatio() const { return (float)d_data.width / (float)d_data.height; }
	bool running() const { return d_data.running; }
	bool focused() const { return d_data.focused; }

	// Callback Utilities
	void registerKeyboardCallback(EventCallback cb);
		// Register the callback for the Keyboard. This callback will only
		// be called with Keyboard events.

	void registerMouseCallback(EventCallback cb);
		// Register the callback for the mouse/ This willback will only be
		// called with Mouse events.

	void registerCallback(const std::string& name, EventCallback cb);
		// Registers an additional callback with the given name. If a
		// callback for the given name is already provided, it is overwritten.

	void deregisterCallback(const std::string& name);
		// Removes the callback corresponding to the given name. If no
		// callback exists for the given name, this is a noop.

	void setCursorVisibility(bool visibility);

	void clear();
};

}