#include <windows.h>
#include <shobjidl.h>
#include <shellapi.h>
#include <commoncontrols.h>

#include <Nt/Core/Defines.h>
#include <Nt/Core/NtTypes.h>
#include <Nt/Core/String.h>
#include <Nt/Core/Utilities.h>
#include <Nt/Core/Log.h>

#include <Nt/Core/Math/Vectors.h>
#include <Nt/Core/Math/Rect.h>

#include <Nt/Graphics/Menu.h>
#include <Nt/Graphics/HandleWindow.h>

namespace Nt {
	Int2D GetCursorPosition() noexcept {
		Int2D CursorPosition;
		GetCursorPos((POINT*)&CursorPosition);
		return CursorPosition;
	}
	Int2D GetMonitorSize() noexcept {
		return {
			GetSystemMetrics(SM_CXSCREEN),
			GetSystemMetrics(SM_CYSCREEN)
		};
	}

	IntRect GetClientRect(HWND hwnd) noexcept {
		RECT rect = { };
		if (hwnd == nullptr)
			Log::Warning("Handle window pointer is null.");
		else if (!GetClientRect(hwnd, &rect))
			Log::Warning("Failed to get window client rect.");
		return rect;
	}
	IntRect GetWindowRect(HWND hwnd) noexcept {
		RECT rect = { };
		if (hwnd == nullptr)
			Log::Warning("Handle window pointer is null.");
		else if (!GetWindowRect(hwnd, &rect))
			Log::Warning("Failed to get window rect.");
		return rect;
	}
	IntRect AdjustWindowRect(const IntRect& windowRect, const DWord& styles, const Bool& isHasMenu) noexcept {
		RECT rect = windowRect;
		if (!AdjustWindowRect(&rect, styles, isHasMenu))
			Log::Warning("Failed to get window rect.");
		return rect;
	}
	IntRect AdjustWindowRectEx(const IntRect& windowRect, const DWord& styles, const DWord& exStyles, const Bool& isHasMenu) noexcept {
		RECT rect = windowRect;
		if (!AdjustWindowRectEx(&rect, styles, isHasMenu, exStyles))
			Log::Warning("Failed to get window rect.");
		return rect;
	}

	COLORREF VectorToColorRef(const Byte3D& color) {
		return RGB(color.r, color.g, color.b);
	}
	Byte3D ColorRefToVector(const COLORREF& color) {
		return Byte3D(GetRValue(color), GetGValue(color), GetBValue(color));
	}

	HBRUSH CreateSolidBrush(const Byte3D& color) noexcept {
		return ::CreateSolidBrush(VectorToColorRef(color));
	}


	HandleWindow::HandleWindow() noexcept :
		m_Styles(0),
		m_ExStyles(0),
		m_hwnd(nullptr),
		m_hParent(nullptr),
		m_hdc(nullptr),
		m_ID(0),
		m_ZOrder(ZORDER_TOP),
		m_hInstance(GetModuleHandle(nullptr)),
		m_pParam(nullptr),
		m_BackgroundColor(255, 255, 255),
		m_IsMenuEnabled(false),
		m_IsWindowEnabled(true)
	{ 
	}

	void HandleWindow::Create(const IntRect& windowRect, const String& name) {
		m_WindowRect = windowRect;
		m_Name = name;
		_CreateWindow();
	}
	HandleWindow& HandleWindow::CreateCopy() const {
		HandleWindow window;
		window.m_hParent = m_hParent;
		window.m_Menu = m_Menu;
		window.m_ID = m_ID;
		window.m_hInstance = m_hInstance;
		window.m_pParam = m_pParam;
		window.m_BackgroundColor = m_BackgroundColor;
		window.m_Name = m_Name;
		window.m_ClassName = m_ClassName;
		window.m_ClientRect = m_ClientRect;
		window.m_Styles = m_Styles;
		window.m_ExStyles = m_ExStyles;
		window.m_IsMenuEnabled = m_IsMenuEnabled;
		window.m_IsWindowEnabled = m_IsWindowEnabled;
		window._CreateWindow();
		return window;
	}

