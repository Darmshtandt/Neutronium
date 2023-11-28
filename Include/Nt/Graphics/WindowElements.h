#pragma once

namespace Nt {
	__inline void InitializeCommonControls() {
		static Bool once = true;
		if (once) {
			InitCommonControls();
			once = false;
		}
	}
	__inline void LoadMSFTEditDLL() {
		static Bool once = true;
		if (once) {
			LoadLibrary(L"Msftedit.dll");
			once = false;
		}
	}

	__inline LRESULT CALLBACK SubClassProc_KillFocus(HWND hwnd, uInt uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData) {
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
	};
	__inline LRESULT CALLBACK SubClassProc_TreeView(HWND hwnd, uInt uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData) {
		if (uMsg == WM_CHAR && wParam == VK_ESCAPE)
			TreeView_SelectItem(hwnd, nullptr);
		if (uMsg == WM_CHAR || uMsg == WM_KEYDOWN || uMsg == WM_KEYUP) {
			const HWND hParent = GetParent(hwnd);
			if (hParent != nullptr)
				SendMessage(hParent, uMsg, wParam, lParam);
		}
		return SubClassProc_KillFocus(hwnd, uMsg, wParam, lParam, uIdSubclass, dwRefData);
	};
	__inline LRESULT CALLBACK SubClassProc_ListView(HWND hwnd, uInt uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData) {
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
	__inline LRESULT CALLBACK SubClassProc_ListBox(HWND hwnd, uInt uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData) {
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
	};
}

#include <Nt/Graphics/WindowElements/Tooltip.h>
#include <Nt/Graphics/WindowElements/ImageList.h>
#include <Nt/Graphics/WindowElements/Text.h>
#include <Nt/Graphics/WindowElements/TextEdit.h>
#include <Nt/Graphics/WindowElements/Button.h>
#include <Nt/Graphics/WindowElements/ComboBox.h>
#include <Nt/Graphics/WindowElements/TreeView.h>
#include <Nt/Graphics/WindowElements/ListView.h>
#include <Nt/Graphics/WindowElements/ListBox.h>