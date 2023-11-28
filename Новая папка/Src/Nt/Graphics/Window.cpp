#include <windows.h>
#include <commdlg.h>
#include <shobjidl.h>

#include <functional>
#include <queue>

#include <Nt/Core/Defines.h>
#include <Nt/Core/NtTypes.h>
#include <Nt/Core/String.h>
#include <Nt/Core/Utilities.h>
#include <Nt/Core/Log.h>

#include <Nt/Core/Math/Vector2D.h>
#include <Nt/Core/Math/Vector3D.h>
#include <Nt/Core/Math/Rect.h>

#include <Nt/Graphics/Menu.h>
#include <Nt/Graphics/HandleWindow.h>
#include <Nt/Graphics/Window.h>

namespace Nt {
	String FileDialog(cwString FilePath, cwString Filter, const Bool& IsOpenFile) noexcept {
		std::filesystem::path initialPath = std::filesystem::current_path();

		std::wstring wPath(FilePath, MAX_PATH);

		ShowCursor(TRUE);
		SetLastError(0);

		OPENFILENAMEW ofn = { };
		ofn.nMaxFile = MAX_PATH;
		ofn.nFilterIndex = 1;
		ofn.lStructSize = sizeof(ofn);
		ofn.lpstrDefExt = L"";
		ofn.lpstrFilter = Filter;
		ofn.lpstrFile = &wPath[0];
		ofn.Flags = OFN_FILEMUSTEXIST | OFN_EXPLORER;

		Bool Result = true;
		if (IsOpenFile) {
			ofn.Flags |= OFN_PATHMUSTEXIST;
			Result = GetOpenFileName(&ofn);
		}
		else {
			ofn.Flags |= OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
			Result = GetSaveFileName(&ofn);
		}
		std::filesystem::current_path(initialPath);

		if (Result)
			return ofn.lpstrFile;
		if (GetLastError() != 0)
			Raise(String("Failed to open file dialog") + String(GetLastError()));
		return L"";
	}
}