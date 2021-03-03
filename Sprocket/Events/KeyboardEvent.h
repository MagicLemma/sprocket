#pragma once
#include "Event.h"
#include "Types.h"

namespace Sprocket {

class KeyboardButtonPressedEvent : public Event
{
	int d_key;
	int d_scancode;
	int d_mods;

public:
	KeyboardButtonPressedEvent(int key, int scancode, int mods);

	int Key() const { return d_key; }
	int Scancode() const { return d_scancode; }
	int Mods() const { return d_mods; }
};

class KeyboardButtonReleasedEvent : public Event
{
	int d_key;
	int d_scancode;
	int d_mods;

public:
	KeyboardButtonReleasedEvent(int key, int scancode, int mods);

	int Key() const { return d_key; }
	int Scancode() const { return d_scancode; }
	int Mods() const { return d_mods; }
};

class KeyboardButtonHeldEvent : public Event
{
	int d_key;
	int d_scancode;
	int d_mods;

public:
	KeyboardButtonHeldEvent(int key, int scancode, int mods);

	int Key() const { return d_key; }
	int Scancode() const { return d_scancode; }
	int Mods() const { return d_mods; }
};

class KeyboardKeyTypedEvent : public Event
{
	u32 d_key;

public:
	KeyboardKeyTypedEvent(u32 key);

	int Key() const { return d_key; }
};

}
