#include "pch.h"
#include "Core/Input.h"

#ifdef PLATFORM_WINDOWS
	#include "Platform/Windows/WindowsInput.h"
#endif

namespace Basil
{
	// All functions call the implementation (which is platform-specific)
	// Create input instance
	Unique<Input> Input::instance = Input::create();
	Unique<Input> Input::create()
	{
		#ifdef PLATFORM_WINDOWS
			return makeUnique<WindowsInput>();
		#else
			ASSERT(false, "Unknown platform.");
			return nullptr;
		#endif
	}

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