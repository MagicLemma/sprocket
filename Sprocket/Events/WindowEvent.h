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
	virtual int CategoryFlags() const override { return WINDOW; }
	std::string ToString() const override;

	u32 Width() const { return d_width; }
	u32 Height() const { return d_height; }
	float AspectRatio() const { return (float)d_width / (float)d_height; }
};

class WindowClosedEvent : public Event
{
public:
	virtual int CategoryFlags() const override { return WINDOW; }
	std::string ToString() const override;
};

class WindowGotFocusEvent : public Event
{
public:
	virtual int CategoryFlags() const override { return WINDOW; }
	std::string ToString() const override;
};

class WindowLostFocusEvent : public Event
{
public:
	virtual int CategoryFlags() const override { return WINDOW; }
	std::string ToString() const override;
};

class WindowMaximizeEvent : public Event
{
public:
	virtual int CategoryFlags() const override { return WINDOW; }
	std::string ToString() const override;
};

class WindowMinimizeEvent : public Event
{
public:
	virtual int CategoryFlags() const override { return WINDOW; }
	std::string ToString() const override;
};

}