#include "pch.h"
#include "WindowsInput.h"

namespace Basil
{
	Input* Input::instance = new WindowsInput();

	// Return if key is being pressed
	bool WindowsInput::isKeyPressedImpl(int keycode)
	{
		// Get window
		GLFWwindow* window = static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow());

		// Check to see if key is being pressed
		int state = glfwGetKey(window, keycode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	// Return if mouse button is being pressed
	bool WindowsInput::isMouseButtonPressedImpl(int button)
	{
		// Get window
		GLFWwindow* window = static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow());

		// Check to see if mouse button is being pressed
		int state = glfwGetMouseButton(window, button);
		return state == GLFW_PRESS;
	}

	// Return mouse position pair
	std::pair<float, float> WindowsInput::getMousePositionImpl()
	{
		// Get window
		GLFWwindow* window = static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow());

		// Get mouse position and return it
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		return { (float)xpos, (float)ypos };
	}

	// Return mouse X position
	float WindowsInput::getMouseXImpl()
	{
		return getMousePositionImpl().first;
	}

	// Return mouse Y position
	float WindowsInput::getMouseYImpl()
	{
		return getMousePositionImpl().second;
	}
}