	void HandleWindow::Hide() {
		if (!IsCreated())
			Raise("Handle window is not created");

		m_Styles &= ~WS_VISIBLE;
		ShowWindow(m_hwnd, SW_HIDE);
	}
	void HandleWindow::Show() {
		if (!IsCreated())
			Raise("Handle window is not created");

		m_Styles |= WS_VISIBLE;
		ShowWindow(m_hwnd, SW_SHOW);
	}
	void HandleWindow::ShowMaximized() {
		if (!IsCreated())
			Raise("Handle window is not created");

		m_Styles |= WS_VISIBLE;
		ShowWindow(m_hwnd, SW_SHOWMAXIMIZED);
	}

	Bool HandleWindow::EnableWindow() noexcept {
		m_IsWindowEnabled = true;
		if (m_hwnd)
			return ::EnableWindow(m_hwnd, TRUE);
		return true;
	}
	Bool HandleWindow::DisableWindow() noexcept {
		m_IsWindowEnabled = false;
		if (m_hwnd)
			return ::EnableWindow(m_hwnd, FALSE);
		return true;
	}

	Int2D HandleWindow::ScreenToClient(const Int2D& cursorPosition) const noexcept {
		POINT point = cursorPosition;
		::ScreenToClient(m_hwnd, &point);
		return point;
	}

	void HandleWindow::AddStyles(const DWord& styles) noexcept {
		m_Styles |= styles;
		if (m_hwnd)
			SetWindowLongPtr(m_hwnd, GWL_STYLE, m_Styles);
	}
	void HandleWindow::RemoveStyles(const DWord& styles) noexcept {
		m_Styles &= ~styles;
		if (m_hwnd)
			SetWindowLongPtr(m_hwnd, GWL_STYLE, m_Styles);
	}
	void HandleWindow::AddExStyles(const DWord& styles) noexcept {
		m_ExStyles |= styles;
		if (m_hwnd)
			SetWindowLongPtr(m_hwnd, GWL_EXSTYLE, m_ExStyles);
	}
	void HandleWindow::RemoveExStyles(const DWord& styles) noexcept {
		m_ExStyles &= ~styles;
		if (m_hwnd)
			SetWindowLongPtr(m_hwnd, GWL_EXSTYLE, m_ExStyles);
	}

	void HandleWindow::EnableMenu() noexcept {
		m_IsMenuEnabled = true;
		if (m_hwnd)
			::SetMenu(m_hwnd, m_Menu.GetHandle());
	}
	void HandleWindow::DisableMenu() noexcept {
		m_IsMenuEnabled = false;
		if (m_hwnd)
			::SetMenu(m_hwnd, nullptr);
	}

	void HandleWindow::Destroy() noexcept {
		if (m_hwnd) {
			DestroyWindow(m_hwnd);
			m_hwnd = nullptr;
			m_hParent = nullptr;
		}
	}

