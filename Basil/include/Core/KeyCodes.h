/*
 * This header file defines keyboard codes for the Basil game engine.
 */
#pragma once

#include <ostream>

namespace Basil
{
	// Define keycodes
	typedef enum class KeyCode : uint16_t
	{
		// From glfw3.h
		Space = 32,
		Apostrophe = 39, /* ' */
		Comma = 44, /* , */
		Minus = 45, /* - */
		Period = 46, /* . */
		Slash = 47, /* / */

		D0 = 48, /* 0 */
		D1 = 49, /* 1 */
		D2 = 50, /* 2 */
		D3 = 51, /* 3 */
		D4 = 52, /* 4 */
		D5 = 53, /* 5 */
		D6 = 54, /* 6 */
		D7 = 55, /* 7 */
		D8 = 56, /* 8 */
		D9 = 57, /* 9 */

		Semicolon = 59, /* ; */
		Equal = 61, /* = */

		A = 65,
		B = 66,
		C = 67,
		D = 68,
		E = 69,
		F = 70,
		G = 71,
		H = 72,
		I = 73,
		J = 74,
		K = 75,
		L = 76,
		M = 77,
		N = 78,
		O = 79,
		P = 80,
		Q = 81,
		R = 82,
		S = 83,
		T = 84,
		U = 85,
		V = 86,
		W = 87,
		X = 88,
		Y = 89,
		Z = 90,

		LeftBracket = 91,  /* [ */
		Backslash = 92,  /* \ */
		RightBracket = 93,  /* ] */
		GraveAccent = 96,  /* ` */

		World1 = 161, /* non-US #1 */
		World2 = 162, /* non-US #2 */

		/* Function keys */
		Escape = 256,
		Enter = 257,
		Tab = 258,
		Backspace = 259,
		Insert = 260,
		Delete = 261,
		Right = 262,
		Left = 263,
		Down = 264,
		Up = 265,
		PageUp = 266,
		PageDown = 267,
		Home = 268,
		End = 269,
		CapsLock = 280,
		ScrollLock = 281,
		NumLock = 282,
		PrintScreen = 283,
		Pause = 284,
		F1 = 290,
		F2 = 291,
		F3 = 292,
		F4 = 293,
		F5 = 294,
		F6 = 295,
		F7 = 296,
		F8 = 297,
		F9 = 298,
		F10 = 299,
		F11 = 300,
		F12 = 301,
		F13 = 302,
		F14 = 303,
		F15 = 304,
		F16 = 305,
		F17 = 306,
		F18 = 307,
		F19 = 308,
		F20 = 309,
		F21 = 310,
		F22 = 311,
		F23 = 312,
		F24 = 313,
		F25 = 314,

		/* Keypad */
		KP0 = 320,
		KP1 = 321,
		KP2 = 322,
		KP3 = 323,
		KP4 = 324,
		KP5 = 325,
		KP6 = 326,
		KP7 = 327,
		KP8 = 328,
		KP9 = 329,
		KPDecimal = 330,
		KPDivide = 331,
		KPMultiply = 332,
		KPSubtract = 333,
		KPAdd = 334,
		KPEnter = 335,
		KPEqual = 336,

		LeftShift = 340,
		LeftControl = 341,
		LeftAlt = 342,
		LeftSuper = 343,
		RightShift = 344,
		RightControl = 345,
		RightAlt = 346,
		RightSuper = 347,
		Menu = 348
	} Key;

	// Operator <<
	inline std::ostream& operator<<(std::ostream& os, KeyCode keyCode)
	{
		os << static_cast<int32_t>(keyCode);
		return os;
	}
}

