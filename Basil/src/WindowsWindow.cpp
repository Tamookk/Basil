#include "pch.h"
#include "WindowsWindow.h"

namespace Basil
{
	static bool glfwInitialised = false;

	// Create window
	Window* Window::create(const WindowProps& props)
	{
		return new WindowsWindow(props);
	}

	// Constructor - initialise window
	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		init(props);
	}

	// Destructor - call shutdown function
	WindowsWindow::~WindowsWindow()
	{
		shutdown();
	}

	// Shutdown function
	void WindowsWindow::init(const WindowProps& props)
	{
		// Set data struct data
		data.title = props.title;
		data.width = props.width;
		data.height = props.height;

		// Log that we are creating a window
		LOG_INFO("Creating window {0} ({1}x{2})", data.title, data.width, data.height);

		// Attempt to initialise GLFW if it isn't already
		if (!glfwInitialised)
		{
			int success = glfwInit();
			ASSERT(success, "Could not initialise GLFW");
			glfwInitialised = true;
		}

		window = glfwCreateWindow(data.width, data.height, data.title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(window);
		glfwSetWindowUserPointer(window, &data);
		setVsync(true);
	}

	// Shutdown function
	void WindowsWindow::shutdown()
	{
		glfwDestroyWindow(window);
	}
	
	// On update function
	void WindowsWindow::onUpdate()
	{
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	unsigned int WindowsWindow::getWidth() const { return data.width; }
	unsigned int WindowsWindow::getHeight() const { return data.height; }
	void WindowsWindow::setEventCallback(const EventCallbackFunction& callback) { data.eventCallbackFunction = callback; }
	
	void WindowsWindow::setVsync(bool enabled)
	{
		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		data.vsync = enabled;
	}

	bool WindowsWindow::isVsync() const { return data.vsync; }
}