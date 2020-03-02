#include "Core/Window.h"
#include "Utility/Log.h"
#include "Events/WindowEvent.h"
#include "Events/KeyboardEvent.h"
#include "Events/MouseEvent.h"

#include <glad/glad.h>
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
	, d_data({name, width, height, true, true})
	, d_keyboardCallback([](const Event&){})
	, d_mouseCallback([](const Event&){})
	, d_extraCallbacks()
{
	// Set the callback that will get called by GLFW. This just
	// forwards all events on to the keyboard/mouse and any
	// registered objects.
	d_data.callback = [&](const Event& event) {
		if (auto e = event.as<WindowClosedEvent>()) {
			d_data.running = false;
			return;
		}
		if (event.isInCategory(EventCategory::KEYBOARD)) {
			d_keyboardCallback(event);
		}
		if (event.isInCategory(EventCategory::MOUSE)) {
			d_mouseCallback(event);
		}
		for (auto [name, callback]: d_extraCallbacks)
		{
			callback(event);
		}
	};

	if (!s_GLFWInitialised) {
		int initGlfw = glfwInit();
		// TODO: SPKT_CORE_ASSERT(success == 0, "Could not initialise GLFW!");
		s_GLFWInitialised = true;
	}

	d_impl->window = glfwCreateWindow(
		d_data.width,
		d_data.height,
		d_data.name.c_str(),
		nullptr,
		nullptr
	);

	glfwMakeContextCurrent(d_impl->window);
	glfwSetWindowUserPointer(d_impl->window, &d_data);
	glfwSwapInterval(1);  // Set VSync to be true
	glfwSetInputMode(d_impl->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Initialise GLAD
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	// Set GLFW callbacks
	glfwSetWindowSizeCallback(d_impl->window, [](GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height); // Update the viewport

		WindowData* data = (WindowData*)glfwGetWindowUserPointer(window);
		if (!data->focused) return;
		WindowResizeEvent event(width, height);
		data->width = width;
		data->height = height;
		data->callback(event);
	});

	glfwSetWindowCloseCallback(d_impl->window, [](GLFWwindow* window)
	{
		WindowData* data = (WindowData*)glfwGetWindowUserPointer(window);
		if (!data->focused) return;
		WindowClosedEvent event;
		data->callback(event);
	});

	glfwSetKeyCallback(d_impl->window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
		WindowData* data = (WindowData*)glfwGetWindowUserPointer(window);
		if (!data->focused) return;
		switch (action)
		{
			case GLFW_PRESS: {
				KeyboardButtonPressedEvent event(key, scancode, mods);
				data->callback(event);
			} break;
			case GLFW_RELEASE: {
				KeyboardButtonReleasedEvent event(key, scancode, mods);
				data->callback(event);
			} break;
			case GLFW_REPEAT: {
				KeyboardButtonHeldEvent event(key, scancode, mods);
				data->callback(event);
			} break;
		}
	});

	glfwSetMouseButtonCallback(d_impl->window, [](GLFWwindow* window, int button, int action, int mods) {
		WindowData* data = (WindowData*)glfwGetWindowUserPointer(window);
		if (!data->focused) return;
		switch (action)
		{
		case GLFW_PRESS: {
			MouseButtonPressedEvent event(button, action, mods);
			data->callback(event);
		} break;
		case GLFW_RELEASE: {
			MouseButtonReleasedEvent event(button, action, mods);
			data->callback(event);
		} break;
		}
	});

	glfwSetCursorPosCallback(d_impl->window, [](GLFWwindow* window, double xPos, double yPos) {
		WindowData* data = (WindowData*)glfwGetWindowUserPointer(window);
		if (!data->focused) return;
		MouseMovedEvent event(xPos, yPos);
		data->callback(event);
	});

	glfwSetScrollCallback(d_impl->window, [](GLFWwindow* window, double xOffset, double yOffset) {
		WindowData* data = (WindowData*)glfwGetWindowUserPointer(window);
		if (!data->focused) return;
		MouseScrolledEvent event(xOffset, yOffset);
		data->callback(event);
	});

	glfwSetWindowFocusCallback(d_impl->window, [](GLFWwindow* window, int focused) {
		WindowData* data = (WindowData*)glfwGetWindowUserPointer(window);
		if (focused) {
			WindowGotFocusEvent event;
			data->focused = true;
			data->callback(event);
		}
		else {
			WindowLostFocusEvent event;
			data->focused = false;
			data->callback(event);
		}
	});
}

Window::~Window()
{
	glfwDestroyWindow(d_impl->window);
}

void Window::onUpdate()
{
	glfwSwapBuffers(d_impl->window);
	glfwPollEvents();
}

void Window::clear()
{
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0, 0, 0, 1);
}

void Window::setCursorVisibility(bool visibility)
{
	if (visibility) {
		glfwSetInputMode(d_impl->window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
	else {
		glfwSetInputMode(d_impl->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
}

void Window::setFaceCulling(bool culling)
{
    if (culling) {
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
    }
    else {
        glDisable(GL_CULL_FACE);
    }
}

void Window::registerKeyboardCallback(EventCallback cb)
{
	d_keyboardCallback = cb;
}

void Window::registerMouseCallback(EventCallback cb)
{
	d_mouseCallback = cb;
}

void Window::registerCallback(const std::string& name, EventCallback cb)
{
	d_extraCallbacks[name] = cb;
}

void Window::deregisterCallback(const std::string& name)
{
	auto it = d_extraCallbacks.find(name);
	if (it != d_extraCallbacks.end()) {
		d_extraCallbacks.erase(it);
	}
}

}