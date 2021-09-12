#include "Window.h"

#include <Sprocket/Core/events.h>
#include <Sprocket/Core/Log.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cstdlib>

namespace spkt {

struct window_impl
{
	GLFWwindow* window;	
	std::uint32_t vao;
};

window::window(const std::string& name, std::uint32_t width, std::uint32_t height)
	: d_impl(std::make_unique<window_impl>())
	, d_data({name, width, height})
	, d_clear_colour({1.0, 1.0, 1.0})
{
	if (GLFW_TRUE != glfwInit()) {
		log::fatal("Failed to initialise GLFW");
		std::exit(-1);
	}

	d_impl->window = glfwCreateWindow(
		d_data.width,
		d_data.height,
		d_data.name.c_str(),
		nullptr,
		nullptr
	);

	if (!d_impl->window) {
		log::fatal("Failed to create window");
		std::exit(-2);
	}

	glfwMakeContextCurrent(d_impl->window);
	glfwSetWindowUserPointer(d_impl->window, &d_data);

	// Initialise GLAD
	if (0 == gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		log::fatal("Failed to initialise GLAD");
		std::exit(-3);
	}

	int versionMajor;
	int versionMinor;
	glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);
	glGetIntegerv(GL_MINOR_VERSION, &versionMinor);
	log::info("OpenGL version: {}.{}", versionMajor, versionMinor);

	// Set OpenGL error callback
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback([](GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void*) {
		
		switch (severity) {
			case GL_DEBUG_SEVERITY_NOTIFICATION: return;
			case GL_DEBUG_SEVERITY_LOW: {
				log::info("{}, {}, {}, {}, {}", source, type, id, length, message);
			} break;
			case GL_DEBUG_SEVERITY_MEDIUM: {
				log::warn("{}, {}, {}, {}, {}", source, type, id, length, message);
			} break;
			case GL_DEBUG_SEVERITY_HIGH: {
				log::error("{}, {}, {}, {}, {}", source, type, id, length, message);
			} break;
		}
	}, nullptr);

	// Set GLFW callbacks
	glfwSetWindowSizeCallback(d_impl->window, [](GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
		window_data* data = (window_data*)glfwGetWindowUserPointer(window);
		if (!data->focused) return;
		auto event = spkt::make_event<window_resized_event>(width, height);
		data->width = width;
		data->height = height;
		data->callback(event);
	});

	glfwSetWindowCloseCallback(d_impl->window, [](GLFWwindow* window)
	{
		window_data* data = (window_data*)glfwGetWindowUserPointer(window);
		if (!data->focused) return;
		auto event = spkt::make_event<window_closed_event>();
		data->running = false;
		data->callback(event);
	});

