#include "Core/Window.h"
#include "Utility/Log.h"
#include "Events/WindowEvent.h"
#include "Events/KeyboardEvent.h"
#include "Events/MouseEvent.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Sprocket {

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
	, d_data({name, width, height})
	, d_callback([](const Event&) {})
	, d_clearColour({1.0, 1.0, 1.0})
{
	glfwInit();
	
	// Set the callback that will get called by GLFW. This just
	// forwards all events on to the keyboard/mouse and any
	// registered objects.
	d_data.callback = [&](Event& event) {
		if (auto e = event.As<WindowClosedEvent>()) {
			d_data.running = false;
			return;
		}
		d_callback(event);
	};

	d_impl->window = glfwCreateWindow(
		d_data.width,
		d_data.height,
		d_data.name.c_str(),
		nullptr,
		nullptr
	);

	glfwMakeContextCurrent(d_impl->window);
	glfwSetWindowUserPointer(d_impl->window, &d_data);

	// Initialise GLAD
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	int versionMajor;
	int versionMinor;
	glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);
	glGetIntegerv(GL_MINOR_VERSION, &versionMinor);
	SPKT_LOG_INFO("OpenGL version: {}.{}", versionMajor, versionMinor);

	// Set GLFW callbacks
	glfwSetWindowSizeCallback(d_impl->window, [](GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
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
		MouseMovedEvent event(static_cast<float>(xPos),
		                      static_cast<float>(yPos));
		data->callback(event);
	});

	glfwSetScrollCallback(d_impl->window, [](GLFWwindow* window, double xOffset, double yOffset) {
		WindowData* data = (WindowData*)glfwGetWindowUserPointer(window);
		if (!data->focused) return;
		MouseScrolledEvent event(static_cast<float>(xOffset),
		                         static_cast<float>(yOffset));
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

	glfwSetWindowMaximizeCallback(d_impl->window, [](GLFWwindow* window, int maximized) {
		WindowData* data = (WindowData*)glfwGetWindowUserPointer(window);
		if (maximized) {
			WindowMaximizeEvent event;
			data->callback(event);
		}
		else {
			WindowMinimizeEvent event;
			data->callback(event);
		}
	});

	glfwSetCharCallback(d_impl->window, [](GLFWwindow* window, unsigned int key) {
		WindowData* data = (WindowData*)glfwGetWindowUserPointer(window);
		if (!data->focused) return;
		KeyboardKeyTypedEvent event(key);
		data->callback(event);
	});
}

Window::~Window()
{
	glfwDestroyWindow(d_impl->window);
	glfwTerminate();
}

void Window::OnUpdate()
{
	glfwSwapBuffers(d_impl->window);
	glfwPollEvents();
}

void Window::Clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glClearColor(d_clearColour.r, d_clearColour.g, d_clearColour.b, 1.0);
}

void Window::SetClearColour(const Maths::vec3& colour)
{
	d_clearColour = colour;
}

void Window::SetCursorVisibility(bool visibility)
{
	if (visibility) {
		glfwSetInputMode(d_impl->window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
	else {
		glfwSetInputMode(d_impl->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
}

void Window::SetWindowName(const std::string& name)
{
	glfwSetWindowTitle(d_impl->window, name.c_str());
}

void Window::SetCallback(EventCallback cb)
{
	d_callback = cb;
}

const char* Window::GetClipboardData()
{
	return glfwGetClipboardString(d_impl->window);
}

void Window::SetClipboardData(const std::string& text)
{
	glfwSetClipboardString(d_impl->window, text.c_str());
}

}