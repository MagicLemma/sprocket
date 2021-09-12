#pragma once
#include <Sprocket/Core/events.h>

#include <glm/glm.hpp>

#include <cstddef>
#include <functional>
#include <memory>
#include <string>

#ifndef _WIN32
#error "Sprocket currently only supports Windows"
#endif

namespace spkt {

using event_handler = std::function<void(spkt::event&)>;

struct window_impl;

struct window_data
{
	std::string   name;
	std::uint32_t width;
	std::uint32_t height;

	bool fullscreen = false;
	bool running    = true;
	bool focused    = true;

	event_handler callback = [](spkt::event&) {};
};

class window
{
	std::unique_ptr<window_impl> d_impl;
	window_data                  d_data;

	glm::vec3 d_clear_colour;

	glm::vec2 d_mouse_position;
	glm::vec2 d_mouse_offset;

private:
	// Deleted Constructors
	window(window&&) = delete;
	window(const window&) = delete;
	window& operator=(const window&) = delete;

public:
	window(const std::string& name = "Window", std::uint32_t width = 1280, std::uint32_t height = 720);
	~window();

	void begin_frame();
	void end_frame();

	void SetClearColour(const glm::vec3& colour);

	// Getters
	std::uint32_t Height() const { return d_data.height; }
	std::uint32_t Width() const { return d_data.width; }
	float AspectRatio() const { return (float)d_data.width / (float)d_data.height; }
	bool Running() const { return d_data.running; }
	bool Focused() const { return d_data.focused; }

	// Callback Utilities
	void SetCallback(event_handler cb);
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
	const char* GetClipboardData() const;
	void SetClipboardData(const std::string& text) const;

	// Low level Utilities
	void* NativeWindow() const;

	void Close() { d_data.running = false; }
};

}