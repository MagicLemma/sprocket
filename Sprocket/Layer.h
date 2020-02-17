#pragma once
#include "Events/Event.h"

#include <string>

namespace Sprocket {

class Layer
{
protected:
	std::string d_name;

public:
	Layer(const std::string& name = "Layer");
	virtual ~Layer();

	virtual void onAttach() {}
	virtual void onDetach() {}
	virtual void onUpdate() {}
	virtual void onEvent(Event& event) {}

	const std::string& name() { return d_name; }
};

}