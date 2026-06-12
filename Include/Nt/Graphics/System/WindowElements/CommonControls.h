#pragma once

#ifdef _WINDEF_
#include <Nt/Core/Utilities.h>

namespace Nt {
	NT_API void InitializeCommonControls();
	NT_API void LoadMSFTEditDLL();

	[[deprecated]] NT_API LRESULT CALLBACK SubClassProc_KillFocus(HWND hwnd, uInt uMsg, WPARAM wParam, LPARAM lParam, [[maybe_unused]] UINT_PTR uIdSubclass, [[maybe_unused]] DWORD_PTR dwRefData);;
	[[deprecated]] NT_API LRESULT CALLBACK SubClassProc_ListView(HWND hwnd, uInt uMsg, WPARAM wParam, LPARAM lParam, [[maybe_unused]] UINT_PTR uIdSubclass, [[maybe_unused]] DWORD_PTR dwRefData);
	[[deprecated]] NT_API LRESULT CALLBACK SubClassProc_ListBox(HWND hwnd, uInt uMsg, WPARAM wParam, LPARAM lParam, [[maybe_unused]] UINT_PTR uIdSubclass, [[maybe_unused]] DWORD_PTR dwRefData);;
}
#endif