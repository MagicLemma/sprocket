#include "KeyboardEvent.h"

#include <sstream>

namespace Sprocket {

KeyboardButtonPressedEvent::KeyboardButtonPressedEvent(int key, int scancode, int mods)
	: d_key(key)
	, d_scancode(scancode)
	, d_mods(mods)
{
}

std::string KeyboardButtonPressedEvent::toString() const
{
	std::stringstream ss;
	ss << "KeyboardButtonPressedEvent: " << d_key << ", " << d_scancode << ", " << d_mods;
	return ss.str();
}

KeyboardButtonReleasedEvent::KeyboardButtonReleasedEvent(int key, int scancode, int mods)
	: d_key(key)
	, d_scancode(scancode)
	, d_mods(mods) 
{
}

std::string KeyboardButtonReleasedEvent::toString() const
{
	std::stringstream ss;
	ss << "KeyboardButtonReleasedEvent: " << d_key << ", " << d_scancode << ", " << d_mods;
	return ss.str();
}

KeyboardButtonHeldEvent::KeyboardButtonHeldEvent(int key, int scancode, int mods)
	: d_key(key)
	, d_scancode(scancode)
	, d_mods(mods) 
{
}

std::string KeyboardButtonHeldEvent::toString() const
{
	std::stringstream ss;
	ss << "KeyboardButtonHeldEvent: " << d_key << ", " << d_scancode << ", " << d_mods;
	return ss.str();
}

}