#pragma once

namespace Nt {
	class Keyboard {
	public:
		enum Key : Byte {
			KEY_0 = '0',
			KEY_1 = '1',
			KEY_2 = '2',
			KEY_3 = '3',
			KEY_4 = '4',
			KEY_5 = '5',
			KEY_6 = '6',
			KEY_7 = '7',
			KEY_8 = '8',
			KEY_9 = '9',

			KEY_A = 'A',
			KEY_B = 'B',
			KEY_C = 'C',
			KEY_D = 'D',
			KEY_E = 'E',
			KEY_F = 'F',
			KEY_G = 'G',
			KEY_H = 'H',
			KEY_I = 'I',
			KEY_J = 'J',
			KEY_K = 'K',
			KEY_L = 'L',
			KEY_M = 'M',
			KEY_N = 'N',
			KEY_O = 'O',
			KEY_P = 'P',
			KEY_Q = 'Q',
			KEY_R = 'R',
			KEY_S = 'S',
			KEY_T = 'T',
			KEY_U = 'U',
			KEY_V = 'V',
			KEY_W = 'W',
			KEY_X = 'X',
			KEY_Y = 'Y',
			KEY_Z = 'Z',

			KEY_CANCEL = 0x03,
			KEY_BACK = 0x08,
			KEY_TAB = 0x09,
			KEY_CLEAR = 0x0C,
			KEY_RETURN = 0x0D,
			KEY_SHIFT = 0x10,
			KEY_CONTROL = 0x11,
			KEY_MENU = 0x12,
			KEY_PAUSE = 0x13,
			KEY_CAPITAL = 0x14,
			KEY_KANA = 0x15,
			KEY_HANGEUL = 0x15,
			KEY_HANGUL = 0x15,
			KEY_IME_ON = 0x16,
			KEY_JUNJA = 0x17,
			KEY_FINAL = 0x18,
			KEY_HANJA = 0x19,
			KEY_KANJI = 0x19,
			KEY_IME_OFF = 0x1A,
			KEY_ESCAPE = 0x1B,
			KEY_CONVERT = 0x1C,
			KEY_NONCONVERT = 0x1D,
			KEY_ACCEPT = 0x1E,
			KEY_MODECHANGE = 0x1F,
			KEY_SPACE = 0x20,
			KEY_PRIOR = 0x21,
			KEY_NEXT = 0x22,
			KEY_END = 0x23,
			KEY_HOME = 0x24,
			KEY_LEFT = 0x25,
			KEY_UP = 0x26,
			KEY_RIGHT = 0x27,
			KEY_DOWN = 0x28,
			KEY_SELECT = 0x29,
			KEY_PRINT = 0x2A,
			KEY_EXEC = 0x2B,
			KEY_SNAPSHOT = 0x2C,
			KEY_INSERT = 0x2D,
			KEY_DEL = 0x2E,
			KEY_HELP = 0x2F,
			KEY_LWIN = 0x5B,
			KEY_RWIN = 0x5C,
			KEY_APPS = 0x5D,
			KEY_SLEEP = 0x5F,
			KEY_NUMPAD0 = 0x60,
			KEY_NUMPAD1 = 0x61,
			KEY_NUMPAD2 = 0x62,
			KEY_NUMPAD3 = 0x63,
			KEY_NUMPAD4 = 0x64,
			KEY_NUMPAD5 = 0x65,
			KEY_NUMPAD6 = 0x66,
			KEY_NUMPAD7 = 0x67,
			KEY_NUMPAD8 = 0x68,
			KEY_NUMPAD9 = 0x69,
			KEY_MULTIPLY = 0x6A,
			KEY_ADD = 0x6B,
			KEY_SEPARATOR = 0x6C,
			KEY_SUBTRACT = 0x6D,
			KEY_DECIMAL = 0x6E,
			KEY_DIVIDE = 0x6F,
			KEY_F1 = 0x70,
			KEY_F2 = 0x71,
			KEY_F3 = 0x72,
			KEY_F4 = 0x73,
			KEY_F5 = 0x74,
			KEY_F6 = 0x75,
			KEY_F7 = 0x76,
			KEY_F8 = 0x77,
			KEY_F9 = 0x78,
			KEY_F10 = 0x79,
			KEY_F11 = 0x7A,
			KEY_F12 = 0x7B,
			KEY_F13 = 0x7C,
			KEY_F14 = 0x7D,
			KEY_F15 = 0x7E,
			KEY_F16 = 0x7F,
			KEY_F17 = 0x80,
			KEY_F18 = 0x81,
			KEY_F19 = 0x82,
			KEY_F20 = 0x83,
			KEY_F21 = 0x84,
			KEY_F22 = 0x85,
			KEY_F23 = 0x86,
			KEY_F24 = 0x87,
			KEY_NAVIGATION_VIEW = 0x88,
			KEY_NAVIGATION_MENU = 0x89,
			KEY_NAVIGATION_UP = 0x8A,
			KEY_NAVIGATION_DOWN = 0x8B,
			KEY_NAVIGATION_LEFT = 0x8C,
			KEY_NAVIGATION_RIGHT = 0x8D,
			KEY_NAVIGATION_ACCEPT = 0x8E,
			KEY_NAVIGATION_CANCEL = 0x8F,
			KEY_NUMLOCK = 0x90,
			KEY_SCROLL = 0x91,
			KEY_OEM_NEC_EQUAL = 0x92,
			KEY_OEM_FJ_JISHO = 0x92,
			KEY_OEM_FJ_MASSHOU = 0x93,
			KEY_OEM_FJ_TOUROKU = 0x94,
			KEY_OEM_FJ_LOYA = 0x95,
			KEY_OEM_FJ_ROYA = 0x96,
			KEY_LSHIFT = 0xA0,
			KEY_RSHIFT = 0xA1,
			KEY_LCONTROL = 0xA2,
			KEY_RCONTROL = 0xA3,
			KEY_LMENU = 0xA4,
			KEY_RMENU = 0xA5,
			KEY_BROWSER_BACK = 0xA6,
			KEY_BROWSER_FORWARD = 0xA7,
			KEY_BROWSER_REFRESH = 0xA8,
			KEY_BROWSER_STOP = 0xA9,
			KEY_BROWSER_SEARCH = 0xAA,
			KEY_BROWSER_FAVORITES = 0xAB,
			KEY_BROWSER_HOME = 0xAC,
			KEY_VOLUME_MUTE = 0xAD,
			KEY_VOLUME_DOWN = 0xAE,
			KEY_VOLUME_UP = 0xAF,
			KEY_MEDIA_NEXT_TRACK = 0xB0,
			KEY_MEDIA_PREV_TRACK = 0xB1,
			KEY_MEDIA_STOP = 0xB2,
			KEY_MEDIA_PLAY_PAUSE = 0xB3,
			KEY_LAUNCH_MAIL = 0xB4,
			KEY_LAUNCH_MEDIA_SELECT = 0xB5,
			KEY_LAUNCH_APP1 = 0xB6,
			KEY_LAUNCH_APP2 = 0xB7,
			KEY_OEM_PLUS = 0xBA,
			KEY_OEM_COMMA = 0xBB,
			KEY_OEM_MINUS = 0xBC,
			KEY_OEM_PERIOD = 0xBD,
			KEY_OEM_1 = 0xBE,
			KEY_OEM_2 = 0xBF,
			KEY_OEM_3 = 0xC0,
			KEY_OEM_4 = 0xDB,
			KEY_OEM_5 = 0xDC,
			KEY_OEM_6 = 0xDD,
			KEY_OEM_7 = 0xDE,
			KEY_OEM_8 = 0xDF,
			KEY_OEM_AX = 0xE1,
			KEY_OEM_102 = 0xE2,
			KEY_ICO_HELP = 0xE3,
			KEY_ICO_00 = 0xE4,
			KEY_PROCESS = 0xE5,
			KEY_ICO_CLEAR = 0xE6,
			KEY_PACKET = 0xE7,
			KEY_OEM_RESET = 0xE9,
			KEY_OEM_JUMP = 0xEA,
			KEY_OEM_PA1 = 0xEB,
			KEY_OEM_PA2 = 0xEC,
			KEY_OEM_PA3 = 0xED,
			KEY_OEM_WSCTRL = 0xEE,
			KEY_OEM_CUSEL = 0xEF,
			KEY_OEM_ATTN = 0xF0,
			KEY_OEM_FINISH = 0xF1,
			KEY_OEM_COPY = 0xF2,
			KEY_OEM_AUTO = 0xF3,
			KEY_OEM_ENLW = 0xF4,
			KEY_OEM_BACKTAB = 0xF5,
			KEY_ATTN = 0xF6,
			KEY_CRSEL = 0xF7,
			KEY_EXSEL = 0xF8,
			KEY_EREOF = 0xF9,
			KEY_PLAY = 0xFA,
			KEY_ZOOM = 0xFB,
			KEY_NONAME = 0xFC,
			KEY_PA1 = 0xFD,
			KEY_OEM_CLEAR = 0xFE,
		};
		
