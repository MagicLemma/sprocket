#pragma once
#include "Events/Event.h"

#include <string>
#include <functional>
#include <memory>
#include <vector>

namespace Sprocket {

using EventCallback = std::function<void(Event&)>;

struct WindowImpl;

class Window
{
	std::shared_ptr<WindowImpl> d_impl;

	std::string   d_name;
	unsigned int  d_width;
	unsigned int  d_height;

	bool          d_running;
	bool          d_focused;

	std::vector<EventCallback> d_callbacks;

	// Deleted Constructors
	Window(Window&&) = delete;
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;

	void onEvent(Event& event);

public:
	Window(const std::string& name = "Window",
	       unsigned int width = 1280, 
		   unsigned int height = 720);
	~Window();

	void onUpdate();

	unsigned int height() const { return d_height; }
	void height(unsigned int newHeight) { d_height = newHeight; }

	unsigned int width() const { return d_width; }
	void width(unsigned int newWidth) { d_width = newWidth; }

	bool running() const { return d_running; }
	void running(bool isRunning) { d_running = isRunning; }

	bool focused() const { return d_focused; }
	void focused(bool isFocused) { d_focused = isFocused; }

	// Register additional objects that may want to act on events.
	void registerCallback(EventCallback cb) { d_callbacks.push_back(cb); }

	void clear();
};

}