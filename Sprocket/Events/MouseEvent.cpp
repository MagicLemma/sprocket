#include "Events/MouseEvent.h"

#include <fmt/core.h>

namespace Sprocket {

MouseButtonPressedEvent::MouseButtonPressedEvent(int button, int action, int mods)
	: d_button(button)
	, d_action(action)
	, d_mods(mods) 
{
}

std::string MouseButtonPressedEvent::ToString() const
{
	return fmt::format("MouseButtonPressedEvent: {}, {}, {}", d_button, d_action, d_mods);

}

MouseButtonReleasedEvent::MouseButtonReleasedEvent(int button, int action, int mods)
	: d_button(button)
	, d_action(action)
	, d_mods(mods) 
{
}

std::string MouseButtonReleasedEvent::ToString() const
{
	return fmt::format("MouseButtonPressedEvent: {}, {}, {}", d_button, d_action, d_mods);
}

MouseMovedEvent::MouseMovedEvent(float xPos, float yPos)
	: d_xPos(xPos)
	, d_yPos(yPos)
{
}

std::string MouseMovedEvent::ToString() const
{
	return fmt::format("MouseMovedEvent: {}, {}", d_xPos, d_yPos);
}

MouseScrolledEvent::MouseScrolledEvent(float xOffset, float yOffset)
	: d_xOffset(xOffset)
	, d_yOffset(yOffset)
{
}

std::string MouseScrolledEvent::ToString() const
{
	return fmt::format("MouseScrolledEvent: {}, {}", d_xOffset, d_yOffset);
}

}