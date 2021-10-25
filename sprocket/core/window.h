#pragma once
#include <sprocket/core/events.h>

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
	glm::vec3     clear_colour;

	bool fullscreen = false;
	bool running    = true;
	bool focused    = true;

	glm::vec2 mouse_position;
	glm::vec2 mouse_offset;
	glm::vec2 mouse_scrolled;

	event_handler callback = [](spkt::event&) {};
};

class window
{
	std::unique_ptr<window_impl> d_impl;
	window_data                  d_data;

	window(const window&) = delete;
	window& operator=(const window&) = delete;

public:
	window(const std::string& name = "Sprocket", std::uint32_t width = 1280, std::uint32_t height = 720);
	~window();

	void close() { d_data.running = false; }

	void begin_frame();
	void end_frame();

	void set_name(const std::string& name);
	void set_event_handler(const event_handler& cb);
	void set_clear_colour(const glm::vec3& colour);
	void set_fullscreen();
	void set_cursor_visibility(bool visibility);
	void set_windowed(int width, int height);

	std::string name() const { return d_data.name; }
	std::uint32_t height() const { return d_data.height; }
	std::uint32_t width() const { return d_data.width; }
	float aspect_ratio() const { return (float)d_data.width / (float)d_data.height; }

	bool is_fullscreen() const { return d_data.fullscreen; }
	bool is_running() const { return d_data.running; }
	bool is_focused() const { return d_data.focused; }

	glm::vec2 mouse_position() const;
	glm::vec2 mouse_offset() const { return d_data.mouse_offset; }
	glm::vec2 mouse_scrolled() const { return d_data.mouse_scrolled; }

	const char* get_clipboard_data() const;
	void set_clipboard_data(const std::string& text) const;

	void* native_handle() const;

};

}