#pragma once

#include <Nt/Core/Math/Vector.h>
#include <memory>

#ifdef _CONTROL
#	undef _CONTROL
#endif

#ifdef _SPACE
#	undef _SPACE
#endif

namespace Nt {
	enum class Key : Byte {
		_0 = Byte('0'),
		_1 = Byte('1'),
		_2 = Byte('2'),
		_3 = Byte('3'),
		_4 = Byte('4'),
		_5 = Byte('5'),
		_6 = Byte('6'),
		_7 = Byte('7'),
		_8 = Byte('8'),
		_9 = Byte('9'),

		_A = Byte('A'),
		_B = Byte('B'),
		_C = Byte('C'),
		_D = Byte('D'),
		_E = Byte('E'),
		_F = Byte('F'),
		_G = Byte('G'),
		_H = Byte('H'),
		_I = Byte('I'),
		_J = Byte('J'),
		_K = Byte('K'),
		_L = Byte('L'),
		_M = Byte('M'),
		_N = Byte('N'),
		_O = Byte('O'),
		_P = Byte('P'),
		_Q = Byte('Q'),
		_R = Byte('R'),
		_S = Byte('S'),
		_T = Byte('T'),
		_U = Byte('U'),
		_V = Byte('V'),
		_W = Byte('W'),
		_X = Byte('X'),
		_Y = Byte('Y'),
		_Z = Byte('Z'),

		_CANCEL = 0x03,
		_BACK = 0x08,
		_TAB = 0x09,
		_CLEAR = 0x0C,
		_RETURN = 0x0D,
		_SHIFT = 0x10,
		_CONTROL = 0x11,
		_MENU = 0x12,
		_PAUSE = 0x13,
		_CAPITAL = 0x14,
		_KANA = 0x15,
		_HANGEUL = 0x15,
		_HANGUL = 0x15,
		_IME_ON = 0x16,
		_JUNJA = 0x17,
		_FINAL = 0x18,
		_HANJA = 0x19,
		_KANJI = 0x19,
		_IME_OFF = 0x1A,
		_ESCAPE = 0x1B,
		_CONVERT = 0x1C,
		_NONCONVERT = 0x1D,
		_ACCEPT = 0x1E,
		_MODECHANGE = 0x1F,
		_SPACE = 0x20,
		_PRIOR = 0x21,
		_NEXT = 0x22,
		_END = 0x23,
		_HOME = 0x24,
		_LEFT = 0x25,
		_UP = 0x26,
		_RIGHT = 0x27,
		_DOWN = 0x28,
		_SELECT = 0x29,
		_PRINT = 0x2A,
		_EXEC = 0x2B,
		_SNAPSHOT = 0x2C,
		_INSERT = 0x2D,
		_DEL = 0x2E,
		_HELP = 0x2F,
		_LWIN = 0x5B,
		_RWIN = 0x5C,
		_APPS = 0x5D,
		_SLEEP = 0x5F,
		_NUMPAD0 = 0x60,
		_NUMPAD1 = 0x61,
		_NUMPAD2 = 0x62,
		_NUMPAD3 = 0x63,
		_NUMPAD4 = 0x64,
		_NUMPAD5 = 0x65,
		_NUMPAD6 = 0x66,
		_NUMPAD7 = 0x67,
		_NUMPAD8 = 0x68,
		_NUMPAD9 = 0x69,
		_MULTIPLY = 0x6A,
		_ADD = 0x6B,
		_SEPARATOR = 0x6C,
		_SUBTRACT = 0x6D,
		_DECIMAL = 0x6E,
		_DIVIDE = 0x6F,
		_F1 = 0x70,
		_F2 = 0x71,
		_F3 = 0x72,
		_F4 = 0x73,
		_F5 = 0x74,
		_F6 = 0x75,
		_F7 = 0x76,
		_F8 = 0x77,
		_F9 = 0x78,
		_F10 = 0x79,
		_F11 = 0x7A,
		_F12 = 0x7B,
		_F13 = 0x7C,
		_F14 = 0x7D,
		_F15 = 0x7E,
		_F16 = 0x7F,
		_F17 = 0x80,
		_F18 = 0x81,
		_F19 = 0x82,
		_F20 = 0x83,
		_F21 = 0x84,
		_F22 = 0x85,
		_F23 = 0x86,
		_F24 = 0x87,
		_NAVIGATION_VIEW = 0x88,
		_NAVIGATION_MENU = 0x89,
		_NAVIGATION_UP = 0x8A,
		_NAVIGATION_DOWN = 0x8B,
		_NAVIGATION_LEFT = 0x8C,
		_NAVIGATION_RIGHT = 0x8D,
		_NAVIGATION_ACCEPT = 0x8E,
		_NAVIGATION_CANCEL = 0x8F,
		_NUMLOCK = 0x90,
		_SCROLL = 0x91,
		_OEM_NEC_EQUAL = 0x92,
		_OEM_FJ_JISHO = 0x92,
		_OEM_FJ_MASSHOU = 0x93,
		_OEM_FJ_TOUROKU = 0x94,
		_OEM_FJ_LOYA = 0x95,
		_OEM_FJ_ROYA = 0x96,
		_LSHIFT = 0xA0,
		_RSHIFT = 0xA1,
		_LCONTROL = 0xA2,
		_RCONTROL = 0xA3,
		_LMENU = 0xA4,
		_RMENU = 0xA5,
		_BROWSER_BACK = 0xA6,
		_BROWSER_FORWARD = 0xA7,
		_BROWSER_REFRESH = 0xA8,
		_BROWSER_STOP = 0xA9,
		_BROWSER_SEARCH = 0xAA,
		_BROWSER_FAVORITES = 0xAB,
		_BROWSER_HOME = 0xAC,
		_VOLUME_MUTE = 0xAD,
		_VOLUME_DOWN = 0xAE,
		_VOLUME_UP = 0xAF,
		_MEDIA_NEXT_TRACK = 0xB0,
		_MEDIA_PREV_TRACK = 0xB1,
		_MEDIA_STOP = 0xB2,
		_MEDIA_PLAY_PAUSE = 0xB3,
		_LAUNCH_MAIL = 0xB4,
		_LAUNCH_MEDIA_SELECT = 0xB5,
		_LAUNCH_APP1 = 0xB6,
		_LAUNCH_APP2 = 0xB7,
		_OEM_PLUS = 0xBA,
		_OEM_COMMA = 0xBB,
		_OEM_MINUS = 0xBC,
		_OEM_PERIOD = 0xBD,
		_OEM_1 = 0xBE,
		_OEM_2 = 0xBF,
		_OEM_3 = 0xC0,
		_OEM_4 = 0xDB,
		_OEM_5 = 0xDC,
		_OEM_6 = 0xDD,
		_OEM_7 = 0xDE,
		_OEM_8 = 0xDF,
		_OEM_AX = 0xE1,
		_OEM_102 = 0xE2,
		_ICO_HELP = 0xE3,
		_ICO_00 = 0xE4,
		_PROCESS = 0xE5,
		_ICO_CLEAR = 0xE6,
		_PACKET = 0xE7,
		_OEM_RESET = 0xE9,
		_OEM_JUMP = 0xEA,
		_OEM_PA1 = 0xEB,
		_OEM_PA2 = 0xEC,
		_OEM_PA3 = 0xED,
		_OEM_WSCTRL = 0xEE,
		_OEM_CUSEL = 0xEF,
		_OEM_ATTN = 0xF0,
		_OEM_FINISH = 0xF1,
		_OEM_COPY = 0xF2,
		_OEM_AUTO = 0xF3,
		_OEM_ENLW = 0xF4,
		_OEM_BACKTAB = 0xF5,
		_ATTN = 0xF6,
		_CRSEL = 0xF7,
		_EXSEL = 0xF8,
		_EREOF = 0xF9,
		_PLAY = 0xFA,
		_ZOOM = 0xFB,
		_NONAME = 0xFC,
		_PA1 = 0xFD,
		_OEM_CLEAR = 0xFE,
	};

