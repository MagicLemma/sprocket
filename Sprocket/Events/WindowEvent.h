#pragma once
#include "Events/Event.h"

namespace Sprocket {

class WindowResizeEvent : public Event
{
	unsigned int d_width;
	unsigned int d_height;

public:
	WindowResizeEvent(unsigned int width, unsigned int height);
	virtual int CategoryFlags() const override { return WINDOW; }
	std::string ToString() const override;

	unsigned int Width() const { return d_width; }
	unsigned int Height() const { return d_height; }
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