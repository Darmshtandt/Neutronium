#include <Windows.h>
#include <cstdlib>

#include <Nt/Core/Defines.h>
#include <Nt/Core/NtTypes.h>
#include <Nt/Core/String.h>
#include <Nt/Core/Utilities.h>
#include <Nt/Core/Math/Vector2D.h>
#include <Nt/Core/Console.h>

namespace Nt {
	const HWND _ConsoleWindow = GetConsoleWindow();

	void Console::Show() noexcept {
		ShowWindow(_ConsoleWindow, SW_SHOW);
	}
	void Console::Hide() noexcept {
		ShowWindow(_ConsoleWindow, SW_HIDE);
	}

	void Console::Pause() noexcept {
		system("pause");
	}
	void Console::Clear() noexcept {
		system("cls");
	}

	Int Console::SetInputCodePage(const CodePageID& ID) noexcept {
		return SetConsoleCP(uInt(ID));
	}
	Int Console::SetOutputCodePage(const CodePageID& ID) noexcept {
		return SetConsoleOutputCP(uInt(ID));
	}
	Int2D Console::SetIOCodePage(const CodePageID& ID) noexcept {
		return Int2D(
			SetConsoleCP(uInt(ID)),
			SetConsoleOutputCP(uInt(ID))
		);
	}
}