#include "Application.h"
#include "Log.h"
#include "Events/Event.h"
#include "Events/WindowEvent.h"

#include <iostream>

namespace Sprocket {

Application::Application()
	: d_window(std::make_unique<Window>())
	, d_running(true)
{
	d_window->setCallback([this](Event& event) {
		onEvent(event);
	});
}

Application::~Application()
{
}

void Application::run()
{
	while (d_running)
	{
		d_window->clear();

		//for (auto& layer : d_layers) {
		//	layer->onUpdate();
		//}

		d_window->onUpdate();
	}
}

void Application::onEvent(Event& event)
{
	if (auto e = dynamic_cast<WindowClosedEvent*>(&event)) {
		onWindowClose(*e);
	}
	std::string e = event.toString();
	SPKT_LOG_CORE_INFO("Dispatching " + event.toString());
	//for (auto it = d_layers.rbegin(); it != d_layers.rend(); ++it) {
	//	(*it)->onEvent(event);
	//	if (event.handled()) {
	//		break;
	//	}
	//}
}

/*void Application::pushLayer(LayerPtr layer)
{
	d_layers.pushLayer(layer);
}

void Application::pushOverlay(LayerPtr layer)
{
	d_layers.pushOverlay(layer);
}
*/
bool Application::onWindowClose(WindowClosedEvent & e)
{
	d_running = false;
	return true;
}

}