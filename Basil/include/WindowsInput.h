#pragma once

#include "Application.h"
#include "Input.h"

#include <GLFW/glfw3.h>

namespace Basil
{
	class WindowsInput : public Input
	{
		protected:
			bool isKeyPressedImpl(int keycode) override;
			bool isMouseButtonPressedImpl(int button) override;
			std::pair<float, float> getMousePositionImpl() override;
			float getMouseXImpl() override;
			float getMouseYImpl() override;
	};
}