#include "Events/MouseEvent.h"

#include <sstream>

namespace Sprocket {

MouseButtonPressedEvent::MouseButtonPressedEvent(int button, int action, int mods)
	: d_button(button)
	, d_action(action)
	, d_mods(mods) 
{
}

std::string MouseButtonPressedEvent::toString() const
{
	std::stringstream ss;
	ss << "MouseButtonPressedEvent: " << d_button << ", " << d_action << ", " << d_mods;
	return ss.str();
}

MouseButtonReleasedEvent::MouseButtonReleasedEvent(int button, int action, int mods)
	: d_button(button)
	, d_action(action)
	, d_mods(mods) 
{
}

std::string MouseButtonReleasedEvent::toString() const
{
	std::stringstream ss;
	ss << "MouseButtonReleasedEvent: " << d_button << ", " << d_action << ", " << d_mods;
	return ss.str();
}

MouseMovedEvent::MouseMovedEvent(float xPos, float yPos)
	: d_xPos(xPos)
	, d_yPos(yPos)
{
}

std::string MouseMovedEvent::toString() const
{
	std::stringstream ss;
	ss << "MouseMovedEvent: " << d_xPos << ", " << d_yPos;
	return ss.str();
}

MouseScrolledEvent::MouseScrolledEvent(float xOffset, float yOffset)
	: d_xOffset(xOffset)
	, d_yOffset(yOffset)
{
}

std::string MouseScrolledEvent::toString() const
{
	std::stringstream ss;
	ss << "MouseScrolledEvent: " << d_xOffset << ", " << d_yOffset;
	return ss.str();
}

}