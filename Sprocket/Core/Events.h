#pragma once
#include <any>
#include <cstddef>
#include <functional>
#include <string>
#include <typeindex>
#include <typeinfo> // TODO: Replace with own version

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

	template <typename T> bool is() const noexcept { return d_event.type() == typeid(T); }
	template <typename T> const T& get() const { return std::any_cast<const T&>(d_event); }
	template <typename T> const T* get_if() const noexcept { return std::any_cast<T>(&d_event); }

	bool is_consumed() const noexcept { return d_consumed; }
	void consume() noexcept { d_consumed = true; }

	// Implementation defined name, should only be used for logging.
	std::string type_name() const noexcept { return d_event.type().name(); }

	const std::type_info& type_info() const noexcept { return d_event.type(); }
};

template <typename T, typename... Args>
event make_event(Args&&... args)
{
	return event(std::in_place_type<T>, std::forward<Args>(args)...);
}

// KEYBOARD EVENTS 
struct KeyboardButtonPressed {
	int key;
	int scancode;
	int mods;
	KeyboardButtonPressed(int k, int s, int m) : key(k), scancode(s), mods(m) {}
};

struct KeyboardButtonReleased {
	int key;
	int scancode;
	int mods;
	KeyboardButtonReleased(int k, int s, int m) : key(k), scancode(s), mods(m) {}
};

struct KeyboardButtonHeld {
	int key;
	int scancode;
	int mods;
	KeyboardButtonHeld(int k, int s, int m) : key(k), scancode(s), mods(m) {}
};

struct KeyboardTyped {
	std::uint32_t key;
	KeyboardTyped(std::uint32_t k) : key(k) {}
};

// MOUSE EVENTS
struct MouseButtonPressed {
	int button;
	int action;
	int mods;
	MouseButtonPressed(int b, int a, int m) : button(b), action(a), mods(m) {}
};

struct MouseButtonReleased {
	int button;
	int action;
	int mods;
	MouseButtonReleased(int b, int a, int m) : button(b), action(a), mods(m) {}
};

struct MouseMoved {
	float x_pos;
	float y_pos;
	MouseMoved(float x, float y) : x_pos(x), y_pos(y) {}
};

struct MouseScrolled {
	float x_offset;
	float y_offset;
	MouseScrolled(float x, float y) : x_offset(x), y_offset(y) {}
};

// WINDOW EVENTS
struct WindowResize {
	int width;
	int height;
	WindowResize(int w, int h) : width(w), height(h) {}
};

struct WindowClosed {};
struct WindowGotFocus {};
struct WindowLostFocus {};
struct WindowMaximize {};
struct WindowMinimize {};

}