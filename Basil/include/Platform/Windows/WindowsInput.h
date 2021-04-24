/*
 * This header file is the Windows implementation of Input.
 */
#pragma once

#include "Core/Application.h"
#include "Core/Input.h"

#include <GLFW/glfw3.h>

namespace Basil
{
	class WindowsInput : public Input
	{
		protected:
			bool isKeyPressedImpl(KeyCode keyCode) override;
			bool isMouseButtonPressedImpl(MouseCode mouseCode) override;
			std::pair<float, float> getMousePositionImpl() override;
			float getMouseXImpl() override;
			float getMouseYImpl() override;
	};
}