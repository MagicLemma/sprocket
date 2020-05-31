#pragma once
#include "Events/Event.h"

namespace Sprocket {

class KeyboardButtonPressedEvent : public Event
{
	int d_key;
	int d_scancode;
	int d_mods;

public:
	KeyboardButtonPressedEvent(int key, int scancode, int mods);
	virtual int CategoryFlags() const override { return KEYBOARD | INPUT; }
	std::string ToString() const override;

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
	virtual int CategoryFlags() const override { return KEYBOARD; }
	std::string ToString() const override;

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
	virtual int CategoryFlags() const override { return KEYBOARD | INPUT; }
	std::string ToString() const override;

	int Key() const { return d_key; }
	int Scancode() const { return d_scancode; }
	int Mods() const { return d_mods; }
};

class KeyboardKeyTypedEvent : public Event
{
	unsigned int d_key;

public:
	KeyboardKeyTypedEvent(unsigned int key);
	virtual int CategoryFlags() const override { return KEYBOARD | INPUT; }
	std::string ToString() const override;

	int Key() const { return d_key; }
};

}
