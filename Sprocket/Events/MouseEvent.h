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

	int Button() const { return d_button; }
	int Action() const { return d_action; }
	int Mods() const { return d_mods; }
};

class MouseButtonReleasedEvent : public Event
{
	int d_button;
	int d_action;
	int d_mods;

public:
	MouseButtonReleasedEvent(int button, int action, int mods);

	int Button() const { return d_button; }
	int Action() const { return d_action; }
	int Mods() const { return d_mods; }
};

class MouseMovedEvent : public Event
{
	float d_xPos;
	float d_yPos;

public:
	MouseMovedEvent(float xPos, float yPos);

	float XPos() const { return d_xPos; }
	float YPos() const { return d_yPos; }
};

class MouseScrolledEvent : public Event
{
	float d_xOffset;
	float d_yOffset;

public:
	MouseScrolledEvent(float xOffset, float yOffset);

	float XOffset() const { return d_xOffset; }
	float YOffset() const { return d_yOffset; }
};

}