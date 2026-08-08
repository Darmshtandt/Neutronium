// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <Nt/Core/WinMinimal.h>

#undef NOUSER

#include <windows.h>

#include <Nt/Core/Utilities.h>
#include <Nt/Core/Input.h>


namespace Nt {
	void Keyboard::Update() noexcept {
		memcpy(m_IsPressedKeyPrevStateArray, m_IsPressedKeyArray, 256);
		ZeroMemory(m_IsPressedKeyArray, 256);
	}

	Bool Keyboard::IsKeyPressed(const Key& keyCode, const Bool& once) noexcept {
		const Int& keyIntCode = Int(keyCode);

		m_IsPressedKeyArray[keyIntCode] = (GetAsyncKeyState(keyIntCode) & 0x8000);
		if (m_IsPressedKeyArray[keyIntCode]) {
			if (once)
				return (!m_IsPressedKeyPrevStateArray[keyIntCode]);
			return true;
		}
		return false;
	}

	void Mouse::Update() noexcept {
		memcpy(m_IsPressedButtonPrevStateArray, m_IsPressedButtonArray, 6);
		ZeroMemory(m_IsPressedButtonArray, 6);
	}

	Bool Mouse::IsButtonPressed(const Key& buttonCode, const Bool& once) noexcept {
		m_IsPressedButtonArray[buttonCode] = (GetAsyncKeyState(buttonCode) & 0x8000);
		if (m_IsPressedButtonArray[buttonCode]) {
			if (once)
				return (!m_IsPressedButtonPrevStateArray[buttonCode]);
			return true;
		}
		return false;
	}

	NT_FORCE_INLINE Bool Mouse::MoveCursorToCenter() noexcept {
		return SetCursorPos(
			GetSystemMetrics(SM_CXSCREEN) / 2, 
			GetSystemMetrics(SM_CYSCREEN) / 2);
	}

	NT_FORCE_INLINE Int2D Mouse::GetCursorPosition() noexcept {
		POINT cursorPosition;
		GetCursorPos(&cursorPosition);

		return cursorPosition;
	}

	NT_FORCE_INLINE void Mouse::SetCursorPosition(const Int2D& Position) noexcept {
		SetCursorPos(Position.x, Position.y);
	}
}