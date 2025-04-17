// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <Nt/Core/Utilities.h>
#include <Nt/Core/Console.h>
#include <Nt/Core/WinMinimal.h>

#undef NOUSER

#include <Windows.h>
#include <cstdlib>

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