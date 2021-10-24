#pragma once
#include <any>
#include <cstddef>

namespace spkt {

class event
{
	std::any d_event;
	bool     d_consumed = false;

public:
	template <typename T, typename... Args>
	explicit event(std::in_place_type_t<T>, Args&&... args)
		: d_event(std::in_place_type<T>, std::forward<Args>(args)...)
	{}

	template <typename T> const T* get_if() const noexcept { return std::any_cast<T>(&d_event); }
	bool is_consumed() const noexcept { return d_consumed; }
	void consume() noexcept { d_consumed = true; }
};

template <typename T, typename... Args>
event make_event(Args&&... args)
{
	return event(std::in_place_type<T>, std::forward<Args>(args)...);
}

struct keyboard_pressed_event { int key, scancode, mods; };
struct keyboard_released_event { int key, scancode, mods; };
struct keyboard_held_event { int key, scancode, mods; };
struct keyboard_typed_event { std::uint32_t key; };

struct mouse_pressed_event { int button, action, mods; };
struct mouse_released_event { int button, action, mods; };
struct mouse_moved_event { float x_pos, y_pos; };
struct mouse_scrolled_event { float x_offset, y_offset; };

struct window_resized_event { int width, height; };
struct window_closed_event {};
struct window_focused_event {};
struct window_unfocused_event {};
struct window_maximised_event {};
struct window_minimised_event {};

}