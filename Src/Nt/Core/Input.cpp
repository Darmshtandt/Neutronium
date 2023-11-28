#include <windows.h>

#include <Nt/Core/Defines.h>
#include <Nt/Core/NtTypes.h>
#include <Nt/Core/String.h>
#include <Nt/Core/Utilities.h>
#include <Nt/Core/Math/Vector2D.h>
#include <Nt/Core/Input.h>

namespace Nt {
	//Bool Keyboard::IsKeyPressed(const Key& Key, const Bool& Once) noexcept {
	//	Bool Result = false;
	//	const Byte KeyCode = Byte(Key);
	//	Byte Keyboard[256];
	//	if (GetKeyboardState(Keyboard) && (Keyboard[KeyCode] & 0x80)) {
	//		Result = true;
	//		if (Once && (m_PressStampKeys[KeyCode] != m_PressStamp))
	//			Result = (m_PressStampKeys[KeyCode] != m_PressStamp - 1);
	//		m_PressStampKeys[KeyCode] = m_PressStamp;
	//	}
	//	return Result;
	//}
	Bool Keyboard::IsKeyPressed(const Key& Key, const Bool& Once) noexcept {
		Bool Result = false;
		const Byte KeyCode = Byte(Key);
		if (GetAsyncKeyState(KeyCode) & 0x8000) {
			Result = true;
			if (Once && (m_PressStampKeys[KeyCode] != m_PressStamp))
				Result = (m_PressStampKeys[KeyCode] != m_PressStamp - 1);
			m_PressStampKeys[KeyCode] = m_PressStamp;
		}
		return Result;
	}

	Bool Mouse::IsButtonPressed(const Mouse::Button& Button, const Bool& Once) noexcept {
		Bool Result = false;
		const Byte ButtonCode = Byte(Button);
		if (GetAsyncKeyState(ButtonCode) & 0x8000) {
			Result = true;
			if (Once && (m_PressStampButtons[ButtonCode] != m_PressStamp))
				Result = (m_PressStampButtons[ButtonCode] != m_PressStamp - 1);
			m_PressStampButtons[ButtonCode] = m_PressStamp;
		}
		return Result;
	}

	Int2D Mouse::GetCursorPosition() noexcept {
		POINT CursorPosition;
		GetCursorPos(&CursorPosition);
		return CursorPosition;
	}

	void Mouse::SetCursorPosition(const Int2D& Position) noexcept {
		SetCursorPos(Position.x, Position.y);
	}
}