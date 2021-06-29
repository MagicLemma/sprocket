#pragma once
#include "Events.h"
#include "Maths.h"
#include "Types.h"

#include <string>
#include <functional>
#include <memory>
#include <map>

#ifndef _WIN32
#error "Sprocket currently only supports Windows"
#endif

namespace spkt {

using EventCallback = std::function<void(ev::Event&)>;

struct WindowImpl;

struct WindowData
{
	std::string name;
	u32 width;
	u32 height;

	bool fullscreen = false;
	bool running = true;
	bool focused = true;

	EventCallback callback = [](ev::Event&) {};
};

class Window
{
	std::unique_ptr<WindowImpl> d_impl;
	WindowData                  d_data;

	glm::vec3 d_clearColour;

	glm::vec2 d_mousePos;
	glm::vec2 d_mouseOffset;

private:
	// Deleted Constructors
	Window(Window&&) = delete;
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;

public:
	Window(const std::string& name = "Window", u32 width = 1280, u32 height = 720);
	~Window();

	void OnUpdate();
	void Clear();

	void SetClearColour(const glm::vec3& colour);

	// Getters
	u32 Height() const { return d_data.height; }
	u32 Width() const { return d_data.width; }
	float AspectRatio() const { return (float)d_data.width / (float)d_data.height; }
	bool Running() const { return d_data.running; }
	bool Focused() const { return d_data.focused; }

	// Callback Utilities
	void SetCallback(EventCallback cb);
		// Sets the callback to be given events. If a callback is already
		// provided, it is overwritten.

	// Window Utilities
	std::string GetWindowName() const;
	void SetWindowName(const std::string& name);
	void SetCursorVisibility(bool visibility);
	void SetFullscreen();
	void SetWindowed(int width, int height);
	bool IsFullscreen() const;

	// Mouse Utilities
	glm::vec2 GetMousePos() const;
	glm::vec2 GetMouseOffset() const;

	// Clipboard Utilities
	const char* GetClipboardData();
	void SetClipboardData(const std::string& text);

	// Low level Utilities
	void* NativeWindow() const;

	void Close() { d_data.running = false; }
};

}