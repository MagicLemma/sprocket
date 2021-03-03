#pragma once
#include <variant>
#include <string>

namespace Sprocket {

class Event
{
	bool d_consumed = false;

public:
	virtual ~Event() {}
	
	bool IsConsumed() const { return d_consumed; }
	void Consume() { d_consumed = true; }
		// A flag that can be set if something has used the event
		// but doesn't want to block it be sent onwards. Primary
		// motivation for this is mouse moved events in UIs. When
		// mousing over a GUI window, the "selected entity" system
		// should clear the hovered entity pointer; it will do this
		// when it receives a consumed MouseMovedEvent.

	// Returns a pointer to this object as Type if the original
	// type of this object is Type, and nullptr otherwise.
	template <typename Type> Type* As()
	{
		return dynamic_cast<Type*>(this);
	}
};

}