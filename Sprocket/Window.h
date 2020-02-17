#pragma once
#include "Events/Event.h"

#include <string>
#include <functional>
#include <memory>

namespace Sprocket {

using EventCallback = std::function<void(Event&)>;

struct WindowImpl;

struct WindowData
{
	std::string   name;
	unsigned int  width;
	unsigned int  height;

	EventCallback callback;

	bool          focused = true;
		// True if the window is in focus or not. While the window is out
		// of focus, no Events will be processed (apart from WindowFocus
		// events).

	WindowData(
		const std::string& name   = "Sprocket Engine",
		int                width  = 1280,
		int                height = 720)
	: name(name), width(width), height(height) {}
};

class Window
{
	WindowData                  d_data;
	std::shared_ptr<WindowImpl> d_impl;

	// Deleted Constructors
	Window(Window&&) = delete;
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;

public:
	Window(const WindowData& properties = WindowData());
	~Window();

	void onUpdate();

	unsigned int height() const { return d_data.height; }
	unsigned int width() const { return d_data.width; }

	void setCallback(EventCallback cb) { d_data.callback = cb; }

	void clear();
};

}