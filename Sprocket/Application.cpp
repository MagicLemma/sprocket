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
		d_window->onUpdate();
	}
}

void Application::onEvent(Event& event)
{
	if (auto e = dynamic_cast<WindowClosedEvent*>(&event)) {
		onWindowClose(*e);
	}
	SPKT_LOG_CORE_INFO("Dispatching " + event.toString());
}

bool Application::onWindowClose(WindowClosedEvent & e)
{
	d_running = false;
	return true;
}

}