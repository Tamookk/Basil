#include "pch.h"
#include "Core/Input.h"

namespace Basil
{
	// All functions call the implementation (which is platform-specific)
	// Return if key is pressed
	bool Input::isKeyPressed(KeyCode keycode)
	{
		return instance->isKeyPressedImpl(keycode);
	}

	// Return if mouse button is pressed
	bool Input::isMouseButtonPressed(MouseCode mouseCode)
	{
		return instance->isMouseButtonPressedImpl(mouseCode);
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