	void HandleWindow::SetIcon(const String& iconPath) {
		const HICON hIcon = (HICON)LoadImageA(nullptr, iconPath.c_str(), IMAGE_ICON,
			0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE | LR_SHARED);
		if (m_hwnd)
			SetClassLongPtr(m_hwnd, GCLP_HICON, reinterpret_cast<Long>(hIcon));
	}
	void HandleWindow::SetIconSmall(const String& iconPath) {
		HICON hIcon = (HICON)LoadImageA(nullptr, iconPath.c_str(), IMAGE_ICON,
			0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE | LR_SHARED);
		if (m_hwnd)
			SetClassLongPtr(m_hwnd, GCLP_HICONSM, reinterpret_cast<Long>(hIcon));
	}

#ifdef _WINDEF_
	void HandleWindow::SetInstance(const HINSTANCE& hInstance) noexcept {
		if (!IsCreated())
			m_hInstance = hInstance;
		else
			Log::Warning("hInstance cannot be changed because the window has already been created");
	}
	void HandleWindow::SetParentHandle(const HWND& hParent) {
		if (m_hwnd)
			::SetParent(m_hwnd, hParent);

		if (hParent == nullptr)
			RemoveStyles(WS_CHILD);
		else if (m_hParent == nullptr)
			AddStyles(WS_CHILD);

		m_hParent = hParent;
	}
#endif
	void HandleWindow::SetMenu(const Menu& menu) noexcept {
		m_Menu = menu;
		if (m_hwnd && m_IsMenuEnabled)
			::SetMenu(m_hwnd, m_Menu.GetHandle());
	}
	void HandleWindow::SetID(const Int& newID) noexcept {
		m_ID = newID;
		if (m_hwnd)
			SetWindowLongPtr(m_hwnd, m_ID, GWLP_ID);
	}
	void HandleWindow::SetParent(const HandleWindow& parentWindow) {
		SetParentHandle(parentWindow.GetHandle());
	}
	Bool HandleWindow::SetWindowPos(const IntRect& rect, const WindowPositionFlags& flags) noexcept {
		const Bool result = ::SetWindowPos(m_hwnd, HWND(m_ZOrder), rect.Left, rect.Top, rect.Right, rect.Bottom, flags);
		if (m_hwnd != nullptr && !(flags & POSITIONFLAG_NOMOVE || flags & POSITIONFLAG_NOSIZE)) {
			m_WindowRect = rect;

			m_ClientRect = Nt::AdjustWindowRectEx(m_WindowRect, m_Styles, m_ExStyles, m_IsMenuEnabled);
			m_ClientRect.RightBottom -= abs(m_ClientRect.LeftTop);
			m_ClientRect.LeftTop = Int2D();

			if (m_Styles & WS_BORDER)
				m_ClientRect.RightBottom -= Int2D(GetSystemMetrics(SM_CXDLGFRAME), GetSystemMetrics(SM_CYDLGFRAME));
		}

		if (flags & POSITIONFLAG_SHOWWINDOW)
			m_Styles |= WS_VISIBLE;
		else if (flags & POSITIONFLAG_HIDEWINDOW)
			m_Styles &= ~WS_VISIBLE;
		return result;
	}
	Bool HandleWindow::SetWindowPos(const ZOrder& order, const IntRect& rect, const WindowPositionFlags& flags) noexcept {
		if (flags & POSITIONFLAG_NOOWNERZORDER || flags & POSITIONFLAG_NOZORDER)
			m_ZOrder = order;
		return SetWindowPos(rect, flags);
	}
	void HandleWindow::SetPosition(const Int2D& newPosition) noexcept {
		if (m_WindowRect.LeftTop != newPosition) {
			m_WindowRect.LeftTop = newPosition;
			SetWindowPos(m_ZOrder, m_WindowRect, POSITIONFLAG_NOSIZE);
		}
	}
	void HandleWindow::SetSize(const Int2D& newSize) noexcept {
		if (m_WindowRect.RightBottom != newSize) {
			m_WindowRect.RightBottom = newSize;
			SetWindowPos(m_ZOrder, m_WindowRect, POSITIONFLAG_NOMOVE);
		}
	}
	void HandleWindow::SetWindowRect(const IntRect& newRect) noexcept {
		if (m_WindowRect != newRect) {
			m_WindowRect = newRect;
			SetWindowPos(m_WindowRect, POSITIONFLAG_NONE);
		}
	}
	void HandleWindow::SetName(const String& name) {
		m_Name = name;
		if (m_hwnd)
			SetWindowText(m_hwnd, m_Name.c_str());
	}

#ifdef _WINDEF_
	HWND HandleWindow::GetParentHandle() const noexcept {
		return m_hParent;
	}
	HWND HandleWindow::GetHandle() const noexcept {
		return m_hwnd;
	}
	HDC HandleWindow::GetDC() const noexcept {
		return m_hdc;
	}
	HINSTANCE HandleWindow::GetInstance() const noexcept {
		return m_hInstance;
	}
#endif
	Nt::String HandleWindow::GetName() const {
		return m_Name;
	}
	Menu HandleWindow::GetMenu() const noexcept {
		return m_Menu;
	}
	IntRect HandleWindow::GetClientRect() const noexcept {
		return m_ClientRect;
	}
	IntRect HandleWindow::GetWindowRect() const noexcept {
		return m_WindowRect;
	}
	IntRect HandleWindow::GetAdjustedWindowRect() const noexcept {
		return AdjustWindowRectEx(m_WindowRect, m_Styles, m_IsMenuEnabled, m_ExStyles);
	}
	HandleWindow HandleWindow::GetParent() const noexcept {
		return HandleWindow(m_hParent);
	}
	Int HandleWindow::GetID() const noexcept {
		return m_ID;
	}
	void* HandleWindow::GetParamPtr() const noexcept {
		return m_pParam;
	}
	Byte3D HandleWindow::GetBackgroundColor() const noexcept {
		return m_BackgroundColor;
	}
	Bool HandleWindow::IsMenuEnabled() const noexcept {
		return m_IsMenuEnabled;
	}
	Bool HandleWindow::IsShowed() const noexcept {
		return (m_Styles & WS_VISIBLE);
	}
	Bool HandleWindow::IsCreated() const noexcept {
		return (m_hwnd != nullptr);
	}

