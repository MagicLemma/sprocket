#pragma once
#include <Sprocket/Core/Events.h>

#include <glm/glm.hpp>

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
	std::uint32_t width;
	std::uint32_t height;

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
	Window(const std::string& name = "Window", std::uint32_t width = 1280, std::uint32_t height = 720);
	~Window();

	void on_update();
	void Clear();

	void SetClearColour(const glm::vec3& colour);

	// Getters
	std::uint32_t Height() const { return d_data.height; }
	std::uint32_t Width() const { return d_data.width; }
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