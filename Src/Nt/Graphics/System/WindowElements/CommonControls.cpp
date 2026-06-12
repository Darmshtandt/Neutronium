#include <Nt/Core/WinMinimal.h>

#undef NOUSER
#undef NOWINOFFSETS
#undef NOMSG

#include <windows.h>
#include <windowsx.h>
#include <CommCtrl.h>

#include <Nt/Graphics/System/WindowElements/CommonControls.h>

namespace Nt {
	void InitializeCommonControls() {
		static Bool once = true;
		if (once) {
			INITCOMMONCONTROLSEX initCommonControl;
			initCommonControl.dwSize = sizeof(INITCOMMONCONTROLSEX);
			initCommonControl.dwICC = ICC_WIN95_CLASSES;

			InitCommonControlsEx(&initCommonControl);

			once = false;
		}
	}

	void LoadMSFTEditDLL() {
		static Bool once = true;
		if (once) {
			LoadLibrary(L"Msftedit.dll");
			once = false;
		}
	}

	LRESULT CALLBACK SubClassProc_KillFocus(HWND hwnd, uInt uMsg, WPARAM wParam, LPARAM lParam, [[maybe_unused]] UINT_PTR uIdSubclass, [[maybe_unused]] DWORD_PTR dwRefData) {
		if (uMsg == WM_CHAR) {
			if (wParam == VK_ESCAPE) {
				SendMessage(hwnd, WM_KILLFOCUS, 0, 0);
				return NULL;
			}
			else if (wParam == VK_TAB) {
				return NULL;
			}
		}
		return DefSubclassProc(hwnd, uMsg, wParam, lParam);
	}

	LRESULT CALLBACK SubClassProc_ListView(HWND hwnd, uInt uMsg, WPARAM wParam, LPARAM lParam, [[maybe_unused]] UINT_PTR uIdSubclass, [[maybe_unused]] DWORD_PTR dwRefData) {
		switch (uMsg) {
		case WM_MOUSEMOVE:
		case WM_LBUTTONUP:
			HWND hParent = GetParent(hwnd);
			if (hParent)
				SendMessage(hParent, uMsg, wParam, lParam);
			break;
		}
		return DefSubclassProc(hwnd, uMsg, wParam, lParam);
	}

	LRESULT CALLBACK SubClassProc_ListBox(HWND hwnd, uInt uMsg, WPARAM wParam, LPARAM lParam, [[maybe_unused]] UINT_PTR uIdSubclass, [[maybe_unused]] DWORD_PTR dwRefData) {
		Bool isDeselect = false;
		if (uMsg == WM_KEYDOWN && wParam == VK_ESCAPE) {
			isDeselect = true;
		}
		else if (uMsg == WM_LBUTTONDOWN) {
			isDeselect = true;

			const POINT cursorPosition = {
				GET_X_LPARAM(lParam),
				GET_Y_LPARAM(lParam)
			};
			uInt index = ListBox_GetTopIndex(hwnd);

			RECT itemRect;
			while (ListBox_GetItemRect(hwnd, index, &itemRect) != LB_ERR) {
				if (PtInRect(&itemRect, cursorPosition)) {
					isDeselect = false;
					break;
				}
				++index;
			}
		}

		if (isDeselect) {
			ListBox_SetCurSel(hwnd, -1);
			const uInt id = GetWindowLongPtr(hwnd, GWLP_ID);
			SendMessage(GetParent(hwnd), WM_COMMAND,
				MAKEWPARAM(id, LBN_SELCHANGE), reinterpret_cast<LPARAM>(hwnd));
		}
		return DefSubclassProc(hwnd, uMsg, wParam, lParam);
	}

}