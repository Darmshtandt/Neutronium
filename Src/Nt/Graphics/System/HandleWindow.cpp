// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <Nt/Core/WinMinimal.h>

#undef NOUSER
#undef NOGDI
#undef NOWINOFFSETS
#undef NOMSG

#include <windows.h>
#include <shobjidl.h>
#include <shellapi.h>
#include <commoncontrols.h>
#include <wrl/client.h>

#include <Nt/Graphics/System/HandleWindow.h>

#include <Nt/Core/EventBus.h>

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
			Log::Instance().Warning("Handle window pointer is null.");
		else if (!GetClientRect(hwnd, &rect))
			Log::Instance().Warning("Failed to get window client rect.");
		return rect;
	}
	IntRect GetWindowRect(HWND hwnd) noexcept {
		RECT rect = { };
		if (hwnd == nullptr)
			Log::Instance().Warning("Handle window pointer is null.");
		else if (!GetWindowRect(hwnd, &rect))
			Log::Instance().Warning("Failed to get window rect.");
		return rect;
	}
	IntRect AdjustWindowRect(const IntRect& windowRect, const DWord& styles, const Bool& isHasMenu) noexcept {
		RECT rect = windowRect;
		if (!AdjustWindowRect(&rect, styles, isHasMenu))
			Log::Instance().Warning("Failed to get window rect.");
		return rect;
	}
	IntRect AdjustWindowRectEx(const IntRect& windowRect, const DWord& styles, const DWord& exStyles, const Bool& isHasMenu) noexcept {
		RECT rect = windowRect;
		if (!AdjustWindowRectEx(&rect, styles, isHasMenu, exStyles))
			Log::Instance().Warning("Failed to get window rect.");
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

	IntRect AdjustedWindowRect(HWND hwnd, const DWord& styles, const Bool& isHasMenu) noexcept {
		return AdjustWindowRect(GetWindowRect(hwnd), styles, isHasMenu);
	}
	IntRect AdjustedWindowRectEx(HWND hwnd, const DWord& styles, const DWord& exStyles, const Bool& isHasMenu) noexcept {
		return AdjustWindowRectEx(GetWindowRect(hwnd), styles, isHasMenu, exStyles);
	}

	void DrawFrameRect(HDC hdc, const IntRect& rect, const Int& lineWeight, const Byte3D& color) {
		const RECT leftSide = {
			rect.Left, rect.Top,
			rect.Left + lineWeight, rect.Top + rect.Bottom
		};
		const RECT topSide = {
			rect.Left, rect.Top,
			rect.Left + rect.Right, rect.Top + lineWeight
		};
		const RECT rightSide = {
			rect.Left + rect.Right - lineWeight, rect.Top,
			rect.Left + rect.Right, rect.Top + rect.Bottom
		};
		const RECT bottomSide = {
			rect.Left, rect.Top + rect.Bottom - lineWeight,
			rect.Left + rect.Right, rect.Top + rect.Bottom
		};

		const HBRUSH hBrush = CreateSolidBrush(color);
		FillRect(hdc, &leftSide, hBrush);
		FillRect(hdc, &topSide, hBrush);
		FillRect(hdc, &rightSide, hBrush);
		FillRect(hdc, &bottomSide, hBrush);
	}

	HandleWindow::HandleWindow() noexcept :
		m_hInstance(GetModuleHandle(nullptr))
	{
	}

	HandleWindow::HandleWindow(HandleWindow&& window) :
		m_Name(std::move(window.m_Name)),
		m_ClassName(std::move(window.m_ClassName)),
		m_BackgroundColor(window.m_BackgroundColor),
		m_hInstance(window.m_hInstance),
		m_hParent(window.m_hParent),
		m_hwnd(window.m_hwnd),
		m_hdc(window.m_hdc),
		m_pMenu(std::move(window.m_pMenu)),
		m_pParam(window.m_pParam),
		m_ClientRect(window.m_ClientRect),
		m_WindowRect(window.m_WindowRect),
		m_ID(window.m_ID),
		m_ZOrder(window.m_ZOrder),
		m_Styles(window.m_Styles),
		m_ExStyles(window.m_ExStyles),
		m_IsMenuEnabled(window.m_IsMenuEnabled),
		m_IsWindowEnabled(window.m_IsWindowEnabled)
	{
		window.m_hInstance = nullptr;
		window.m_hParent = nullptr;
		window.m_hwnd = nullptr;
		window.m_hdc = nullptr;
		window.m_pParam = nullptr;
		window.m_ID = 0;
	}
	HandleWindow::HandleWindow(const HWND& hwnd) {
		m_hwnd = hwnd;
		if (m_hwnd == nullptr)
			return;

		const uInt nameLength = GetWindowTextLength(m_hwnd);
		m_Name.resize(nameLength);
		GetWindowText(m_hwnd, m_Name.data(), nameLength);

		wChar className[MAX_PATH] = { };
		GetClassName(m_hwnd, className, MAX_PATH);
		m_ClassName = className;

		WNDCLASS wndClass;
		GetClassInfo(m_hInstance, m_ClassName.c_str(), &wndClass);

		COLORREF colorRefBackground;
		GetObject(wndClass.hbrBackground, sizeof(colorRefBackground), &colorRefBackground);
		m_BackgroundColor = ColorRefToVector(colorRefBackground);

		m_pParam = this;
		SetWindowLongPtr(m_hwnd, GWLP_USERDATA, reinterpret_cast<Long>(m_pParam));

		m_hdc = ::GetDC(m_hwnd);
		m_hInstance = reinterpret_cast<HINSTANCE>(GetWindowLongPtr(m_hwnd, GWLP_HINSTANCE));
		m_hParent = reinterpret_cast<HWND>(GetWindowLongPtr(m_hwnd, GWLP_HWNDPARENT));
		m_pMenu = nullptr;
		m_ClientRect = Nt::GetClientRect(m_hwnd);
		m_WindowRect = Nt::GetWindowRect(m_hwnd);
		m_ID = GetWindowLongPtr(m_hwnd, GWLP_ID);;
		m_Styles = GetWindowLongPtr(m_hwnd, GWL_STYLE);
		m_ExStyles = GetWindowLongPtr(m_hwnd, GWL_EXSTYLE);
		m_IsMenuEnabled = false;
		m_IsWindowEnabled = IsWindowEnabled(m_hwnd);
		m_ZOrder = ZORDER_NOTOPMOST;
	}
	HandleWindow::HandleWindow(const IntRect& rect, const String& name) {
		HandleWindow::Create(rect, name);
	}

	void HandleWindow::Create(const IntRect& windowRect, const String& name) {
		m_WindowRect = windowRect;
		m_Name = name;
		_CreateWindow();
	}

	void HandleWindow::Hide() {
		if (!IsCreated())
			Raise("Handle window not created");

		m_Styles &= ~STYLE_VISIBLE;
		ShowWindow(m_hwnd, SW_HIDE);
	}
	void HandleWindow::Show() {
		if (!IsCreated())
			Raise("Handle window not created");

		m_Styles |= STYLE_VISIBLE;
		ShowWindow(m_hwnd, SW_SHOW);
	}
	void HandleWindow::ShowMaximized() {
		if (!IsCreated())
			Raise("Handle window not created");

		m_Styles |= STYLE_VISIBLE;
		ShowWindow(m_hwnd, SW_SHOWMAXIMIZED);
	}

	Bool HandleWindow::EnableWindow() noexcept {
		m_IsWindowEnabled = true;
		if (m_hwnd != nullptr)
			return ::EnableWindow(m_hwnd, TRUE);
		return true;
	}
	Bool HandleWindow::DisableWindow() noexcept {
		m_IsWindowEnabled = false;
		if (m_hwnd != nullptr)
			return ::EnableWindow(m_hwnd, FALSE);
		return true;
	}

	Int2D HandleWindow::ScreenToClient(const Int2D& cursorPosition) const noexcept {
		POINT point = cursorPosition;
		::ScreenToClient(m_hwnd, &point);
		return point;
	}

	Long HandleWindow::AddStyles(const DWord& styles) noexcept {
		return SetStyles(m_Styles | styles);
	}
	Long HandleWindow::RemoveStyles(const DWord& styles) noexcept {
		return SetStyles(m_Styles & (~styles));
	}
	Long HandleWindow::AddExStyles(const DWord& exStyles) noexcept {
		return SetExStyles(m_ExStyles | exStyles);
	}
	Long HandleWindow::RemoveExStyles(const DWord& exStyles) noexcept {
		return SetExStyles(m_ExStyles & (!exStyles));
	}

	void HandleWindow::EnableMenu() noexcept {
		m_IsMenuEnabled = true;
		if (m_pMenu != nullptr && m_hwnd != nullptr)
			::SetMenu(m_hwnd, m_pMenu->GetHandle());
	}
	void HandleWindow::DisableMenu() noexcept {
		m_IsMenuEnabled = false;
		if (m_hwnd != nullptr)
			::SetMenu(m_hwnd, nullptr);
	}

	void HandleWindow::Destroy() noexcept {
		if (m_hwnd != nullptr) {
			DestroyWindow(m_hwnd);
			m_hwnd = nullptr;
			m_hParent = nullptr;
		}
	}

	Bool HandleWindow::InvalidateRect(const IntRect* pRect, const Bool& isErased) noexcept {
		if (m_hwnd == nullptr) {
			Log::Instance().Warning("Handle window not created");
			return false;
		}

		if (pRect == nullptr)
			return ::InvalidateRect(m_hwnd, nullptr, isErased);

		const RECT winapiRect = (*pRect);
		return ::InvalidateRect(m_hwnd, &winapiRect, isErased);
	}

	HandleWindow& HandleWindow::operator = (HandleWindow&& window) {
		if (this == &window)
			return *this;

		m_Name = std::move(window.m_Name);
		m_ClassName = std::move(window.m_ClassName);
		m_BackgroundColor = window.m_BackgroundColor;
		m_hInstance = window.m_hInstance;
		m_hParent = window.m_hParent;
		m_hwnd = window.m_hwnd;
		m_hdc = window.m_hdc;
		m_pMenu = std::move(window.m_pMenu);
		m_pParam = window.m_pParam;
		m_ClientRect = window.m_ClientRect;
		m_WindowRect = window.m_WindowRect;
		m_ID = window.m_ID;
		m_ZOrder = window.m_ZOrder;
		m_Styles = window.m_Styles;
		m_ExStyles = window.m_ExStyles;
		m_IsMenuEnabled = window.m_IsMenuEnabled;
		m_IsWindowEnabled = window.m_IsWindowEnabled;

		return *this;
	}

	void HandleWindow::SetBackgroundColor(const Byte3D& color) {
		m_BackgroundColor = color;
		if (m_hwnd != nullptr) {
			const HBRUSH hColor = CreateSolidBrush(m_BackgroundColor);
			SetClassLongPtr(m_hwnd, GCLP_HBRBACKGROUND, (LONG_PTR)hColor);
			InvalidateRect(nullptr, true);
		}
	}
	void HandleWindow::SetIcon(const HICON& hIcon) {
		if (m_hwnd != nullptr)
			SetClassLongPtr(m_hwnd, GCLP_HICON, reinterpret_cast<Long>(hIcon));
	}
	void HandleWindow::SetIconSmall(const HICON& hIcon) {
		if (m_hwnd != nullptr)
			SetClassLongPtr(m_hwnd, GCLP_HICONSM, reinterpret_cast<Long>(hIcon));
	}
	void HandleWindow::SetIcon(const String& iconPath) {
		const HICON hIcon = (HICON)LoadImageA(nullptr, iconPath.c_str(), IMAGE_ICON,
			0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE | LR_SHARED);
		if (m_hwnd != nullptr)
			SetClassLongPtr(m_hwnd, GCLP_HICON, reinterpret_cast<Long>(hIcon));
	}
	void HandleWindow::SetIconSmall(const String& iconPath) {
		HICON hIcon = (HICON)LoadImageA(nullptr, iconPath.c_str(), IMAGE_ICON,
			0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE | LR_SHARED);
		if (m_hwnd != nullptr)
			SetClassLongPtr(m_hwnd, GCLP_HICONSM, reinterpret_cast<Long>(hIcon));
	}

	Long HandleWindow::SetWindowInfo(const Index& index, const Long& data) noexcept {
		if (m_hwnd == nullptr) {
			Log::Instance().Warning("Handle window not created");
			return 0;
		}

		return SetWindowLongPtr(m_hwnd, index, data);
	}
	void HandleWindow::SetInstance(const HINSTANCE& hInstance) noexcept {
		if (IsCreated()) {
			Log::Instance().Warning("hInstance cannot be changed because the window has already been created");
			return;
		}

		m_hInstance = hInstance;
	}
	void HandleWindow::SetParentHandle(const HWND& hParent) {
		if (m_hwnd != nullptr)
			::SetParent(m_hwnd, hParent);

		if (hParent == nullptr)
			RemoveStyles(STYLE_CHILD);
		else if (m_hParent == nullptr)
			AddStyles(STYLE_CHILD);

		m_hParent = hParent;
	}

	Bool HandleWindow::SetMenu(Menu* pMenu) noexcept {
		m_pMenu = pMenu;

		if (pMenu != nullptr &&m_hwnd != nullptr && m_IsMenuEnabled)
			return ::SetMenu(m_hwnd, m_pMenu->GetHandle());
		return false;
	}
	Long HandleWindow::SetID(const Int& newID) noexcept {
		m_ID = newID;

		if (m_hwnd != nullptr)
			return SetWindowInfo(INDEX_ID, m_ID);
		return 0;
	}
	void HandleWindow::SetParent(const HandleWindow& parentWindow) {
		SetParentHandle(parentWindow.GetHandle());
	}
	Bool HandleWindow::SetWindowPos(const IntRect& rect, const WindowPositionFlags& flags) noexcept {
		if (m_hwnd == nullptr)
			return false;

		const Bool result = ::SetWindowPos(m_hwnd, HWND(m_ZOrder), rect.Left, rect.Top, rect.Right, rect.Bottom, flags);
		if ((flags & POSITIONFLAG_NOMOVE) == 0 || (flags & POSITIONFLAG_NOSIZE) == 0) {
			m_WindowRect = _ComputeRealWindowRect();
			m_ClientRect = _ComputeRealClientRect();

			//if (m_Styles & STYLE_BORDER)
			//	m_ClientRect.RightBottom -= Int2D(GetSystemMetrics(SM_CXDLGFRAME), GetSystemMetrics(SM_CYDLGFRAME));
		}

		if (flags & POSITIONFLAG_SHOWWINDOW)
			m_Styles |= STYLE_VISIBLE;
		else if (flags & POSITIONFLAG_HIDEWINDOW)
			m_Styles &= ~STYLE_VISIBLE;
		return result;
	}
	Bool HandleWindow::SetWindowPos(const ZOrder& order, const IntRect& rect, const WindowPositionFlags& flags) noexcept {
		if (flags & POSITIONFLAG_NOOWNERZORDER || flags & POSITIONFLAG_NOZORDER)
			m_ZOrder = order;
		return SetWindowPos(rect, flags);
	}
	void HandleWindow::SetPosition(const Int2D& newPosition) noexcept {
		if (m_WindowRect.LeftTop == newPosition)
			return;

		const WindowPositionFlags falgs = WindowPositionFlags(POSITIONFLAG_NOSIZE | POSITIONFLAG_NOOWNERZORDER | POSITIONFLAG_NOZORDER);
		SetWindowPos({ newPosition - m_WindowRect.LeftTop, m_WindowRect.RightBottom }, falgs);
	}
	void HandleWindow::SetSize(const Int2D& newSize) noexcept {
		if (m_WindowRect.RightBottom == newSize)
			return;

		const WindowPositionFlags falgs = WindowPositionFlags(POSITIONFLAG_NOMOVE | POSITIONFLAG_NOOWNERZORDER | POSITIONFLAG_NOZORDER);
		SetWindowPos({ m_WindowRect.LeftTop, newSize }, falgs);
	}
	void HandleWindow::SetWindowRect(const IntRect& newRect) noexcept {
		if (m_WindowRect == newRect)
			return;

		if (SetWindowPos(newRect, POSITIONFLAG_NONE)) {
			m_WindowRect = _ComputeRealWindowRect();
			m_ClientRect = _ComputeRealClientRect();
		}
		else {
			m_WindowRect = m_ClientRect = newRect;
		}
	}
	void HandleWindow::SetName(const String& name) {
		m_Name = name;
		if (m_hwnd != nullptr)
			SetWindowText(m_hwnd, m_Name.c_str());
	}
	uInt HandleWindow::SetStyles(const uInt& styles) {
		m_Styles = styles;

		if (m_hwnd != nullptr)
			return SetWindowInfo(INDEX_STYLE, m_Styles);
		return 0;
	}
	uInt HandleWindow::SetExStyles(const uInt& exStyles) {
		m_ExStyles = exStyles;

		if (m_hwnd != nullptr)
			return SetWindowInfo(INDEX_EXSTYLE, m_ExStyles);
		return 0;
	}
	void HandleWindow::SetEventBus(const std::weak_ptr<EventBus>& bus) noexcept {
		m_EventBus = bus;
	}

	std::weak_ptr<EventBus> HandleWindow::GetEventBus() noexcept {
		return m_EventBus;
	}
	HWND HandleWindow::GetParentHandle() const noexcept {
		return m_hParent;
	}
	HWND HandleWindow::GetHandle() const noexcept {
		return m_hwnd;
	}
	HDC HandleWindow::GetDC() const noexcept {
		return m_hdc;
	}
	HDC HandleWindow::GetWindowDC() const noexcept {
		return ::GetWindowDC(m_hwnd);
	}
	HINSTANCE HandleWindow::GetInstance() const noexcept {
		return m_hInstance;
	}

	Long HandleWindow::GetWindowInfo(const Index& index) const noexcept {
		if (m_hwnd == nullptr) {
			Log::Instance().Warning("Handle window not created");
			return 0;
		}

		return GetWindowLongPtr(m_hwnd, index);
	}
	String HandleWindow::GetName() const {
		return m_Name;
	}
	Menu* HandleWindow::GetMenu() noexcept {
		return m_pMenu;
	}
	IntRect HandleWindow::GetClientRect() const noexcept {
		return m_ClientRect;
	}
	IntRect HandleWindow::GetWindowRect() const noexcept {
		return m_WindowRect;
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
	uInt HandleWindow::GetStyles() const noexcept {
		return m_Styles;
	}
	uInt HandleWindow::GetExStyles() const noexcept {
		return m_ExStyles;
	}
	Bool HandleWindow::IsMenuEnabled() const noexcept {
		return m_IsMenuEnabled;
	}
	Bool HandleWindow::IsShowed() const noexcept {
		return (m_Styles & STYLE_VISIBLE);
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

			Raise("Failed to create " + windowType);
		}

		m_WindowRect = _ComputeRealWindowRect();
		m_ClientRect = _ComputeRealClientRect();

		if (m_IsMenuEnabled && m_pMenu != nullptr)
			::SetMenu(m_hwnd, m_pMenu->GetHandle());
		m_hdc = ::GetDC(m_hwnd);

		if (!m_IsWindowEnabled)
			::EnableWindow(m_hwnd, FALSE);

		UpdateWindow(m_hwnd);
	}
	IntRect HandleWindow::_ComputeRealWindowRect() {
		IntRect windowRect = Nt::GetWindowRect(m_hwnd);
		windowRect.RightBottom -= windowRect.LeftTop;
		return windowRect;
	}
	IntRect HandleWindow::_ComputeRealClientRect() {
		const IntRect windowRect = _ComputeRealWindowRect();
		const IntRect adjustedWindowRect = Nt::AdjustWindowRectEx(windowRect, m_Styles, m_ExStyles, m_IsMenuEnabled);

		IntRect clientRect(Int2D(), windowRect.RightBottom);
		clientRect.RightBottom -= adjustedWindowRect.RightBottom - windowRect.RightBottom;
		return clientRect;
	}
	Long HandleWindow::_SendMessage(const uInt& message, const uInt& wParam, const Long& lParam) const {
		if (m_hwnd == nullptr)
			Raise("Handle window not created");
		return SendMessage(m_hwnd, message, wParam, lParam);
	}

	void HandleWindow::EmmitExceptionEvent(std::string text) const {
		if (!m_EventBus.expired())
			m_EventBus.lock()->Emmit<ExceptionEvent>({ std::move(text) });
	}
	void HandleWindow::EventBusProc(uInt uMsg, uInt wParam, Long lParam) const {
		if (m_EventBus.expired())
			return;

		std::shared_ptr<EventBus> pBus = m_EventBus.lock();
		switch (uMsg) {
		case WM_VSCROLL: 
			break;

		case WM_HOTKEY:
			break;

		case WM_KILLFOCUS:
			pBus->Emmit<KillFocusEvent>({ });
			return;

		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN: {
			KeyDownEvent e = { };
			e.Key = (uMsg == WM_RBUTTONDOWN) ? BUTTON_RIGHT : BUTTON_LEFT;
			e.Position = { LOWORD(lParam), HIWORD(lParam) };
			e.HandleID = m_ID;

			pBus->Emmit<KeyDownEvent>(e);
		}
			return;

		case WM_LBUTTONUP:
		case WM_RBUTTONUP: {
			KeyDownEvent e = { };
			e.Key = (uMsg == WM_RBUTTONUP) ? BUTTON_RIGHT : BUTTON_LEFT;
			e.Position = { LOWORD(lParam), HIWORD(lParam) };
			e.HandleID = m_ID;

			pBus->Emmit<KeyDownEvent>(e);
		}
			return;

		case WM_COMMAND:
			pBus->Emmit<CommandEvent>({ wParam, lParam, m_ID });
			return;

		case WM_SIZING:
			break;
		case WM_SIZE:
			break;

		case WM_KEYDOWN:
			pBus->Emmit<KeyDownEvent>({ Int2D(), Key(wParam), m_ID });
			return;

		case WM_KEYUP:
			pBus->Emmit<KeyUpEvent>({ Int2D(), Key(wParam), m_ID });
			return;

		case WM_NCMOUSEHOVER:
			break;
		case WM_NCMOUSELEAVE:
			break;
		case WM_NCMOUSEMOVE:
			break;

		case WM_MOUSEMOVE:
			break;
		case WM_MOUSEACTIVATE:
			break;
		case WM_MOUSEHOVER:
			break;
		case WM_MOUSELEAVE:
			break;
		case WM_MOUSEHWHEEL:
			break;
		case WM_MOUSEWHEEL:
			break;

		case WM_NCPAINT:
			break;
		case WM_PAINT:
			break;

		case WM_CLOSE:
			break;

		case WM_DESTROY:
			break;
		}

		pBus->Emmit<OtherEvent>({ uMsg, wParam, lParam, m_ID });
	}

	HandleWindow* HandleWindow::GetHandleFromUserData(HWND hwnd, const uInt& uMsg, LPARAM lParam) {
		HandleWindow* pThis = nullptr;
		if (uMsg == WM_CREATE) {
			const CREATESTRUCT* pWindowStruct = reinterpret_cast<CREATESTRUCT*>(lParam);
			pThis = static_cast<HandleWindow*>(pWindowStruct->lpCreateParams);
			RequireNotNull(pThis);

			pThis->m_hwnd = hwnd;

			SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<Long>(pThis));
		}
		else {
			pThis = reinterpret_cast<HandleWindow*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
		}

		return pThis;
	}
	LRESULT HandleWindow::SubClassProc(HWND hwnd, uInt uMsg, uInt wParam, Long lParam, uInt uIdSubclass, uLong dwRefData) {
		(void)dwRefData;

		HandleWindow* pThis = nullptr;

		try {
			pThis = GetHandleFromUserData(hwnd, uMsg, lParam);
			if (pThis != nullptr)
				pThis->EventBusProc(uMsg, wParam, lParam);
		}
		catch (const Error& error) {
			if (pThis != nullptr)
				pThis->EmmitExceptionEvent(error.what());
			error.Show();
		}
		catch (const std::exception& error) {
			if (pThis != nullptr)
				pThis->EmmitExceptionEvent(error.what());
		}

		if (uMsg == WM_DESTROY)
			RemoveWindowSubclass(hwnd, SubClassProc, uIdSubclass);
		return DefSubclassProc(hwnd, uMsg, wParam, lParam);
	}

	String OpenFileDialog(const String& startPath, const _FILEOPENDIALOGOPTIONS& options) {
		String filePath;
		
		Microsoft::WRL::ComPtr<IFileDialog> pFileDialog = nullptr;
		Microsoft::WRL::ComPtr<IShellItem> pItem = nullptr;

		try {
			HRESULT hResult = CoCreateInstance(CLSID_FileOpenDialog, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pFileDialog));
			if (FAILED(hResult))
				throw hResult;

			DWord dwOptions;
			pFileDialog->GetOptions(&dwOptions);
			pFileDialog->SetOptions(dwOptions | options);

			const std::wstring wStartPath(startPath);

			hResult = SHCreateItemFromParsingName(
				wStartPath.c_str(), nullptr, IID_PPV_ARGS(&pItem));
			if (SUCCEEDED(hResult))
				pFileDialog->SetFolder(pItem.Get());

			hResult = pFileDialog->Show(nullptr);
			if (FAILED(hResult))
				throw hResult;

			pItem = nullptr;
			hResult = pFileDialog->GetResult(&pItem);
			if (FAILED(hResult))
				throw hResult;

			wChar* pszFolderPath = nullptr;
			hResult = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFolderPath);
			if (FAILED(hResult))
				throw hResult;

			filePath = pszFolderPath;
			CoTaskMemFree(pszFolderPath);
		}
		catch (const HRESULT& hResult) {
			if (hResult != HRESULT_FROM_WIN32(ERROR_CANCELLED)) {
				String errorMessage = "Failed to open file dialog.\nError code: 0x";
				errorMessage += HRESULT_CODE(hResult);
				Raise(errorMessage);
			}
		}

		return filePath;
	}
}
