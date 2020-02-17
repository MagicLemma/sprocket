#pragma once
#include "Window.h"
#include "Events/WindowEvent.h"
//#include "Layer.h"
#include "Log.h"
//#include "LayerStack.h"

#include <memory>

namespace Sprocket {

class Application
{
	std::unique_ptr<Window> d_window;
	//LayerStack              d_layers;

	bool                    d_running;

	bool onWindowClose(WindowClosedEvent& e);

public:
	Application();
	virtual ~Application();

	virtual void run();

	void onEvent(Event& event);

	//void pushLayer(LayerPtr layer);
	//void pushOverlay(LayerPtr layer);
};

}