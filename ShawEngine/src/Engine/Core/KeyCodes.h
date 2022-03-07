#pragma once

namespace ShawEngine
{
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

	inline std::ostream& operator<<(std::ostream& os, KeyCode keyCode)
	{
		os << static_cast<int32_t>(keyCode);
		return os;
	}
}

// From glfw3.h
#define SE_KEY_SPACE           ::ShawEngine::Key::Space
#define SE_KEY_APOSTROPHE      ::ShawEngine::Key::Apostrophe    /* ' */
#define SE_KEY_COMMA           ::ShawEngine::Key::Comma         /* , */
#define SE_KEY_MINUS           ::ShawEngine::Key::Minus         /* - */
#define SE_KEY_PERIOD          ::ShawEngine::Key::Period        /* . */
#define SE_KEY_SLASH           ::ShawEngine::Key::Slash         /* / */
#define SE_KEY_0               ::ShawEngine::Key::D0
#define SE_KEY_1               ::ShawEngine::Key::D1
#define SE_KEY_2               ::ShawEngine::Key::D2
#define SE_KEY_3               ::ShawEngine::Key::D3
#define SE_KEY_4               ::ShawEngine::Key::D4
#define SE_KEY_5               ::ShawEngine::Key::D5
#define SE_KEY_6               ::ShawEngine::Key::D6
#define SE_KEY_7               ::ShawEngine::Key::D7
#define SE_KEY_8               ::ShawEngine::Key::D8
#define SE_KEY_9               ::ShawEngine::Key::D9
#define SE_KEY_SEMICOLON       ::ShawEngine::Key::Semicolon     /* ; */
#define SE_KEY_EQUAL           ::ShawEngine::Key::Equal         /* = */
#define SE_KEY_A               ::ShawEngine::Key::A
#define SE_KEY_B               ::ShawEngine::Key::B
#define SE_KEY_C               ::ShawEngine::Key::C
#define SE_KEY_D               ::ShawEngine::Key::D
#define SE_KEY_E               ::ShawEngine::Key::E
#define SE_KEY_F               ::ShawEngine::Key::F
#define SE_KEY_G               ::ShawEngine::Key::G
#define SE_KEY_H               ::ShawEngine::Key::H
#define SE_KEY_I               ::ShawEngine::Key::I
#define SE_KEY_J               ::ShawEngine::Key::J
#define SE_KEY_K               ::ShawEngine::Key::K
#define SE_KEY_L               ::ShawEngine::Key::L
#define SE_KEY_M               ::ShawEngine::Key::M
#define SE_KEY_N               ::ShawEngine::Key::N
#define SE_KEY_O               ::ShawEngine::Key::O
#define SE_KEY_P               ::ShawEngine::Key::P
#define SE_KEY_Q               ::ShawEngine::Key::Q
#define SE_KEY_R               ::ShawEngine::Key::R
#define SE_KEY_S               ::ShawEngine::Key::S
#define SE_KEY_T               ::ShawEngine::Key::T
#define SE_KEY_U               ::ShawEngine::Key::U
#define SE_KEY_V               ::ShawEngine::Key::V
#define SE_KEY_W               ::ShawEngine::Key::W
#define SE_KEY_X               ::ShawEngine::Key::X
#define SE_KEY_Y               ::ShawEngine::Key::Y
#define SE_KEY_Z               ::ShawEngine::Key::Z
#define SE_KEY_LEFT_BRACKET    ::ShawEngine::Key::LeftBracket   /* [ */
#define SE_KEY_BACKSLASH       ::ShawEngine::Key::Backslash     /* \ */
#define SE_KEY_RIGHT_BRACKET   ::ShawEngine::Key::RightBracket  /* ] */
#define SE_KEY_GRAVE_ACCENT    ::ShawEngine::Key::GraveAccent   /* ` */
#define SE_KEY_WORLD_1         ::ShawEngine::Key::World1        /* non-US #1 */
#define SE_KEY_WORLD_2         ::ShawEngine::Key::World2        /* non-US #2 */

