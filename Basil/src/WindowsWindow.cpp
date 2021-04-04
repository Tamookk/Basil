#include "pch.h"
#include "WindowsWindow.h"

namespace Basil
{
	static bool glfwInitialised = false;

	// Error callback function
	static void glfwErrorCallback(int errorCode, const char* description)
	{
		LOG_ERROR("GLFW Error #{0}: {1}", errorCode, description);
	}

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
			glfwSetErrorCallback(glfwErrorCallback);
			glfwInitialised = true;
		}

		// GLFW window stuff
		window = glfwCreateWindow(data.width, data.height, data.title.c_str(), nullptr, nullptr);
		context = new OpenGLContext(window);
		context->init();

		// Some more GLFW window stuff
		glfwSetWindowUserPointer(window, &data);
		setVsync(true);

		// Set GLFW callbacks
		glfwSetWindowCloseCallback(window, [](GLFWwindow* window)
		{
			// Get address of data struct
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			// Create window close event
			WindowCloseEvent event;

			// Call the event callback function
			data.eventCallbackFunction(event);
		});
		
		glfwSetWindowSizeCallback(window, [](GLFWwindow* window, int width, int height)
		{
			// Get address of data struct
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				
			// Set width and height variables in struct
			data.width = width;
			data.height = height;

			// Create a window resize event
			WindowResizeEvent event(width, height);

			// Call the event callback function with our event as the parameter
			data.eventCallbackFunction(event);
		});

		glfwSetWindowFocusCallback(window, [](GLFWwindow* window, int focus)
		{
			// Get address of data struct
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				
			// Respond - true if window has gained focus
			if (focus == GLFW_TRUE)
			{
				WindowFocusEvent event;
				data.eventCallbackFunction(event);
			}
			else if (focus == GLFW_FALSE)
			{
				WindowLostFocusEvent event;
				data.eventCallbackFunction(event);
			}
		});

		glfwSetWindowPosCallback(window, [](GLFWwindow* window, int xpos, int ypos)
		{
			// Get data struct, create window moved event, and call event callback function
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			WindowMovedEvent event;
			data.eventCallbackFunction(event);
		});

		glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			// Get address of data struct
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			// Create different events depending on the action
			switch (action)
			{
				case GLFW_PRESS:
				{
					KeyPressedEvent event(key, 0);
					data.eventCallbackFunction(event);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleasedEvent event(key);
					data.eventCallbackFunction(event);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyPressedEvent event(key, 1);
					data.eventCallbackFunction(event);
					break;
				}
			}
		});

		glfwSetCharCallback(window, [](GLFWwindow* window, unsigned int keycode)
		{
			// Get address of data struct
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			// Process event
			KeyTypedEvent event(keycode);
			data.eventCallbackFunction(event);
		});

		glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods)
		{
			// Get address of the data struct
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			// Create different events depending on the action
			switch (action)
			{
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent event(button);
					data.eventCallbackFunction(event);
					break;
				}
				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent event(button);
					data.eventCallbackFunction(event);
					break;
				}
			}
		});

		glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xpos, double ypos)
		{
			// Get address of data struct
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			// Create mouse moved event
			MouseMovedEvent event((float)xpos, (float)ypos);

			// Call the event callback function
			data.eventCallbackFunction(event);
		});

		glfwSetScrollCallback(window, [](GLFWwindow* window, double xOffset, double yOffset)
		{
			// Get address of data struct
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			// Create mouse scrolled event
			MouseScrolledEvent event((float)xOffset, (float)yOffset);

			// Call the event callback function
			data.eventCallbackFunction(event);
		});
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
		context->swapBuffers();
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

	void* WindowsWindow::getNativeWindow() const { return window; }
}