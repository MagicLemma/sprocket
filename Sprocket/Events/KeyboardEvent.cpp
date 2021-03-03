#include "KeyboardEvent.h"
#include "Types.h"

namespace Sprocket {

KeyboardButtonPressedEvent::KeyboardButtonPressedEvent(int key, int scancode, int mods)
	: d_key(key)
	, d_scancode(scancode)
	, d_mods(mods)
{
}

KeyboardButtonReleasedEvent::KeyboardButtonReleasedEvent(int key, int scancode, int mods)
	: d_key(key)
	, d_scancode(scancode)
	, d_mods(mods) 
{
}

KeyboardButtonHeldEvent::KeyboardButtonHeldEvent(int key, int scancode, int mods)
	: d_key(key)
	, d_scancode(scancode)
	, d_mods(mods) 
{
}

KeyboardKeyTypedEvent::KeyboardKeyTypedEvent(u32 key)
	: d_key(key) 
{
}

}