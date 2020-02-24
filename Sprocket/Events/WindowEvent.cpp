#include "Events/WindowEvent.h"

#include <sstream>

namespace Sprocket {

WindowResizeEvent::WindowResizeEvent(unsigned int width, unsigned int height)
	: d_width(width)
	, d_height(height)
{
}

std::string WindowResizeEvent::toString() const
{
	std::stringstream ss;
	ss << "WindowResizeEvent: " << d_width << ", " << d_height;
	return ss.str();
}

WindowClosedEvent::WindowClosedEvent()
{
}

std::string WindowClosedEvent::toString() const
{
	return "WindowClosedEvent";
}

WindowGotFocusEvent::WindowGotFocusEvent()
{
}

std::string WindowGotFocusEvent::toString() const
{
	return "WindowGotFocus";
}

WindowLostFocusEvent::WindowLostFocusEvent()
{
}

std::string WindowLostFocusEvent::toString() const
{
	return "WindowLostFocus";
}

}