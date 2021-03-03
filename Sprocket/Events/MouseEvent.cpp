#include "Events/MouseEvent.h"

namespace Sprocket {

MouseButtonPressedEvent::MouseButtonPressedEvent(int button, int action, int mods)
	: d_button(button)
	, d_action(action)
	, d_mods(mods) 
{
}

MouseButtonReleasedEvent::MouseButtonReleasedEvent(int button, int action, int mods)
	: d_button(button)
	, d_action(action)
	, d_mods(mods) 
{
}

MouseMovedEvent::MouseMovedEvent(float xPos, float yPos)
	: d_xPos(xPos)
	, d_yPos(yPos)
{
}

MouseScrolledEvent::MouseScrolledEvent(float xOffset, float yOffset)
	: d_xOffset(xOffset)
	, d_yOffset(yOffset)
{
}

}