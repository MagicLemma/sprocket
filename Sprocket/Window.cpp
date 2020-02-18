#include "Window.h"
#include "Log.h"
#include "Events/WindowEvent.h"
#include "Events/KeyboardEvent.h"
#include "Events/MouseEvent.h"

#include <GLFW/glfw3.h>

namespace Sprocket {

namespace {

static bool s_GLFWInitialised = false;

}

struct WindowImpl
{
	GLFWwindow* window;	
};

Window::Window(
	const std::string& name,
	unsigned int width, 
	unsigned int height
)
	: d_impl(std::make_shared<WindowImpl>())
	, d_name(name)
	, d_width(width)
	, d_height(height)
	, d_running(true)
	, d_focused(true)
{
	if (!s_GLFWInitialised) {
		int success = glfwInit();
		// TODO: SPKT_CORE_ASSERT(success == 0, "Could not initialise GLFW!");
		s_GLFWInitialised = true;
	}

	d_impl->window = glfwCreateWindow(
		d_width,
		d_height,
		d_name.c_str(),
		nullptr,
		nullptr
	);

	glfwMakeContextCurrent(d_impl->window);
	glfwSetWindowUserPointer(d_impl->window, this);
	glfwSwapInterval(1);  // Set VSync to be true

	// Set GLFW callbacks
	glfwSetWindowSizeCallback(d_impl->window, [](GLFWwindow* window, int width, int height)
	{
		Window* w = (Window*)glfwGetWindowUserPointer(window);
		if (!w->focused()) return;
		WindowResizeEvent event(width, height);
		w->width(width);
		w->height(height);
		w->onEvent(event);
	});

	glfwSetWindowCloseCallback(d_impl->window, [](GLFWwindow* window)
	{
		Window* w = (Window*)glfwGetWindowUserPointer(window);
		if (!w->focused()) return;
		WindowClosedEvent event;
		w->onEvent(event);
	});

	glfwSetKeyCallback(d_impl->window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
		Window* w = (Window*)glfwGetWindowUserPointer(window);
		if (!w->focused()) return;
		switch (action)
		{
			case GLFW_PRESS: {
				KeyboardButtonPressedEvent event(key, scancode, mods);
				w->onEvent(event);
			} break;
			case GLFW_RELEASE: {
				KeyboardButtonReleasedEvent event(key, scancode, mods);
				w->onEvent(event);
			} break;
			case GLFW_REPEAT: {
				KeyboardButtonHeldEvent event(key, scancode, mods);
				w->onEvent(event);
			} break;
		}
	});

	glfwSetMouseButtonCallback(d_impl->window, [](GLFWwindow* window, int button, int action, int mods) {
		Window* w = (Window*)glfwGetWindowUserPointer(window);
		if (!w->focused()) return;
		switch (action)
		{
		case GLFW_PRESS: {
			MouseButtonPressedEvent event(button, action, mods);
			w->onEvent(event);
		} break;
		case GLFW_RELEASE: {
			MouseButtonReleasedEvent event(button, action, mods);
			w->onEvent(event);
		} break;
		}
	});

	glfwSetCursorPosCallback(d_impl->window, [](GLFWwindow* window, double xPos, double yPos) {
		Window* w = (Window*)glfwGetWindowUserPointer(window);
		if (!w->focused()) return;
		MouseMovedEvent event(xPos, yPos);
		w->onEvent(event);
	});

	glfwSetScrollCallback(d_impl->window, [](GLFWwindow* window, double xOffset, double yOffset) {
		Window* w = (Window*)glfwGetWindowUserPointer(window);
		if (!w->focused()) return;
		MouseScrolledEvent event(xOffset, yOffset);
		w->onEvent(event);
	});

	glfwSetWindowFocusCallback(d_impl->window, [](GLFWwindow* window, int focused) {
		Window* w = (Window*)glfwGetWindowUserPointer(window);
		if (focused) {
			WindowGotFocusEvent event;
			w->focused(true);
			w->onEvent(event);
		}
		else {
			WindowLostFocusEvent event;
			w->focused(false);
			w->onEvent(event);
		}
	});
}

Window::~Window()
{
	glfwDestroyWindow(d_impl->window);
}

void Window::onUpdate()
{
	glfwPollEvents();
	glfwSwapBuffers(d_impl->window);
}

void Window::onEvent(Event& event)
{
	SPKT_LOG_CORE_INFO("Dispatching {}", event.toString());
	if (auto e = dynamic_cast<WindowClosedEvent*>(&event))
	{
		d_running = false;
		return;
	}
	for (auto callback: d_callbacks)
	{
		callback(event);
	}
}

void Window::clear()
{
}

}