	class Keyboard {
	public:
		Keyboard() = default;

		void Update() noexcept {
			memcpy(m_IsPresedKeyPrevStateArray, m_IsPresedKeyArray, 256);
			ZeroMemory(m_IsPresedKeyArray, 256);
		}

		NT_API Bool IsKeyPressed(const Key& key, const Bool& once) noexcept;

	private:
		Bool m_IsPresedKeyArray[256] = { };
		Bool m_IsPresedKeyPrevStateArray[256] = { };
	};

	class Mouse {
	public:
		enum Button : Byte {
			_LEFT = 0x01,
			_RIGHT = 0x02,
			_MIDDLE = 0x04,
			_X1 = 0x05,
			_X2 = 0x06,
		};

	public:
		Mouse() = default;

		void Update() noexcept {
			memcpy(m_IsPresedButtonPrevStateArray, m_IsPresedButtonArray, 6);
			ZeroMemory(m_IsPresedButtonArray, 6);
		}

		NT_API Bool IsButtonPressed(const Mouse::Button& button, const Bool& once) noexcept;

		NT_API static Int2D GetCursorPosition() noexcept;
		NT_API static void SetCursorPosition(const Int2D& position) noexcept;

	private:
		Bool m_IsPresedButtonArray[6] = { };
		Bool m_IsPresedButtonPrevStateArray[6] = { };
	};

#ifdef __NT__EXPEREMENTAL
	class Gamepad {
	public:
		enum Button : Byte {
			BUTTON_A = 0xC3,
			BUTTON_B = 0xC4,
			BUTTON_X = 0xC5,
			BUTTON_Y = 0xC6,
			BUTTON_RIGHT_SHOULDER = 0xC7,
			BUTTON_LEFT_SHOULDER = 0xC8,
			BUTTON_LEFT_TRIGGER = 0xC9,
			BUTTON_RIGHT_TRIGGER = 0xCA,
			BUTTON_DPAD_UP = 0xCB,
			BUTTON_DPAD_DOWN = 0xCC,
			BUTTON_DPAD_LEFT = 0xCD,
			BUTTON_DPAD_RIGHT = 0xCE,
			BUTTON_MENU = 0xCF,
			BUTTON_VIEW = 0xD0,
			BUTTON_LEFT_THUMBSTICKEY = 0xD1,
			BUTTON_RIGHT_THUMBSTICKEY = 0xD2,
			BUTTON_LEFT_THUMBSTIC_UP = 0xD3,
			BUTTON_LEFT_THUMBSTIC_DOWN = 0xD4,
			BUTTON_LEFT_THUMBSTIC_RIGHT = 0xD5,
			BUTTON_LEFT_THUMBSTIC_LEFT = 0xD6,
			BUTTON_RIGHT_THUMBSTIC_UP = 0xD7,
			BUTTON_RIGHT_THUMBSTIC_DOWN = 0xD8,
			BUTTON_RIGHT_THUMBSTIC_RIGHT = 0xD9,
			BUTTON_RIGHT_THUMBSTIC_LEFT = 0xDA,
		};

	public:
		Gamepad() = default;

	private:
		uInt m_PressStamp = 0;
	};
#endif
}