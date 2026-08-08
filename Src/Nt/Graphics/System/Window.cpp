// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <Nt/Core/WinMinimal.h>

#undef NOUSER
#undef NOMSG
#undef NOWINOFFSETS
#undef NOGDI

#include <windows.h>
#include <windowsx.h>
#include <commdlg.h>
#include <shobjidl.h>

#include <filesystem>

#include <Nt/Graphics/System/Window.h>

#include <cassert>
#include <Nt/Core/MessageWindow.h>
#include <Nt/Core/EventBus.h>

namespace Nt {
	Window::Window() noexcept :
		m_Msg(),
		m_hMenu(nullptr),
		m_IsOpened(false)
	{
		m_Styles |= (STYLE_OVERLAPPEDWINDOW | STYLE_CLIPCHILDREN | STYLE_CLIPSIBLINGS);
	}
	Window::Window(const IntRect& rect, const String& name) :
		m_Msg(),
		m_hMenu(nullptr)
	{
		m_Styles |= (STYLE_OVERLAPPEDWINDOW | STYLE_CLIPCHILDREN | STYLE_CLIPSIBLINGS);
		Window::Create(rect, name);
	}
	Window::Window(const Int2D& size, const String& name) :
		m_Msg(),
		m_hMenu(nullptr)
	{
		m_Styles |= (STYLE_OVERLAPPEDWINDOW | STYLE_CLIPCHILDREN | STYLE_CLIPSIBLINGS);
		Window::Create(size, name);
	}
	Window::Window(const String& name) :
		m_Msg(),
		m_hMenu(nullptr)
	{
		m_Styles |= (STYLE_OVERLAPPEDWINDOW | STYLE_CLIPCHILDREN | STYLE_CLIPSIBLINGS);
		Window::Create(name);
	}

	Window::~Window() {
		Destroy();
	}

	void Window::Create(const String& name) {
		constexpr IntRect defaultWindowRect =  { 
			CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT 
		};

		if (m_WindowRect.Right == 0 || m_WindowRect.Bottom == 0)
			Create(defaultWindowRect, name);
		else
			Create(m_WindowRect, name);
	}
	void Window::Create(const Int2D& size, const String& name) {
		IntRect windowRect;
		windowRect.RightBottom = size;
		windowRect.Left = (GetMonitorSize().x - size.x) / 2;
		windowRect.Top = (GetMonitorSize().y - size.y) / 2;
		Create(windowRect, name);
	}
	void Window::Create(const IntRect& windowRect, const String& name) {
		m_WindowRect = windowRect;
		m_Name = name;
		m_ClassName = m_Name + L"Class";

		SetLastError(0);

		WNDCLASS wndClass = { };
		wndClass.hbrBackground = CreateSolidBrush(m_BackgroundColor);
		wndClass.hInstance = GetModuleHandle(nullptr);
		wndClass.lpfnWndProc = _BaseWndProc;
		wndClass.lpszClassName = m_ClassName.c_str();

		if (!RegisterClass(&wndClass) && GetLastError() == 1410) {
			for (uInt i = 0; i < 100; ++i) {
				const std::wstring newClassName = m_ClassName + std::to_wstring(i);
				wndClass.lpszClassName = newClassName.c_str();

				const Word result = RegisterClass(&wndClass);
				const DWord errorCode = GetLastError();

				if (result) {
					m_ClassName = newClassName;
					wndClass.lpszClassName = m_ClassName.c_str();
					break;
				}
				else if (errorCode != 1410) {
					String errorMsg = "Failed to create window class.\nClass name: \"";
					errorMsg += wndClass.lpszClassName;
					errorMsg += "\". Error code: ";
					errorMsg += errorCode;
					Raise(errorMsg);
				}
			}
		}

		m_pParam = this;

		_CreateWindow();

		m_IsOpened = true;
	}

	void Window::RegisterHotKey(const uInt& modifiers, const Key& key, const std::function<void()>& action) {
		Assert(action, "Empty action");
		Assert(m_hwnd != nullptr, "Window not created");

		const uInt id = _GenerateID();
		m_HotKeyMap[id] = action;

		::RegisterHotKey(m_hwnd, id, modifiers, uInt(key));
	}

	void Window::RegisterWindowListener(const NotNull<WindowListener*>& listener) {
		m_WindowListeners.push_back(listener);
	}
	void Window::RegisterKeyboardListener(const NotNull<KeyboardListener*>& listener) {
		m_KeyboardListeners.push_back(listener);
	}
	void Window::RegisterMouseListener(const NotNull<MouseListener*>& listener) {
		m_MouseListeners.push_back(listener);
	}

