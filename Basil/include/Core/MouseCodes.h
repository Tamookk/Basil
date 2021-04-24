/*
 * This header file contains mouse button codes for the Basil game engine.
 */
#pragma once

namespace Basil
{
	typedef enum class MouseCode : uint16_t
	{
		// From glfw3.h
		Button0 = 0,
		Button1 = 1,
		Button2 = 2,
		Button3 = 3,
		Button4 = 4,
		Button5 = 5,
		Button6 = 6,
		Button7 = 7,

		ButtonLast = Button7,
		ButtonLeft = Button0,
		ButtonRight = Button1,
		ButtonMiddle = Button2
	} Mouse;

	inline std::ostream& operator<<(std::ostream& os, MouseCode mouseCode)
	{
		os << static_cast<int32_t>(mouseCode);
		return os;
	}
}

// From glfw3.h
#define BASIL_MOUSE_BUTTON_0		::Basil::MouseCode::Button0
#define BASIL_MOUSE_BUTTON_1		::Basil::MouseCode::Button1
#define BASIL_MOUSE_BUTTON_2		::Basil::MouseCode::Button2
#define BASIL_MOUSE_BUTTON_3		::Basil::MouseCode::Button3
#define BASIL_MOUSE_BUTTON_4		::Basil::MouseCode::Button4
#define BASIL_MOUSE_BUTTON_5		::Basil::MouseCode::Button5
#define BASIL_MOUSE_BUTTON_6		::Basil::MouseCode::Button6
#define BASIL_MOUSE_BUTTON_7		::Basil::MouseCode::Button7
#define BASIL_MOUSE_BUTTON_LEFT		::Basil::MouseCode::ButtonLeft
#define BASIL_MOUSE_BUTTON_RIGHT	::Basil::MouseCode::ButtonRight
#define BASIL_MOUSE_BUTTON_MIDDLE	::Basil::MouseCode::ButtonMiddle