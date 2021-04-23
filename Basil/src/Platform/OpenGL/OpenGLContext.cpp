#include "pch.h"
#include "Platform/OpenGL/OpenGLContext.h"

namespace Basil
{
	// Constructor
	OpenGLContext::OpenGLContext(GLFWwindow* window)
	{
		this->window = window;
		ASSERT(window, "Window is null.");
	}

	// Initialise context
	void OpenGLContext::init()
	{
		PROFILE_FUNCTION();

		glfwMakeContextCurrent(window);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		ASSERT(status, "Failed to initialise Glad!");
	}

	// Swap buffers
	void OpenGLContext::swapBuffers()
	{
		PROFILE_FUNCTION();

		glfwSwapBuffers(window);
	}
}