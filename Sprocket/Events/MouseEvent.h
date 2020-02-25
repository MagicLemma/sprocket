#pragma once
#include "Events/Event.h"

namespace Sprocket {

class MouseButtonPressedEvent : public Event
{
	int d_button;
	int d_action;
	int d_mods;

public:
	MouseButtonPressedEvent(int button, int action, int mods);
	virtual int categoryFlags() const override { return MOUSE | INPUT; }
	std::string toString() const override;

	int button() const { return d_button; }
	int action() const { return d_action; }
	int mods() const { return d_mods; }
};

class MouseButtonReleasedEvent : public Event
{
	int d_button;
	int d_action;
	int d_mods;

public:
	MouseButtonReleasedEvent(int button, int action, int mods);
	virtual int categoryFlags() const override { return MOUSE; }
	std::string toString() const override;

	int button() const { return d_button; }
	int action() const { return d_action; }
	int mods() const { return d_mods; }
};

class MouseMovedEvent : public Event
{
	double d_xPos;
	double d_yPos;

public:
	MouseMovedEvent(double xPos, double yPos);
	virtual int categoryFlags() const override { return MOUSE; }
	std::string toString() const override;

	double xPos() const { return d_xPos; }
	double yPos() const { return d_yPos; }
};

class MouseScrolledEvent : public Event
{
	double d_xOffset;
	double d_yOffset;

public:
	MouseScrolledEvent(double xOffset, double yOffset);
	virtual int categoryFlags() const override { return MOUSE | INPUT; }
	std::string toString() const override;

	double xOffset() const { return d_xOffset; }
	double yOffset() const { return d_yOffset; }
};

}