	template <class _Ty>
	void UnregisterListener(std::list<_Ty*>& listenerList, const NotNull<_Ty*>& listener) {
		typename std::list<_Ty*>::const_iterator iterator =
			std::find(listenerList.cbegin(), listenerList.cend(), listener);

		if (iterator != listenerList.cend())
			listenerList.erase(iterator);
	}

	void Window::UnregisterWindowListener(const NotNull<WindowListener*>& listener) {
		UnregisterListener<WindowListener>(m_WindowListeners, listener);
	}
	void Window::UnregisterKeyboardListener(const NotNull<KeyboardListener*>& listener) {
		UnregisterListener<KeyboardListener>(m_KeyboardListeners, listener);
	}
	void Window::UnregisterMouseListener(const NotNull<MouseListener*>& listener) {
		UnregisterListener<MouseListener>(m_MouseListeners, listener);
	}

	void Window::PeekMessages() {
		while (PeekMessage(&m_Msg, nullptr, 0, 0, PM_REMOVE)) {
			TranslateMessage(&m_Msg);
			DispatchMessage(&m_Msg);
		}
	}
	Bool Window::PeekMessages(Event* pEvent) {
		PeekMessages();

		if (pEvent != nullptr) {
			if (m_Events.size() == 0)
				return false;

			(*pEvent) = m_Events.front();
			m_Events.pop();
			return (pEvent->Type != Event::Types::NONE);
		}

		Log::Instance().Warning("Event pointer is null");
		return false;
	}

	void Window::Close() noexcept {
		if (m_hwnd != nullptr)
			SendMessage(m_hwnd, WM_CLOSE, 0, 0);
	}

	void Window::Destroy() noexcept {
		m_IsOpened = false;
		HandleWindow::Destroy();
	}

	void Window::EnableSystemBorder() noexcept {
		if (!m_EnabledSystemBorder)
			m_EnabledSystemBorder = true;
	}

	void Window::DisableSystemBorder() noexcept {
		if (m_EnabledSystemBorder)
			m_EnabledSystemBorder = false;
	}

	Bool Window::IsOpened() const noexcept {
		return m_IsOpened;
	}
	Int2D Window::GetPosition() const noexcept {
		return m_WindowRect.LeftTop;
	}
	Int2D Window::GetSize() const noexcept {
		return m_WindowRect.RightBottom;
	}
	Int2D Window::GetClientSize() const noexcept {
		return m_ClientRect.RightBottom;
	}

	Byte3D Window::GetBorderColor() const noexcept {
		return m_BorderColor;
	}

	IntRect Window::GetClientRect() const noexcept {
		return m_ClientRect;
	}

	void Window::SetProcedure(Procedure procedure) noexcept {
		m_Procedure = std::move(procedure);
	}
	void Window::SetBorderColor(const Byte3D& color) noexcept {
		if (m_BorderColor == color)
			return;

		m_BorderColor = color;
		if (m_hwnd != nullptr)
			_SendMessage(WM_NCPAINT, 0, 0);
	}

	void Window::_AddEvent(const Event::Types& type, const DWord& value) {
		m_Events.push({ type, (Long)value });
		if (m_Events.size() > 25)
			m_Events.pop();
	}

	uInt Window::_GenerateID() const noexcept {
		static uInt id = 0;
		return ++id;
	}

	void Window::_Creation(const CREATESTRUCT* pWindowStruct) {
		(void)pWindowStruct;
	}

	void Window::_Resize(const uInt2D& windowSize) {
		(void)windowSize;
	}

