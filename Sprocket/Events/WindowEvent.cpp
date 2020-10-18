#include "WindowEvent.h"
#include "Types.h"

#include <sstream>

namespace Sprocket {

WindowResizeEvent::WindowResizeEvent(u32 width, u32 height)
	: d_width(width)
	, d_height(height)
{
}

std::string WindowResizeEvent::ToString() const
{
	std::stringstream ss;
	ss << "WindowResizeEvent: " << d_width << ", " << d_height;
	return ss.str();
}

std::string WindowClosedEvent::ToString() const
{
	return "WindowClosedEvent";
}

std::string WindowGotFocusEvent::ToString() const
{
	return "WindowGotFocus";
}

std::string WindowLostFocusEvent::ToString() const
{
	return "WindowLostFocus";
}
std::string WindowMaximizeEvent::ToString() const
{
	return "WindowMaximizeEvent";
}

std::string WindowMinimizeEvent::ToString() const
{
	return "WindowMinimizeEvent";
}

}