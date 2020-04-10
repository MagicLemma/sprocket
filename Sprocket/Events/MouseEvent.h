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
	float d_xPos;
	float d_yPos;

public:
	MouseMovedEvent(float xPos, float yPos);
	virtual int categoryFlags() const override { return MOUSE; }
	std::string toString() const override;

	float xPos() const { return d_xPos; }
	float yPos() const { return d_yPos; }
};

class MouseScrolledEvent : public Event
{
	float d_xOffset;
	float d_yOffset;

public:
	MouseScrolledEvent(float xOffset, float yOffset);
	virtual int categoryFlags() const override { return MOUSE | INPUT; }
	std::string toString() const override;

	float xOffset() const { return d_xOffset; }
	float yOffset() const { return d_yOffset; }
};

}