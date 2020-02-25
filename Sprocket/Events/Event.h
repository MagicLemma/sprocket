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
public:
	virtual int categoryFlags() const = 0;
	virtual std::string toString() const = 0;

	inline bool isInCategory(EventCategory category) const
	{
		return categoryFlags() & category;
	}

	// Returns a const* to this object as Type if the original
	// type of this object is Type, and nullptr otherwise.
	template <typename Type> Type const* as() const
	{
		return dynamic_cast<Type const*>(this);
	}
};

}