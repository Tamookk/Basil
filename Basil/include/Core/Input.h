/*
 * This file defines the Input base class that all other input classes
 * inherit from. It is a singleton and involves multiple static functions
 * for polling input.
 */
#pragma once

#include <utility>

#include "Core/Core.h"
#include "Core/KeyCodes.h"
#include "Core/MouseCodes.h"

namespace Basil
{
	class Input
	{
		public:
			static bool isKeyPressed(KeyCode keyCode);
			static bool isMouseButtonPressed(MouseCode mouseCode);
			static std::pair<float, float> getMousePosition();
			static float getMouseX();
			static float getMouseY();
	};
}