// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <Nt/Core/Utilities.h>
#include <Nt/Core/WinMinimal.h>

#undef NOUSER

#include <windows.h>

#pragma comment(lib, "OpenGL32")


namespace Nt {
	void _ShowCursor(const Bool& fShow) noexcept {
		SetCursor(fShow ? LoadCursor(nullptr, IDC_ARROW) : nullptr);
	}
}