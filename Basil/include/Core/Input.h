/*
 * This file defines the Input base class that all other input classes
 * inherit from. It is a singleton and involves multiple static functions
 * for polling input.
 */
#pragma once

#include <utility>

#include "Core/Core.h"

namespace Basil
{
	class BASIL_API Input
	{
		public:
			static bool isKeyPressed(int keycode);
			static bool isMouseButtonPressed(int button);
			static float getMouseX();
			static float getMouseY();
		protected:
			virtual bool isKeyPressedImpl(int keycode) = 0;
			virtual bool isMouseButtonPressedImpl(int button) = 0;
			virtual std::pair<float, float> getMousePositionImpl() = 0;
			virtual float getMouseXImpl() = 0;
			virtual float getMouseYImpl() = 0;
		private:
			static Input* instance;
	};
}