	Bool Window::_NoClientPaint(HRGN hRegion) {
		if (m_EnabledSystemBorder)
			return false;

		RECT rect = {
			0, 0,
			m_WindowRect.Left + m_WindowRect.Right,
			m_WindowRect.Top + m_WindowRect.Bottom
		};

		if ((m_Styles & STYLE_CAPTION) == STYLE_CAPTION) {
			rect.top += GetSystemMetrics(SM_CYSMCAPTION);
			DefWindowProc(m_hwnd, WM_NCPAINT, reinterpret_cast<WPARAM>(hRegion), 0);
		}

		if (GetSystemMenu(m_hwnd, FALSE) != nullptr)
			rect.top += GetSystemMetrics(SM_CYMENU);

		HRGN hWindow = CreateRectRgnIndirect(&rect);
		HDC hdc = GetDCEx(m_hwnd, hWindow, DCX_WINDOW | DCX_INTERSECTRGN | DCX_CACHE);

		RECT frameRect = rect;
		frameRect.right -= m_WindowRect.Left;
		frameRect.bottom -= m_WindowRect.Top;

		HBRUSH hBrush = Nt::CreateSolidBrush(m_BorderColor);
		if (m_Styles & STYLE_BORDER) {
			FrameRect(hdc, &frameRect, hBrush);
		}
		else if (m_Styles & STYLE_DLGFRAME) {
			for (uInt i = 0; i < 3; ++i) {
				FrameRect(hdc, &frameRect, hBrush);
				InflateRect(&frameRect, -1, -1);
			}
		}

		DeleteObject(hBrush);
		DeleteObject(hWindow);
		ReleaseDC(m_hwnd, hdc);

		return true;
	}

	void Window::_Paint(HDC& hdc, PAINTSTRUCT& paint) {
		(void)hdc;
		(void)paint;
	}

	void Window::_Command(const Long& param_1, const Long& param_2) {
		(void)param_1;
		(void)param_2;
	}

	IntRect LParamToIntRect(const LPARAM& lParam) {
		if (lParam == 0)
			return { };
		return *reinterpret_cast<RECT*>(lParam);
	}

	Window* Window::_GetHandleFromUserData(HWND hwnd, const uInt& uMsg, LPARAM lParam) {
		Window* pThis = nullptr;
		if (uMsg == WM_CREATE) {
			const CREATESTRUCT* pWindowStruct = reinterpret_cast<CREATESTRUCT*>(lParam);
			pThis = reinterpret_cast<Window*>(pWindowStruct->lpCreateParams);
			RequireNotNull(pThis);

			pThis->m_hwnd = hwnd;
			pThis->_AddEvent(Event::Types::WINDOW_CREATED, 0);
			pThis->_Creation(pWindowStruct);

			for (WindowListener* listener : pThis->m_WindowListeners)
				listener->Create();

			SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<Long>(pThis));
		}
		else {
			pThis = reinterpret_cast<Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
		}

