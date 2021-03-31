#include "pch.h"
#include "Input.h"

namespace Basil
{
	// All functions call the implementation (which is platform-specific)
	// Return if key is pressed
	bool Input::isKeyPressed(int keycode)
	{
		return instance->isKeyPressedImpl(keycode);
	}

	// Return if mouse button is pressed
	bool Input::isMouseButtonPressed(int button)
	{
		return instance->isMouseButtonPressedImpl(button);
	}

	// Return mouse X position
	float Input::getMouseX()
	{
		return instance->getMouseXImpl();
	}

	// Return mouse Y position
	float Input::getMouseY()
	{
		return instance->getMouseYImpl();
	}
}