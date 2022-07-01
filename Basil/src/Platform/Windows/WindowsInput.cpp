#include "pch.h"
#include "Core/Application.h"
#include "Core/Input.h"

#include <GLFW/glfw3.h>

namespace Basil
{
	// Return if key is being pressed
	bool Input::isKeyPressed(KeyCode keyCode)
	{
		// Get window
		GLFWwindow* window = static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow());

		// Check to see if key is being pressed
		int state = glfwGetKey(window, static_cast<int32_t>(keyCode));
		return state == GLFW_PRESS;
	}

	// Return if mouse button is being pressed
	bool Input::isMouseButtonPressed(MouseCode mouseCode)
	{
		// Get window
		GLFWwindow* window = static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow());

		// Check to see if mouse button is being pressed
		int state = glfwGetMouseButton(window, static_cast<int32_t>(mouseCode));
		return state == GLFW_PRESS;
	}

	// Return mouse position pair
	std::pair<float, float> Input::getMousePosition()
	{
		// Get window
		GLFWwindow* window = static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow());

		// Get mouse position and return it
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		return { (float)xpos, (float)ypos };
	}

	// Return mouse X position
	float Input::getMouseX()
	{
		return getMousePosition().first;
	}

	// Return mouse Y position
	float Input::getMouseY()
	{
		return getMousePosition().second;
	}
}