#include <windows.h>

#include <Nt/Core/Defines.h>
#include <Nt/Core/NtTypes.h>
#include <Nt/Core/String.h>
#include <Nt/Core/Utilities.h>
#include <Nt/Core/Math/Vector2D.h>
#include <Nt/Core/Input.h>

namespace Nt {
	Bool Keyboard::IsKeyPressed(const Key& keyCode, const Bool& once) noexcept {
		m_IsPresedKeyArray[keyCode] = (GetAsyncKeyState(keyCode) & 0x8000);
		if (m_IsPresedKeyArray[keyCode]) {
			if (once)
				return (!m_IsPresedKeyPrevStateArray[keyCode]);
			return true;
		}
		return false;
	}

	Bool Mouse::IsButtonPressed(const Mouse::Button& buttonCode, const Bool& once) noexcept {
		m_IsPresedButtonArray[buttonCode] = (GetAsyncKeyState(buttonCode) & 0x8000);
		if (m_IsPresedButtonArray[buttonCode]) {
			if (once)
				return (!m_IsPresedButtonPrevStateArray[buttonCode]);
			return true;
		}
		return false;
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