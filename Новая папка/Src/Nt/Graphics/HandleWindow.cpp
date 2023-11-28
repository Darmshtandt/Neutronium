#include <windows.h>
#include <shobjidl.h>
#include <shellapi.h>
#include <commoncontrols.h>

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

namespace Nt {
	Int2D GetMonitorSize() noexcept {
		return {
			GetSystemMetrics(SM_CXSCREEN),
			GetSystemMetrics(SM_CYSCREEN)
		};
	}
	IntRect GetClientRect(HWND hwnd) noexcept {
		RECT ClientRect;
		if (!GetClientRect(hwnd, &ClientRect))
			Log::Warning("Failed to get window client rect.");
		return ClientRect;
	}
	IntRect GetWindowRect(HWND hwnd) noexcept {
		RECT ClientRect;
		if (!GetWindowRect(hwnd, &ClientRect))
			Log::Warning("Failed to get window rect.");
		return ClientRect;
	}
	Int2D GetCursorPosition() noexcept {
		Int2D CursorPosition;
		GetCursorPos((POINT*)&CursorPosition);
		return CursorPosition;
	}
	HBRUSH CreateSolidBrush(const uInt3D& color) noexcept {
		return ::CreateSolidBrush(RGB(color.x, color.y, color.z));
	}
	COLORREF ToColorRef(const Int3D& color) {
		return RGB(color.r, color.g, color.b);
	}

	HandleWindow::HandleWindow() noexcept :
		m_Styles(0),
		m_ExStyles(0),
		m_hwnd(nullptr),
		m_pParent(nullptr),
		m_hdc(nullptr),
		m_ID(0),
		m_hInstance(GetModuleHandle(nullptr)),
		m_pParam(nullptr),
		m_BackgroundColor(255, 255, 255),
		m_IsMenuEnabled(false),
		m_IsWindowEnabled(true)
	{ 
	}
	HandleWindow::HandleWindow(const HandleWindow& handle) :
		m_hwnd(nullptr),
		m_hdc(nullptr),
		m_pParent(handle.m_pParent),
		m_Menu(handle.m_Menu),
		m_ID(handle.m_ID),
		m_hInstance(handle.m_hInstance),
		m_pParam(handle.m_pParam),
		m_BackgroundColor(handle.m_BackgroundColor),
		m_Name(handle.m_Name),
		m_ClassName(handle.m_ClassName),
		m_Rect(handle.m_Rect),
		m_Styles(handle.m_Styles),
		m_ExStyles(handle.m_ExStyles),
		m_IsMenuEnabled(handle.m_IsMenuEnabled),
		m_IsWindowEnabled(handle.m_IsWindowEnabled)
	{
		_CreateWindow();
	}
	HandleWindow::~HandleWindow() noexcept {
		Destroy();
	}

	void HandleWindow::Resize(const uInt2D& size) noexcept {
		m_Rect.RightBottom = size;
		SetWindowPos(m_hwnd, nullptr, m_Rect.Left, m_Rect.Top, size.x, size.y, 0);
	}

	void HandleWindow::Hide() const noexcept {
		ShowWindow(m_hwnd, SW_HIDE);
	}
	void HandleWindow::Show() const noexcept {
		ShowWindow(m_hwnd, SW_SHOW);
	}
	void HandleWindow::ShowMaximized() const noexcept {
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
			m_pParent = nullptr;
		}
	}

	void HandleWindow::SetBackgroundColor(const uInt3D& color) {
		m_BackgroundColor = color;
		if (m_hwnd) {
			const HBRUSH hColor = CreateSolidBrush(m_BackgroundColor);
			SetClassLongPtr(m_hwnd, GCLP_HBRBACKGROUND, (LONG_PTR)hColor);
			InvalidateRect(m_hwnd, nullptr, TRUE);
		}
	}
	void HandleWindow::SetIcon(const cwString& iconName) {
		const HICON hIcon = LoadIcon(m_hInstance, iconName);
		if (m_hwnd)
			SetClassLongPtr(m_hwnd, GCLP_HICON, reinterpret_cast<Long>(hIcon));
	}
	void HandleWindow::SetIconSmall(const cwString& iconName) {
		HICON hIcon = LoadIcon(m_hInstance, iconName);
		if (m_hwnd)
			SetClassLongPtr(m_hwnd, GCLP_HICONSM, reinterpret_cast<Long>(hIcon));
	}

