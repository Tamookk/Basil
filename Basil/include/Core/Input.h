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
			static Unique<Input> create();
			static bool isKeyPressed(KeyCode keyCode);
			static bool isMouseButtonPressed(MouseCode mouseCode);
			static float getMouseX();
			static float getMouseY();
		protected:
			virtual bool isKeyPressedImpl(KeyCode keyCode) = 0;
			virtual bool isMouseButtonPressedImpl(MouseCode mouseCode) = 0;
			virtual std::pair<float, float> getMousePositionImpl() = 0;
			virtual float getMouseXImpl() = 0;
			virtual float getMouseYImpl() = 0;
		private:
			static Unique<Input> instance;
	};
}