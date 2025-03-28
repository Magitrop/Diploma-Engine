#pragma once

#include <cstdint>

namespace engine
{
	enum class KeyModifiers : std::uint8_t
	{
		None = 0,
		Shift = 1 << 0,
		Ctrl = 1 << 1,
		Alt = 1 << 2
	};

	enum class KeyCode : std::uint16_t
	{
		Unknown			= static_cast<std::uint16_t>(-1),
		Space			= 32,
		Apostrophe		= 39, /* ' */
		Quote			= 39, /* ' */
		Comma			= 44, /* , */
		Minus			= 45, /* - */
		Period			= 46, /* . */
		Slash			= 47, /* / */
		Key0			= 48, /* Digit 0 */
		Key1			= 49, /* Digit 1 */
		Key2			= 50, /* Digit 2 */
		Key3			= 51, /* Digit 3 */
		Key4			= 52, /* Digit 4 */
		Key5			= 53, /* Digit 5 */
		Key6			= 54, /* Digit 6 */
		Key7			= 55, /* Digit 7 */
		Key8			= 56, /* Digit 8 */
		Key9			= 57, /* Digit 9 */
		Semicolon		= 59, /* ; */
		Equal			= 61, /* = */
		A				= 65,
		B				= 66,
		C				= 67,
		D				= 68,
		E				= 69,
		F				= 70,
		G				= 71,
		H				= 72,
		I				= 73,
		J				= 74,
		K				= 75,
		L				= 76,
		M				= 77,
		N				= 78,
		O				= 79,
		P				= 80,
		Q				= 81,
		R				= 82,
		S				= 83,
		T				= 84,
		U				= 85,
		V				= 86,
		W				= 87,
		X				= 88,
		Y				= 89,
		Z				= 90,
		LeftBracket		= 91, /* [ */
		Backslash		= 92, /* \ */
		RightBracket	= 93, /* ] */
		BackQuote		= 96, /* ` */
		GraveAccent		= 96, /* ` */
		Escape          = 256,
		Enter           = 257,
		Tab             = 258,
		Backspace       = 259,
		Insert          = 260,
		Delete          = 261,
		RightArrow      = 262,
		LeftArrow		= 263,
		DownArrow		= 264,
		UpArrow			= 265,
		PageUp			= 266,
		PageDown		= 267,
		Home			= 268,
		End				= 269,
		CapsLock		= 280,
		ScrollLock		= 281,
		NumLock			= 282,
		PrintScreen		= 283,
		Pause			= 284,
		F1              = 290,
		F2              = 291,
		F3              = 292,
		F4              = 293,
		F5              = 294,
		F6              = 295,
		F7              = 296,
		F8              = 297,
		F9              = 298,
		F10             = 299,
		F11             = 300,
		F12             = 301,
		F13             = 302,
		F14             = 303,
		F15             = 304,
		F16             = 305,
		F17             = 306,
		F18             = 307,
		F19             = 308,
		F20             = 309,
		F21             = 310,
		F22             = 311,
		F23             = 312,
		F24             = 313,
		F25             = 314,
		Keypad0			= 320,
		Numpad0			= 320,
		Keypad1			= 321,
		Numpad1			= 321,
		Keypad2			= 322,
		Numpad2			= 322,
		Keypad3			= 323,
		Numpad3			= 323,
		Keypad4			= 324,
		Numpad4			= 324,
		Keypad5			= 325,
		Numpad5			= 325,
		Keypad6			= 326,
		Numpad6			= 326,
		Keypad7			= 327,
		Numpad7			= 327,
		Keypad8			= 328,
		Numpad8			= 328,
		Keypad9			= 329,
		Numpad9			= 329,
		KeypadDecimal	= 330,
		NumpadDecimal	= 330,
		KeypadDivide	= 331,
		NumpadDivide	= 331,
		KeypadMultiply	= 332,
		NumpadMultiply	= 332,
		KeypadSubtract	= 333,
		NumpadSubtract	= 333,
		KeypadAdd		= 334,
		NumpadAdd		= 334,
		KeypadEnter		= 335,
		NumpadEnter		= 335,
		KeypadEqual		= 336,
		NumpadEqual		= 336,
		LeftShift		= 340,
		LeftControl		= 341,
		LeftAlt			= 342,
		LeftSuper		= 343,
		RightShift		= 344,
		RightControl	= 345,
		RightAlt		= 346,
		RightSuper		= 347,
		Menu			= 348
	};

	enum class MouseButton : std::uint8_t
	{
		Unknown			= static_cast<std::uint8_t>(-1),
		Button1			= 0,
		Button2			= 1,
		Button3			= 2,
		Button4			= 3,
		Button5			= 4,
		Button6			= 5,
		Button7			= 6,
		Button8			= 7,
		Left			= Button1,
		Right			= Button2,
		Middle			= Button3
	};
} // namespace engine