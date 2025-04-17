// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <Nt/Core/WinMinimal.h>

#undef NOUSER

#include <windows.h>

#include <Nt/Core/Utilities.h>
#include <Nt/Core/Input.h>


namespace Nt {
	Bool Keyboard::IsKeyPressed(const Key& keyCode, const Bool& once) noexcept {
		const Int& keyIntCode = Int(keyCode);

		m_IsPresedKeyArray[keyIntCode] = (GetAsyncKeyState(keyIntCode) & 0x8000);
		if (m_IsPresedKeyArray[keyIntCode]) {
			if (once)
				return (!m_IsPresedKeyPrevStateArray[keyIntCode]);
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
		POINT cursorPosition;
		GetCursorPos(&cursorPosition);

		return cursorPosition;
	}

	void Mouse::SetCursorPosition(const Int2D& Position) noexcept {
		SetCursorPos(Position.x, Position.y);
	}
}