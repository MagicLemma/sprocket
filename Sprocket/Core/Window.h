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

	void set_clear_colour(const glm::vec3& colour);

	std::uint32_t height() const { return d_data.height; }
	std::uint32_t width() const { return d_data.width; }
	float aspect_ratio() const { return (float)d_data.width / (float)d_data.height; }

	void set_event_handler(event_handler cb);

	// Window Utilities
	std::string name() const;
	void set_name(const std::string& name);
	void set_cursor_visibility(bool visibility);
	void set_fullscreen();
	void set_windowed(int width, int height);
	bool is_fullscreen() const;
	bool is_running() const { return d_data.running; }
	bool is_focused() const { return d_data.focused; }

	// Mouse Utilities
	glm::vec2 get_mouse_position() const;
	glm::vec2 get_mouse_offset() const;

	// Clipboard Utilities
	const char* get_clipboard_data() const;
	void set_clipboard_data(const std::string& text) const;

	// Low level Utilities
	void* native_handle() const;

	void close() { d_data.running = false; }
};

}