	public:
		Keyboard() = default;

		void Update() noexcept {
			++m_PressStamp;
		}

		NT_API Bool IsKeyPressed(const Key& Key, const Bool& Once = false) noexcept;

	private:
		uInt m_PressStampKeys[256] = { };
		uInt m_PressStamp = 0;
	};

	class Mouse {
	public:
		enum Button : Byte {
			BUTTON_LEFT = 0x01,
			BUTTON_RIGHT = 0x02,
			BUTTON_MIDDLE = 0x04,
			BUTTON_X1 = 0x05,
			BUTTON_X2 = 0x06,
		};

	public:
		Mouse() = default;

		void Update() noexcept {
			++m_PressStamp;
		}

		NT_API Bool IsButtonPressed(const Mouse::Button& Button, const Bool& Once = false) noexcept;

		NT_API static Int2D GetCursorPosition() noexcept;
		NT_API static void SetCursorPosition(const Int2D& Position) noexcept;

	private:
		Int2D m_PrevCursorPosition;
		uInt m_PressStampButtons[6] = { };
		uInt m_PressStamp = 0;
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
			BUTTON_LEFT_THUMBSTICKEY_UP = 0xD3,
			BUTTON_LEFT_THUMBSTICKEY_DOWN = 0xD4,
			BUTTON_LEFT_THUMBSTICKEY_RIGHT = 0xD5,
			BUTTON_LEFT_THUMBSTICKEY_LEFT = 0xD6,
			BUTTON_RIGHT_THUMBSTICKEY_UP = 0xD7,
			BUTTON_RIGHT_THUMBSTICKEY_DOWN = 0xD8,
			BUTTON_RIGHT_THUMBSTICKEY_RIGHT = 0xD9,
			BUTTON_RIGHT_THUMBSTICKEY_LEFT = 0xDA,
		};

	public:
		Gamepad() = default;

	private:
		uInt m_PressStamp = 0;
	};
#endif
}