	void HandleWindow::_CreateWindow() {
		m_hdc = nullptr;
		m_hwnd = CreateWindowEx(
			m_ExStyles,
			m_ClassName.c_str(), m_Name.c_str(),
			m_Styles,
			m_WindowRect.Left, m_WindowRect.Top, m_WindowRect.Right, m_WindowRect.Bottom,
			m_hParent,
			reinterpret_cast<HMENU>(m_ID),
			m_hInstance,
			m_pParam);

		if (!IsCreated()) {
			const String classNameUpperCase = String(m_ClassName).ToUpper();

			String windowType = "handle window";
			if (classNameUpperCase == "EDIT")
				windowType = "TextEdit";
			else if (classNameUpperCase == "BUTTON")
				windowType = "Button";
			else if (classNameUpperCase == "RICHEDIT50W")
				windowType = "RichTextEdit";
			else if (classNameUpperCase == String(WC_TREEVIEW).ToUpper())
				windowType = "TreeVew";


			String errorMsg = "Failed to create ";
			errorMsg += windowType;
			errorMsg += ".\nError code: ";
			errorMsg += GetLastError();
			Raise(errorMsg.c_str());
		}

		m_ClientRect = Nt::AdjustedWindowRectEx(m_hwnd, m_Styles, m_ExStyles, m_IsMenuEnabled);
		m_ClientRect.RightBottom -= abs(m_ClientRect.LeftTop);
		m_ClientRect.LeftTop = Int2D();

		if (m_Styles & WS_BORDER)
			m_ClientRect.RightBottom -= Int2D(GetSystemMetrics(SM_CXDLGFRAME), GetSystemMetrics(SM_CYDLGFRAME)) - 1;

		m_WindowRect = Nt::GetWindowRect(m_hwnd);
		m_WindowRect.RightBottom -= m_WindowRect.LeftTop;

		if (m_IsMenuEnabled)
			::SetMenu(m_hwnd, m_Menu.GetHandle());
		m_hdc = ::GetDC(m_hwnd);

		if (!m_IsWindowEnabled)
			::EnableWindow(m_hwnd, FALSE);
	}
	Long HandleWindow::_SendMessage(const uInt& message, const uInt& wParam, const Long& lParam) const {
		if (!IsCreated())
			Raise("Handle window is not created");
		return SendMessage(m_hwnd, message, wParam, lParam);
	}

	String OpenFileDialog(const String& startPath, const _FILEOPENDIALOGOPTIONS& options) {
		String filePath;

		IFileDialog* pFileDialog = nullptr;
		HRESULT hResult = CoCreateInstance(CLSID_FileOpenDialog, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pFileDialog));
		if (SUCCEEDED(hResult)) {
			DWord dwOptions;
			pFileDialog->GetOptions(&dwOptions);
			pFileDialog->SetOptions(dwOptions | options);

			IShellItem* pItem = nullptr;
			hResult = SHCreateItemFromParsingName(
				startPath.wstr().c_str(), nullptr, IID_PPV_ARGS(&pItem));
			if (SUCCEEDED(hResult)) {
				pFileDialog->SetFolder(pItem);
				pItem->Release();
			}

			hResult = pFileDialog->Show(nullptr);
			if (SUCCEEDED(hResult)) {
				IShellItem* pItem = nullptr;
				hResult = pFileDialog->GetResult(&pItem);
				if (SUCCEEDED(hResult)) {
					wChar* pszFolderPath = nullptr;
					hResult = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFolderPath);
					if (SUCCEEDED(hResult)) {
						filePath = pszFolderPath;
						CoTaskMemFree(pszFolderPath);
					}
					pItem->Release();
				}
			}
			pFileDialog->Release();
		}

		if (FAILED(hResult) && hResult != HRESULT_FROM_WIN32(ERROR_CANCELLED)) {
			String errorMessage = "Failed to open file dialog.\nError code: 0x";
			errorMessage += HRESULT_CODE(hResult);
			Raise(errorMessage);
		}
		return filePath;
	}
}