#include "Window.h"
#include "Log.h"
#include "WindowEvent.h"
#include "KeyboardEvent.h"
#include "MouseEvent.h"
#include "Log.h"
#include "Types.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Sprocket {

struct WindowImpl
{
	GLFWwindow* window;	
};

Window::Window(const std::string& name, u32 width, u32 height)
	: d_impl(std::make_unique<WindowImpl>())
	, d_data({name, width, height})
	, d_clearColour({1.0, 1.0, 1.0})
{
	Log::Init();
	
	glfwInit();

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

	// Set OpenGL error callback
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback([](GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void*) {
		
		switch (severity) {
			case GL_DEBUG_SEVERITY_NOTIFICATION: return;
			case GL_DEBUG_SEVERITY_LOW: {
				SPKT_LOG_TRACE("{}, {}, {}, {}, {}", source, type, id, length, message);
			} break;
			case GL_DEBUG_SEVERITY_MEDIUM: {
				SPKT_LOG_WARN("{}, {}, {}, {}, {}", source, type, id, length, message);
			} break;
			case GL_DEBUG_SEVERITY_HIGH: {
				SPKT_LOG_ERROR("{}, {}, {}, {}, {}", source, type, id, length, message);
			} break;
		}
	}, nullptr);

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
		data->running = false;
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

	glfwSetCharCallback(d_impl->window, [](GLFWwindow* window, u32 key) {
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

	double x, y;
	glfwGetCursorPos(d_impl->window, &x, &y);
	glm::vec2 newMousePos{x, y};

	d_mouseOffset = newMousePos - d_mousePos;
	d_mousePos = newMousePos;
}

void Window::Clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glClearColor(d_clearColour.r, d_clearColour.g, d_clearColour.b, 1.0);
}

void Window::SetClearColour(const glm::vec3& colour)
{
	d_clearColour = colour;
}

void Window::SetCursorVisibility(bool visibility)
{
	int show = visibility ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED;
	glfwSetInputMode(d_impl->window, GLFW_CURSOR, show);
}

glm::vec2 Window::GetMousePos() const
{
	return d_mousePos;
}

glm::vec2 Window::GetMouseOffset() const
{
	return d_mouseOffset;
}

void Window::SetWindowName(const std::string& name)
{
	d_data.name = name;
	glfwSetWindowTitle(d_impl->window, name.c_str());
}

std::string Window::GetWindowName() const
{
	return d_data.name;
}

void Window::SetCallback(EventCallback cb)
{
	d_data.callback = cb;
}

const char* Window::GetClipboardData()
{
	return glfwGetClipboardString(d_impl->window);
}

void Window::SetClipboardData(const std::string& text)
{
	glfwSetClipboardString(d_impl->window, text.c_str());
}

void* Window::NativeWindow() const
{
	return d_impl->window;
}

}