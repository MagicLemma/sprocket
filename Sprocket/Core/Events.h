#pragma once
#include <any>
#include <string>
#include <typeinfo> // TODO: Replace with own version
#include <typeindex>
#include <functional>

#include "Types.h"

namespace Sprocket {
namespace ev {

class Event
{
	std::any d_event;
	bool     d_consumed = false;

public:
	template <typename T, typename... Args>
	explicit Event(std::in_place_type_t<T>, Args&&... args)
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
Event make_event(Args&&... args)
{
	return Event(std::in_place_type<T>, std::forward<Args>(args)...);
}

class Dispatcher
{
public:
	using EventHandler = std::function<void(ev::Event&)>;

	template <typename T>
	using EventHandlerTyped = std::function<void(ev::Event&, const T&)>;

private:
	std::unordered_map<std::type_index, std::vector<EventHandler>> d_handlers;

public:
	template <typename T>
	void subscribe(const EventHandler& handler)
	{
		d_handlers[typeid(T)].push_back(handler);
	}

	template <typename T>
	void subscribe(const EventHandlerTyped<T>& handler)
	{
		d_handlers[typeid(T)].push_back([handler](ev::Event& event) {
			handler(event, event.get<T>());
		});
	}

	void desubscribe_all()
	{
		d_handlers.clear();
	}

	void publish(ev::Event& event) const
	{
		if (auto it = d_handlers.find(event.type_info()); it != d_handlers.end()) {
			for (auto& handler : it->second) {
				handler(event);
			}
		}
	}
};

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
	u32 key;
	KeyboardTyped(u32 k) : key(k) {}
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

}