#pragma once
#include "Events/Event.h"

#include <string>
#include <functional>
#include <memory>
#include <vector>

namespace Sprocket {

using EventCallback = std::function<void(Event&)>;

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

	std::vector<EventCallback>  d_extraCallbacks;

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

	unsigned int height() const { return d_data.height; }
	unsigned int width() const { return d_data.width; }
	bool running() const { return d_data.running; }
	bool focused() const { return d_data.focused; }

	// Register additional objects that may want to act on events.
	void registerCallback(EventCallback cb) { d_extraCallbacks.push_back(cb); }

	void clear();
};

}