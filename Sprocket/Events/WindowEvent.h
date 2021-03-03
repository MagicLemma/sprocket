#pragma once
#include "Event.h"
#include "Types.h"

namespace Sprocket {

class WindowResizeEvent : public Event
{
	u32 d_width;
	u32 d_height;

public:
	WindowResizeEvent(u32 width, u32 height);

	u32 Width() const { return d_width; }
	u32 Height() const { return d_height; }
	float AspectRatio() const { return (float)d_width / (float)d_height; }
};

class WindowClosedEvent : public Event {};
class WindowGotFocusEvent : public Event {};
class WindowLostFocusEvent : public Event {};
class WindowMaximizeEvent : public Event {};
class WindowMinimizeEvent : public Event {};

}