// From glfw3.h
#define HZ_KEY_SPACE           ::Basil::Key::Space
#define HZ_KEY_APOSTROPHE      ::Basil::Key::Apostrophe    /* ' */
#define HZ_KEY_COMMA           ::Basil::Key::Comma         /* , */
#define HZ_KEY_MINUS           ::Basil::Key::Minus         /* - */
#define HZ_KEY_PERIOD          ::Basil::Key::Period        /* . */
#define HZ_KEY_SLASH           ::Basil::Key::Slash         /* / */
#define HZ_KEY_0               ::Basil::Key::D0
#define HZ_KEY_1               ::Basil::Key::D1
#define HZ_KEY_2               ::Basil::Key::D2
#define HZ_KEY_3               ::Basil::Key::D3
#define HZ_KEY_4               ::Basil::Key::D4
#define HZ_KEY_5               ::Basil::Key::D5
#define HZ_KEY_6               ::Basil::Key::D6
#define HZ_KEY_7               ::Basil::Key::D7
#define HZ_KEY_8               ::Basil::Key::D8
#define HZ_KEY_9               ::Basil::Key::D9
#define HZ_KEY_SEMICOLON       ::Basil::Key::Semicolon     /* ; */
#define HZ_KEY_EQUAL           ::Basil::Key::Equal         /* = */
#define HZ_KEY_A               ::Basil::Key::A
#define HZ_KEY_B               ::Basil::Key::B
#define HZ_KEY_C               ::Basil::Key::C
#define HZ_KEY_D               ::Basil::Key::D
#define HZ_KEY_E               ::Basil::Key::E
#define HZ_KEY_F               ::Basil::Key::F
#define HZ_KEY_G               ::Basil::Key::G
#define HZ_KEY_H               ::Basil::Key::H
#define HZ_KEY_I               ::Basil::Key::I
#define HZ_KEY_J               ::Basil::Key::J
#define HZ_KEY_K               ::Basil::Key::K
#define HZ_KEY_L               ::Basil::Key::L
#define HZ_KEY_M               ::Basil::Key::M
#define HZ_KEY_N               ::Basil::Key::N
#define HZ_KEY_O               ::Basil::Key::O
#define HZ_KEY_P               ::Basil::Key::P
#define HZ_KEY_Q               ::Basil::Key::Q
#define HZ_KEY_R               ::Basil::Key::R
#define HZ_KEY_S               ::Basil::Key::S
#define HZ_KEY_T               ::Basil::Key::T
#define HZ_KEY_U               ::Basil::Key::U
#define HZ_KEY_V               ::Basil::Key::V
#define HZ_KEY_W               ::Basil::Key::W
#define HZ_KEY_X               ::Basil::Key::X
#define HZ_KEY_Y               ::Basil::Key::Y
#define HZ_KEY_Z               ::Basil::Key::Z
#define HZ_KEY_LEFT_BRACKET    ::Basil::Key::LeftBracket   /* [ */
#define HZ_KEY_BACKSLASH       ::Basil::Key::Backslash     /* \ */
#define HZ_KEY_RIGHT_BRACKET   ::Basil::Key::RightBracket  /* ] */
#define HZ_KEY_GRAVE_ACCENT    ::Basil::Key::GraveAccent   /* ` */
#define HZ_KEY_WORLD_1         ::Basil::Key::World1        /* non-US #1 */
#define HZ_KEY_WORLD_2         ::Basil::Key::World2        /* non-US #2 */