/* Function keys */
#define SE_KEY_ESCAPE          ::ShawEngine::Key::Escape
#define SE_KEY_ENTER           ::ShawEngine::Key::Enter
#define SE_KEY_TAB             ::ShawEngine::Key::Tab
#define SE_KEY_BACKSPACE       ::ShawEngine::Key::Backspace
#define SE_KEY_INSERT          ::ShawEngine::Key::Insert
#define SE_KEY_DELETE          ::ShawEngine::Key::Delete
#define SE_KEY_RIGHT           ::ShawEngine::Key::Right
#define SE_KEY_LEFT            ::ShawEngine::Key::Left
#define SE_KEY_DOWN            ::ShawEngine::Key::Down
#define SE_KEY_UP              ::ShawEngine::Key::Up
#define SE_KEY_PAGE_UP         ::ShawEngine::Key::PageUp
#define SE_KEY_PAGE_DOWN       ::ShawEngine::Key::PageDown
#define SE_KEY_HOME            ::ShawEngine::Key::Home
#define SE_KEY_END             ::ShawEngine::Key::End
#define SE_KEY_CAPS_LOCK       ::ShawEngine::Key::CapsLock
#define SE_KEY_SCROLL_LOCK     ::ShawEngine::Key::ScrollLock
#define SE_KEY_NUM_LOCK        ::ShawEngine::Key::NumLock
#define SE_KEY_PRINT_SCREEN    ::ShawEngine::Key::PrintScreen
#define SE_KEY_PAUSE           ::ShawEngine::Key::Pause
#define SE_KEY_F1              ::ShawEngine::Key::F1
#define SE_KEY_F2              ::ShawEngine::Key::F2
#define SE_KEY_F3              ::ShawEngine::Key::F3
#define SE_KEY_F4              ::ShawEngine::Key::F4
#define SE_KEY_F5              ::ShawEngine::Key::F5
#define SE_KEY_F6              ::ShawEngine::Key::F6
#define SE_KEY_F7              ::ShawEngine::Key::F7
#define SE_KEY_F8              ::ShawEngine::Key::F8
#define SE_KEY_F9              ::ShawEngine::Key::F9
#define SE_KEY_F10             ::ShawEngine::Key::F10
#define SE_KEY_F11             ::ShawEngine::Key::F11
#define SE_KEY_F12             ::ShawEngine::Key::F12
#define SE_KEY_F13             ::ShawEngine::Key::F13
#define SE_KEY_F14             ::ShawEngine::Key::F14
#define SE_KEY_F15             ::ShawEngine::Key::F15
#define SE_KEY_F16             ::ShawEngine::Key::F16
#define SE_KEY_F17             ::ShawEngine::Key::F17
#define SE_KEY_F18             ::ShawEngine::Key::F18
#define SE_KEY_F19             ::ShawEngine::Key::F19
#define SE_KEY_F20             ::ShawEngine::Key::F20
#define SE_KEY_F21             ::ShawEngine::Key::F21
#define SE_KEY_F22             ::ShawEngine::Key::F22
#define SE_KEY_F23             ::ShawEngine::Key::F23
#define SE_KEY_F24             ::ShawEngine::Key::F24
#define SE_KEY_F25             ::ShawEngine::Key::F25

/* Keypad */
#define SE_KEY_KP_0            ::ShawEngine::Key::KP0
#define SE_KEY_KP_1            ::ShawEngine::Key::KP1
#define SE_KEY_KP_2            ::ShawEngine::Key::KP2
#define SE_KEY_KP_3            ::ShawEngine::Key::KP3
#define SE_KEY_KP_4            ::ShawEngine::Key::KP4
#define SE_KEY_KP_5            ::ShawEngine::Key::KP5
#define SE_KEY_KP_6            ::ShawEngine::Key::KP6
#define SE_KEY_KP_7            ::ShawEngine::Key::KP7
#define SE_KEY_KP_8            ::ShawEngine::Key::KP8
#define SE_KEY_KP_9            ::ShawEngine::Key::KP9
#define SE_KEY_KP_DECIMAL      ::ShawEngine::Key::KPDecimal
#define SE_KEY_KP_DIVIDE       ::ShawEngine::Key::KPDivide
#define SE_KEY_KP_MULTIPLY     ::ShawEngine::Key::KPMultiply
#define SE_KEY_KP_SUBTRACT     ::ShawEngine::Key::KPSubtract
#define SE_KEY_KP_ADD          ::ShawEngine::Key::KPAdd
#define SE_KEY_KP_ENTER        ::ShawEngine::Key::KPEnter
#define SE_KEY_KP_EQUAL        ::ShawEngine::Key::KPEqual

#define SE_KEY_LEFT_SHIFT      ::ShawEngine::Key::LeftShift
#define SE_KEY_LEFT_CONTROL    ::ShawEngine::Key::LeftControl
#define SE_KEY_LEFT_ALT        ::ShawEngine::Key::LeftAlt
#define SE_KEY_LEFT_SUPER      ::ShawEngine::Key::LeftSuper
#define SE_KEY_RIGHT_SHIFT     ::ShawEngine::Key::RightShift
#define SE_KEY_RIGHT_CONTROL   ::ShawEngine::Key::RightControl
#define SE_KEY_RIGHT_ALT       ::ShawEngine::Key::RightAlt
#define SE_KEY_RIGHT_SUPER     ::ShawEngine::Key::RightSuper
#define SE_KEY_MENU            ::ShawEngine::Key::Menu