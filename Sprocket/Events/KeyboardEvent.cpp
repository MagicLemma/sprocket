#include "KeyboardEvent.h"
#include "Types.h"

#include <sstream>

namespace Sprocket {

KeyboardButtonPressedEvent::KeyboardButtonPressedEvent(int key, int scancode, int mods)
	: d_key(key)
	, d_scancode(scancode)
	, d_mods(mods)
{
}

std::string KeyboardButtonPressedEvent::ToString() const
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

std::string KeyboardButtonReleasedEvent::ToString() const
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

std::string KeyboardButtonHeldEvent::ToString() const
{
	std::stringstream ss;
	ss << "KeyboardButtonHeldEvent: " << d_key << ", " << d_scancode << ", " << d_mods;
	return ss.str();
}

KeyboardKeyTypedEvent::KeyboardKeyTypedEvent(u32 key)
	: d_key(key) 
{
}

std::string KeyboardKeyTypedEvent::ToString() const
{
	std::stringstream ss;
	ss << "KeyboardKeyTypedEvent: " << d_key;
	return ss.str();
}

}