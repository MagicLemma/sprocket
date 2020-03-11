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

std::string WindowClosedEvent::toString() const
{
	return "WindowClosedEvent";
}

std::string WindowGotFocusEvent::toString() const
{
	return "WindowGotFocus";
}

std::string WindowLostFocusEvent::toString() const
{
	return "WindowLostFocus";
}
std::string WindowMaximizeEvent::toString() const
{
	return "WindowMaximizeEvent";
}

std::string WindowMinimizeEvent::toString() const
{
	return "WindowMinimizeEvent";
}

}