#include "KeyboardEvent.h"
#include "Types.h"

#include <fmt/core.h>

namespace Sprocket {

KeyboardButtonPressedEvent::KeyboardButtonPressedEvent(int key, int scancode, int mods)
	: d_key(key)
	, d_scancode(scancode)
	, d_mods(mods)
{
}

std::string KeyboardButtonPressedEvent::ToString() const
{
	return fmt::format("KeyboardButtonPressedEvent: {}, {}, {}", d_key, d_scancode, d_mods);
}

KeyboardButtonReleasedEvent::KeyboardButtonReleasedEvent(int key, int scancode, int mods)
	: d_key(key)
	, d_scancode(scancode)
	, d_mods(mods) 
{
}

std::string KeyboardButtonReleasedEvent::ToString() const
{
	return fmt::format("KeyboardButtonReleasedEvent: {}, {}, {}", d_key, d_scancode, d_mods);
}

KeyboardButtonHeldEvent::KeyboardButtonHeldEvent(int key, int scancode, int mods)
	: d_key(key)
	, d_scancode(scancode)
	, d_mods(mods) 
{
}

std::string KeyboardButtonHeldEvent::ToString() const
{
	return fmt::format("KeyboardButtonHeldEvent: {}, {}, {}", d_key, d_scancode, d_mods);
}

KeyboardKeyTypedEvent::KeyboardKeyTypedEvent(u32 key)
	: d_key(key) 
{
}

std::string KeyboardKeyTypedEvent::ToString() const
{
	return fmt::format("KeyboardKeyTypedEvent: {}", d_key);
}

}