#pragma once
#include <variant>
#include <string>

namespace Sprocket {

enum EventCategory
{
	NONE = 0,

	WINDOW             = 1 << 1,
	INPUT              = 1 << 2,
	KEYBOARD_BUTTON    = 1 << 3,
	MOUSE_BUTTON       = 1 << 4,
	MOUSE              = 1 << 5
};

class Event
{
	bool d_consumed = false;

public:
	virtual int categoryFlags() const = 0;
	virtual const char* name() const = 0;
	virtual std::string toString() const { return "Event"; }

	inline bool isInCategory(EventCategory category)
	{
		return categoryFlags() & category;
	}

	void consume() { d_consumed = true; }
	bool handled() { return d_consumed; }
};

}