/* Function keys */
#define HZ_KEY_ESCAPE          ::Basil::Key::Escape
#define HZ_KEY_ENTER           ::Basil::Key::Enter
#define HZ_KEY_TAB             ::Basil::Key::Tab
#define HZ_KEY_BACKSPACE       ::Basil::Key::Backspace
#define HZ_KEY_INSERT          ::Basil::Key::Insert
#define HZ_KEY_DELETE          ::Basil::Key::Delete
#define HZ_KEY_RIGHT           ::Basil::Key::Right
#define HZ_KEY_LEFT            ::Basil::Key::Left
#define HZ_KEY_DOWN            ::Basil::Key::Down
#define HZ_KEY_UP              ::Basil::Key::Up
#define HZ_KEY_PAGE_UP         ::Basil::Key::PageUp
#define HZ_KEY_PAGE_DOWN       ::Basil::Key::PageDown
#define HZ_KEY_HOME            ::Basil::Key::Home
#define HZ_KEY_END             ::Basil::Key::End
#define HZ_KEY_CAPS_LOCK       ::Basil::Key::CapsLock
#define HZ_KEY_SCROLL_LOCK     ::Basil::Key::ScrollLock
#define HZ_KEY_NUM_LOCK        ::Basil::Key::NumLock
#define HZ_KEY_PRINT_SCREEN    ::Basil::Key::PrintScreen
#define HZ_KEY_PAUSE           ::Basil::Key::Pause
#define HZ_KEY_F1              ::Basil::Key::F1
#define HZ_KEY_F2              ::Basil::Key::F2
#define HZ_KEY_F3              ::Basil::Key::F3
#define HZ_KEY_F4              ::Basil::Key::F4
#define HZ_KEY_F5              ::Basil::Key::F5
#define HZ_KEY_F6              ::Basil::Key::F6
#define HZ_KEY_F7              ::Basil::Key::F7
#define HZ_KEY_F8              ::Basil::Key::F8
#define HZ_KEY_F9              ::Basil::Key::F9
#define HZ_KEY_F10             ::Basil::Key::F10
#define HZ_KEY_F11             ::Basil::Key::F11
#define HZ_KEY_F12             ::Basil::Key::F12
#define HZ_KEY_F13             ::Basil::Key::F13
#define HZ_KEY_F14             ::Basil::Key::F14
#define HZ_KEY_F15             ::Basil::Key::F15
#define HZ_KEY_F16             ::Basil::Key::F16
#define HZ_KEY_F17             ::Basil::Key::F17
#define HZ_KEY_F18             ::Basil::Key::F18
#define HZ_KEY_F19             ::Basil::Key::F19
#define HZ_KEY_F20             ::Basil::Key::F20
#define HZ_KEY_F21             ::Basil::Key::F21
#define HZ_KEY_F22             ::Basil::Key::F22
#define HZ_KEY_F23             ::Basil::Key::F23
#define HZ_KEY_F24             ::Basil::Key::F24
#define HZ_KEY_F25             ::Basil::Key::F25

/* Keypad */
#define HZ_KEY_KP_0            ::Basil::Key::KP0
#define HZ_KEY_KP_1            ::Basil::Key::KP1
#define HZ_KEY_KP_2            ::Basil::Key::KP2
#define HZ_KEY_KP_3            ::Basil::Key::KP3
#define HZ_KEY_KP_4            ::Basil::Key::KP4
#define HZ_KEY_KP_5            ::Basil::Key::KP5
#define HZ_KEY_KP_6            ::Basil::Key::KP6
#define HZ_KEY_KP_7            ::Basil::Key::KP7
#define HZ_KEY_KP_8            ::Basil::Key::KP8
#define HZ_KEY_KP_9            ::Basil::Key::KP9
#define HZ_KEY_KP_DECIMAL      ::Basil::Key::KPDecimal
#define HZ_KEY_KP_DIVIDE       ::Basil::Key::KPDivide
#define HZ_KEY_KP_MULTIPLY     ::Basil::Key::KPMultiply
#define HZ_KEY_KP_SUBTRACT     ::Basil::Key::KPSubtract
#define HZ_KEY_KP_ADD          ::Basil::Key::KPAdd
#define HZ_KEY_KP_ENTER        ::Basil::Key::KPEnter
#define HZ_KEY_KP_EQUAL        ::Basil::Key::KPEqual

#define HZ_KEY_LEFT_SHIFT      ::Basil::Key::LeftShift
#define HZ_KEY_LEFT_CONTROL    ::Basil::Key::LeftControl
#define HZ_KEY_LEFT_ALT        ::Basil::Key::LeftAlt
#define HZ_KEY_LEFT_SUPER      ::Basil::Key::LeftSuper
#define HZ_KEY_RIGHT_SHIFT     ::Basil::Key::RightShift
#define HZ_KEY_RIGHT_CONTROL   ::Basil::Key::RightControl
#define HZ_KEY_RIGHT_ALT       ::Basil::Key::RightAlt
#define HZ_KEY_RIGHT_SUPER     ::Basil::Key::RightSuper
#define HZ_KEY_MENU            ::Basil::Key::Menu