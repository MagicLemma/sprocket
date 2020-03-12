#pragma once
#include "Events/Event.h"

namespace Sprocket {

class WindowResizeEvent : public Event
{
	unsigned int d_width;
	unsigned int d_height;

public:
	WindowResizeEvent(unsigned int width, unsigned int height);
	virtual int categoryFlags() const override { return WINDOW; }
	std::string toString() const override;

	unsigned int width() const { return d_width; }
	unsigned int height() const { return d_height; }
	float aspectRatio() const { return (float)d_width / (float)d_height; }
};

class WindowClosedEvent : public Event
{
public:
	virtual int categoryFlags() const override { return WINDOW; }
	std::string toString() const override;
};

class WindowGotFocusEvent : public Event
{
public:
	virtual int categoryFlags() const override { return WINDOW; }
	std::string toString() const override;
};

class WindowLostFocusEvent : public Event
{
public:
	virtual int categoryFlags() const override { return WINDOW; }
	std::string toString() const override;
};

class WindowMaximizeEvent : public Event
{
public:
	virtual int categoryFlags() const override { return WINDOW; }
	std::string toString() const override;
};

class WindowMinimizeEvent : public Event
{
public:
	virtual int categoryFlags() const override { return WINDOW; }
	std::string toString() const override;
};

}