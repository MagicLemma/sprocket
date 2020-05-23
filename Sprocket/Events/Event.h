#pragma once
#include <variant>
#include <string>

namespace Sprocket {

enum EventCategory
{
	NONE = 0,

	WINDOW             = 1 << 1,
	INPUT              = 1 << 2,
	KEYBOARD           = 1 << 3,
	MOUSE              = 1 << 4
};

class Event
{
	bool d_consumed = false;

public:
	virtual int categoryFlags() const = 0;
	virtual std::string toString() const = 0;

	bool isConsumed() const { return d_consumed; }
	void consume() { d_consumed = true; }
		// A flag that can be set if something has used the event
		// but doesn't want to block it be sent onwards. Primary
		// motivation for this is mouse moved events in UIs. When
		// mousing over a GUI window, the "selected entity" system
		// should clear the hovered entity pointer; it will do this
		// when it receives a consumed MouseMovedEvent.

	inline bool isInCategory(EventCategory category) const
	{
		return categoryFlags() & category;
	}

	// Returns a pointer to this object as Type if the original
	// type of this object is Type, and nullptr otherwise.
	template <typename Type> Type* as()
	{
		return dynamic_cast<Type*>(this);
	}
};

}