		return pThis;
	}

	void Window::_VerticalScroll(HWND hwnd, WPARAM wParam) {
		SCROLLINFO scrollInfo;
		scrollInfo.cbSize = sizeof(SCROLLINFO);

		scrollInfo.fMask = SIF_ALL;
		GetScrollInfo(hwnd, SB_VERT, &scrollInfo);

		Int currentPos = scrollInfo.nPos;

		switch (LOWORD(wParam)) {
		case SB_LINEUP:
			scrollInfo.nPos -= 1;
			break;
		case SB_LINEDOWN:
			scrollInfo.nPos += 1;
			break;
		case SB_THUMBTRACK:
			scrollInfo.nPos = scrollInfo.nTrackPos;
			break;
		default:
			return;
		}

		scrollInfo.fMask = SIF_POS;
		SetScrollInfo(hwnd, SB_VERT, &scrollInfo, TRUE);
		GetScrollInfo(hwnd, SB_VERT, &scrollInfo);

		Int yScroll = currentPos - scrollInfo.nPos;
		ScrollWindow(hwnd, 0, yScroll, NULL, NULL);
	}

	LRESULT CALLBACK Window::_BaseWndProc(HWND hwnd, uInt uMsg, WPARAM wParam, LPARAM lParam) {
		Window* pThis = nullptr;

		try {
			pThis = static_cast<Window*>(GetHandleFromUserData(hwnd, uMsg, lParam));
			if (pThis == nullptr)
				return DefWindowProc(hwnd, uMsg, wParam, lParam);

			pThis->EventBusProc(uMsg, wParam, lParam);

			switch (uMsg) {
			case WM_CREATE:
				pThis->_AddEvent(Event::Types::WINDOW_CREATED, 0);
				pThis->_Creation(reinterpret_cast<CREATESTRUCT*>(lParam));

				for (WindowListener* listener : pThis->m_WindowListeners)
					listener->Create();
				break;

			case WM_VSCROLL: 
				pThis->_VerticalScroll(hwnd, wParam);
				break;

			case WM_HOTKEY:
				if (wParam > 0) {
					if (!pThis->m_HotKeyMap.contains(wParam))
						Raise("Unknown hot key");

					pThis->m_HotKeyMap[wParam]();
					return NULL;
				}
				break;

			case WM_LBUTTONDOWN:
				SetFocus(hwnd);
				for (MouseListener* listener : pThis->m_MouseListeners)
					listener->ButtonPressed(BUTTON_LEFT);
				break;
			case WM_RBUTTONDOWN:
				SetFocus(hwnd);
				for (MouseListener* listener : pThis->m_MouseListeners)
					listener->ButtonPressed(BUTTON_RIGHT);
				break;

			case WM_LBUTTONUP:
				SetFocus(hwnd);
				for (MouseListener* listener : pThis->m_MouseListeners)
					listener->ButtonReleased(BUTTON_LEFT);
				break;
			case WM_RBUTTONUP:
				SetFocus(hwnd);
				for (MouseListener* listener : pThis->m_MouseListeners)
					listener->ButtonReleased(BUTTON_RIGHT);
				break;

			case WM_COMMAND:
				pThis->_Command(wParam, lParam);
				break;

			case WM_SIZING:
			{
				if (lParam == 0)
					break;

				IntRect rect = LParamToIntRect(lParam);
				for (WindowListener* listener : pThis->m_WindowListeners)
					listener->Resizing(ResizingType(wParam), rect);

				*reinterpret_cast<RECT*>(lParam) = rect;
			}
				return TRUE;
			case WM_SIZE:
			{
				const Int2D newSize = { LOWORD(lParam), HIWORD(lParam) };

				pThis->m_ClientRect.RightBottom = newSize;
				pThis->m_WindowRect = GetMappedWindowRect(pThis->m_hwnd, pThis->m_hParent);
				pThis->m_WindowRect.RightBottom -= pThis->m_WindowRect.LeftTop;

				pThis->m_ClientRect = pThis->_ComputeRealClientRect();
				//pThis->m_WindowRect.RightBottom += newSize - pThis->m_ClientRect.RightBottom;
				//pThis->m_ClientRect.RightBottom = newSize;

				pThis->_AddEvent(Event::Types::WINDOW_RESIZE, 0);
				pThis->_Resize(pThis->m_ClientRect.RightBottom);

				for (WindowListener* listener : pThis->m_WindowListeners)
					listener->Resize(ResizeType(wParam), newSize);
			}
			return DefWindowProc(hwnd, uMsg, wParam, lParam);
				return NULL;

			case WM_KEYDOWN:
				pThis->_AddEvent(Event::Types::KEY_DOWN, wParam);
				for (KeyboardListener* listener : pThis->m_KeyboardListeners)
					listener->KeyPressed(Key(wParam));
				break;

			case WM_KEYUP:
				pThis->_AddEvent(Event::Types::KEY_UP, wParam);
				for (KeyboardListener* listener : pThis->m_KeyboardListeners)
					listener->KeyReleased(Key(wParam));
				break;

			case WM_NCMOUSEHOVER:
			{
				const Int2D position = { GET_X_LPARAM(lParam), GET_X_LPARAM(lParam) };

				pThis->_AddEvent(Event::Types::NOT_CLIENT_MOUSE_HOVER, wParam);
				for (MouseListener* listener : pThis->m_MouseListeners)
					listener->NoClientHover(HitTest(wParam), position);
			}
				break;
			case WM_NCMOUSELEAVE:
				pThis->_AddEvent(Event::Types::NOT_CLIENT_MOUSE_LEAVE, wParam);
				for (MouseListener* listener : pThis->m_MouseListeners)
					listener->NoClientLeave();
				break;
			case WM_NCMOUSEMOVE:
			{
				const Int2D position = { GET_X_LPARAM(lParam), GET_X_LPARAM(lParam) };

				pThis->_AddEvent(Event::Types::NOT_CLIENT_MOUSE_MOVE, wParam);
				for (MouseListener* listener : pThis->m_MouseListeners)
					listener->NoClientMove(HitTest(wParam), position);
			}
				break;

			case WM_MOUSEMOVE:
			{
				const Int2D position = { LOWORD(lParam), HIWORD(lParam) };

				pThis->_AddEvent(Event::Types::MOUSE_MOVE, wParam);
				for (MouseListener* listener : pThis->m_MouseListeners)
					listener->Move(MouseMode(wParam), position);
			}
				break;
			case WM_MOUSEACTIVATE:
				pThis->_AddEvent(Event::Types::MOUSE_ACTIVATE, wParam);
				break;
			case WM_MOUSEHOVER:
			{
				const Int2D position = { LOWORD(lParam), HIWORD(lParam) };

				pThis->_AddEvent(Event::Types::MOUSE_HOVER, wParam);
				for (MouseListener* listener : pThis->m_MouseListeners)
					listener->Hover(MouseMode(wParam), position);
			}
				break;
			case WM_MOUSELEAVE:
				pThis->_AddEvent(Event::Types::MOUSE_LEAVE, wParam);
				for (MouseListener* listener : pThis->m_MouseListeners)
					listener->Leave();
				break;
			case WM_MOUSEHWHEEL:
			{
				const Int2D position = { LOWORD(lParam), HIWORD(lParam) };

				pThis->_AddEvent(Event::Types::MOUSE_HWHEEL, wParam);
				for (MouseListener* listener : pThis->m_MouseListeners)
					listener->HorizontalWheel(MouseMode(LOWORD(wParam)), HIWORD(wParam), position);

			}
				break;
			case WM_MOUSEWHEEL:
			{
				const Int2D position = { LOWORD(lParam), HIWORD(lParam) };

				pThis->_AddEvent(Event::Types::MOUSE_HWHEEL, wParam);
				for (MouseListener* listener : pThis->m_MouseListeners)
					listener->Wheel(MouseMode(LOWORD(wParam)), HIWORD(wParam), position);

			}
				break;

			case WM_NCPAINT:
				if (pThis->_NoClientPaint(reinterpret_cast<HRGN>(wParam)))
					return 0;
				return DefWindowProc(hwnd, uMsg, wParam, lParam);

			case WM_PAINT:
			{
				PAINTSTRUCT paint;
				HDC hdc = BeginPaint(pThis->m_hwnd, &paint);
				pThis->_Paint(hdc, paint);
				EndPaint(pThis->m_hwnd, &paint);
			}
				return DefWindowProc(hwnd, uMsg, wParam, lParam);

			case WM_CLOSE:
				pThis->_AddEvent(Event::Types::WINDOW_CLOSE, 0);
				pThis->m_IsOpened = false;

				for (WindowListener* listener : pThis->m_WindowListeners)
					listener->Close();
				break;

			case WM_DESTROY:
				pThis->Destroy();
				PostQuitMessage(0);

				wglDeleteContext(wglGetCurrentContext());
				wglMakeCurrent(nullptr, nullptr);

				SetWindowLongPtr(pThis->m_hwnd, GWLP_USERDATA, 0);
				break;
			}

			if (pThis->m_Procedure)
				return pThis->m_Procedure(uMsg, wParam, lParam);
			return DefWindowProc(hwnd, uMsg, wParam, lParam);
		}
		catch (const Error& error) {
			if (pThis != nullptr)
				pThis->EmmitExceptionEvent(error.what());
			error.Show();
		}
		catch (const std::exception& error) {
			if (pThis != nullptr)
				pThis->EmmitExceptionEvent(error.what());
			MessageWindow(error.what(), "Error").Show(MessageIcon::ERROR);
		}

		return 0;
	}

	String FileDialog(cwString FilePath, cwString Filter, const Bool& IsOpenFile) {
		std::filesystem::path initialPath = std::filesystem::current_path();

		std::wstring wPath(MAX_PATH, L'\0');
		if (FilePath)
			wcsncpy_s(wPath.data(), MAX_PATH, FilePath, _TRUNCATE);

		ShowCursor(TRUE);
		SetLastError(0);

		std::wstring filePath(MAX_PATH, L'\0');

		OPENFILENAMEW ofn = { };
		ofn.nMaxFile = MAX_PATH;
		ofn.nFilterIndex = 1;
		ofn.lStructSize = sizeof(ofn);
		ofn.lpstrDefExt = L"";
		ofn.lpstrFilter = Filter;
		ofn.lpstrInitialDir = &wPath[0];
		ofn.lpstrFile = filePath.data();
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
			return filePath;

		DWord errorCode = GetLastError();
		if (errorCode != 0)
			Raise(String("Failed to open file dialog") + String(errorCode));
		return L"";
	}

	String OpenFileDialog(cwString FilePath, cwString Filter) noexcept {
		return FileDialog(FilePath, Filter, true);
	}
	String SaveAsFileDialog(cwString FilePath, cwString Filter) noexcept {
		return FileDialog(FilePath, Filter, false);
	}
}