	glfwSetKeyCallback(d_impl->window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
		window_data* data = (window_data*)glfwGetWindowUserPointer(window);
		if (!data->focused) return;
		switch (action)
		{
			case GLFW_PRESS: {
				auto event = spkt::make_event<keyboard_pressed_event>(key, scancode, mods);
				data->callback(event);
			} break;
			case GLFW_RELEASE: {
				auto event = spkt::make_event<keyboard_released_event>(key, scancode, mods);
				data->callback(event);
			} break;
			case GLFW_REPEAT: {
				auto event = spkt::make_event<keyboard_held_event>(key, scancode, mods);
				data->callback(event);
			} break;
		}
	});

	glfwSetMouseButtonCallback(d_impl->window, [](GLFWwindow* window, int button, int action, int mods) {
		window_data* data = (window_data*)glfwGetWindowUserPointer(window);
		if (!data->focused) return;
		switch (action)
		{
		case GLFW_PRESS: {
			auto event = spkt::make_event<mouse_pressed_event>(button, action, mods);
			data->callback(event);
		} break;
		case GLFW_RELEASE: {
			auto event = spkt::make_event<mouse_released_event>(button, action, mods);
			data->callback(event);
		} break;
		}
	});

	glfwSetCursorPosCallback(d_impl->window, [](GLFWwindow* window, double x_pos, double y_pos) {
		window_data* data = (window_data*)glfwGetWindowUserPointer(window);
		if (!data->focused) return;
		auto event = spkt::make_event<mouse_moved_event>(x_pos, y_pos);
		data->callback(event);
	});

	glfwSetScrollCallback(d_impl->window, [](GLFWwindow* window, double x_offset, double y_offset) {
		window_data* data = (window_data*)glfwGetWindowUserPointer(window);
		if (!data->focused) return;
		auto event = spkt::make_event<mouse_scrolled_event>(x_offset, y_offset);
		data->callback(event);
	});

	glfwSetWindowFocusCallback(d_impl->window, [](GLFWwindow* window, int focused) {
		window_data* data = (window_data*)glfwGetWindowUserPointer(window);
		if (focused) {
			auto event = spkt::make_event<window_focused_event>();
			data->focused = true;
			data->callback(event);
		}
		else {
			auto event = spkt::make_event<window_unfocused_event>();
			data->focused = false;
			data->callback(event);
		}
	});

	glfwSetWindowMaximizeCallback(d_impl->window, [](GLFWwindow* window, int maximized) {
		window_data* data = (window_data*)glfwGetWindowUserPointer(window);
		if (maximized) {
			auto event = spkt::make_event<window_maximised_event>();
			data->callback(event);
		}
		else {
			auto event = spkt::make_event<window_minimised_event>();
			data->callback(event);
		}
	});

	glfwSetCharCallback(d_impl->window, [](GLFWwindow* window, std::uint32_t key) {
		window_data* data = (window_data*)glfwGetWindowUserPointer(window);
		if (!data->focused) return;
		auto event = spkt::make_event<keyboard_typed_event>(key);
		data->callback(event);
	});

	glGenVertexArrays(1, &d_impl->vao);
	glBindVertexArray(d_impl->vao);
}

window::~window()
{
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &d_impl->vao);
	
	glfwDestroyWindow(d_impl->window);
	glfwTerminate();
}

void window::end_frame()
{
	glfwSwapBuffers(d_impl->window);
	glfwPollEvents();

	double x, y;
	glfwGetCursorPos(d_impl->window, &x, &y);
	glm::vec2 newMousePos{x, y};

	d_mouse_offset = newMousePos - d_mouse_position;
	d_mouse_position = newMousePos;
}

void window::begin_frame()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glClearColor(d_clear_colour.r, d_clear_colour.g, d_clear_colour.b, 1.0);
}

void window::SetClearColour(const glm::vec3& colour)
{
	d_clear_colour = colour;
}

void window::SetCursorVisibility(bool visibility)
{
	int show = visibility ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED;
	glfwSetInputMode(d_impl->window, GLFW_CURSOR, show);
}

void window::SetFullscreen()
{
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);
	glfwSetWindowMonitor(
		d_impl->window,
		monitor,
		0,
		0,
		mode->width,
		mode->height,
		mode->refreshRate);
	d_data.fullscreen = true;
}

void window::SetWindowed(int width, int height)
{
	glfwSetWindowMonitor(d_impl->window, nullptr, 50, 50, width, height, 0);
	d_data.fullscreen = false;
}

bool window::IsFullscreen() const
{
	return d_data.fullscreen;
}

glm::vec2 window::GetMousePos() const
{
	return d_mouse_position;
}

glm::vec2 window::GetMouseOffset() const
{
	return d_mouse_offset;
}

void window::SetWindowName(const std::string& name)
{
	d_data.name = name;
	glfwSetWindowTitle(d_impl->window, name.c_str());
}

std::string window::GetWindowName() const
{
	return d_data.name;
}

void window::SetCallback(event_handler cb)
{
	d_data.callback = cb;
}

const char* window::GetClipboardData() const
{
	return glfwGetClipboardString(d_impl->window);
}

void window::SetClipboardData(const std::string& text) const
{
	glfwSetClipboardString(d_impl->window, text.c_str());
}

void* window::NativeWindow() const
{
	return d_impl->window;
}

}