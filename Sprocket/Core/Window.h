#pragma once
#include "Event.h"
#include "Maths.h"

#include <string>
#include <functional>
#include <memory>
#include <map>

namespace Sprocket {

using EventCallback = std::function<void(Event&)>;

struct WindowImpl;

struct WindowData
{
	std::string  name;
	unsigned int width;
	unsigned int height;

	bool running = true;
	bool focused = true;

	EventCallback callback = [](Event&) {};
};

class Window
{
	std::shared_ptr<WindowImpl> d_impl;
	WindowData                  d_data;

	Maths::vec3 d_clearColour;

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

	void OnUpdate();
	void Clear();

	void SetClearColour(const Maths::vec3& colour);

	// Getters
	unsigned int Height() const { return d_data.height; }
	unsigned int Width() const { return d_data.width; }
	float AspectRatio() const { return (float)d_data.width / (float)d_data.height; }
	bool Running() const { return d_data.running; }
	bool Focused() const { return d_data.focused; }

	// Callback Utilities
	void SetCallback(EventCallback cb);
		// Sets the callback to be given events. If a callback is already
		// provided, it is overwritten.

	// Window Utilities
	void SetWindowName(const std::string& name);
	void SetCursorVisibility(bool visibility);

	// Clipboard Utilities
	const char* GetClipboardData();
	void SetClipboardData(const std::string& text);

	// Low level Utilities
	void* NativeWindow() const;
};

}