#include "pch.h"
#include "Platform/Windows/WindowsInput.h"

namespace Basil
{
	Unique<Input> Input::instance = makeUnique<WindowsInput>();

	// Return if key is being pressed
	bool WindowsInput::isKeyPressedImpl(KeyCode keyCode)
	{
		// Get window
		GLFWwindow* window = static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow());

		// Check to see if key is being pressed
		int state = glfwGetKey(window, static_cast<int32_t>(keyCode));
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	// Return if mouse button is being pressed
	bool WindowsInput::isMouseButtonPressedImpl(MouseCode mouseCode)
	{
		// Get window
		GLFWwindow* window = static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow());

		// Check to see if mouse button is being pressed
		int state = glfwGetMouseButton(window, static_cast<int32_t>(mouseCode));
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