#ifdef _WINDEF_
	void HandleWindow::SetInstance(const HINSTANCE& hInstance) noexcept {
		if (m_hwnd == nullptr)
			m_hInstance = hInstance;
		else
			Log::Warning("hInstance cannot be changed because the window has already been created");
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
	void HandleWindow::SetParent(HandleWindow& parentWindow) noexcept {
		const HWND newParent = parentWindow.GetHandle();
		if (m_hwnd)
			::SetParent(m_hwnd, newParent);

		if (newParent == nullptr)
			RemoveStyles(WS_CHILD);
		else if (m_pParent == nullptr)
			AddStyles(WS_CHILD);

		m_pParent = &parentWindow;
	}
	void HandleWindow::SetPosition(const Int2D& newPosition) noexcept {
		if (m_hwnd) {
			::SetWindowPos(m_hwnd, nullptr,
				newPosition.x, newPosition.y, m_Rect.Right, m_Rect.Bottom, SWP_NOSIZE);
		}
		m_Rect.LeftTop = newPosition;
	}
	void HandleWindow::SetSize(const Int2D& newSize) noexcept {
		if (m_hwnd) {
			::SetWindowPos(m_hwnd, nullptr,
				m_Rect.Left, m_Rect.Top, newSize.x, newSize.y, SWP_NOMOVE);
		}
		m_Rect.RightBottom = newSize;
	}
	void HandleWindow::SetRect(const IntRect& newRect) noexcept {
		if (m_hwnd) {
			::SetWindowPos(m_hwnd, nullptr,
				newRect.Left, newRect.Top, newRect.Right, newRect.Bottom, 0);
		}
		m_Rect = newRect;
	}
	void HandleWindow::SetName(const String& name) {
		m_Name = name;
		if (m_hwnd)
			SetWindowText(m_hwnd, m_Name.c_str());
	}

#ifdef _WINDEF_
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
	IntRect HandleWindow::GetRect() const noexcept {
		return m_Rect;
	}
	HandleWindow& HandleWindow::GetParent() const noexcept {
		return *m_pParent;
	}
	Int HandleWindow::GetID() const noexcept {
		return m_ID;
	}
	void* HandleWindow::GetParamPtr() const noexcept {
		return m_pParam;
	}
	uInt3D HandleWindow::GetBackgroundColor() const noexcept {
		return m_BackgroundColor;
	}
	Bool HandleWindow::IsShowed() const noexcept {
		return (m_Styles & WS_VISIBLE);
	}

	void HandleWindow::_CreateWindow() {
		const HWND hParent =
			(m_pParent != nullptr) ? m_pParent->GetHandle() : nullptr;

		m_hdc = nullptr;
		m_hwnd = CreateWindowEx(
			m_ExStyles,
			m_ClassName.c_str(), m_Name.c_str(),
			m_Styles,
			m_Rect.Left, m_Rect.Top, m_Rect.Right, m_Rect.Bottom,
			hParent,
			reinterpret_cast<HMENU>(m_ID),
			m_hInstance,
			m_pParam);

		if (m_hwnd == nullptr) {
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

		if (m_IsMenuEnabled)
			::SetMenu(m_hwnd, m_Menu.GetHandle());
		m_hdc = ::GetDC(m_hwnd);

		if (!m_IsWindowEnabled)
			::EnableWindow(m_hwnd, FALSE);
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