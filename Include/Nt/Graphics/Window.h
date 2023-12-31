﻿#pragma once

namespace Nt {
	struct Event {
		enum Types : unsigned {
			NONE,

			WINDOW_CREATED,
			WINDOW_RESIZE,
			WINDOW_CLOSE,

			KEY_DOWN,
			KEY_UP,

			MOUSE_MOVE,
			MOUSE_ACTIVATE,
			MOUSE_HOVER,
			MOUSE_LEAVE,
			MOUSE_WHEEL,
			MOUSE_HWHEEL,

			NOT_CLIENT_MOUSE_HOVER,
			NOT_CLIENT_MOUSE_LEAVE,
			NOT_CLIENT_MOUSE_MOVE,
		};

		Types Type;
		Long Value;
	};

	class Window : public HandleWindow {
	public:
		using Procedure = std::function<Long(const uInt& messageID, const uInt& param_1, const Long& param_2)>;

	public:
		Window() noexcept :
			m_Msg(MSG { }),
			m_hMenu(nullptr),
			m_IsOpened(false)
		{
			m_Styles |= (WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS);
		}
		Window(const IntRect& rect, const String& name) {
			m_Styles |= (WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS);
			Create(rect, name);
		}
		Window(const Int2D& size, const String& name) {
			m_Styles |= (WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS);
			Create(size, name);
		}
		Window(const String& name) {
			m_Styles |= (WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS);
			Create(name);
		}

		virtual void Create(const String& name) {
			constexpr IntRect defaultWindowRect = 
				{ CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT };

			if (m_WindowRect.Right == 0 || m_WindowRect.Bottom == 0)
				Create(defaultWindowRect, name);
			else
				Create(m_WindowRect, name);
		}
		virtual void Create(const Int2D& size, const String& name) {
			IntRect windowRect;
			windowRect.RightBottom = size;
			windowRect.Left = (GetMonitorSize().x - size.x) / 2;
			windowRect.Top = (GetMonitorSize().y - size.y) / 2;
			Create(windowRect, name);
		}
		virtual void Create(const IntRect& windowRect, const String& name) override {
			m_WindowRect = windowRect;
			m_Name = name;
			m_ClassName = m_Name + L"Class";

			WNDCLASS wndClass = { };
			wndClass.hbrBackground = CreateSolidBrush(m_BackgroundColor);
			wndClass.hInstance = GetModuleHandle(nullptr);
			wndClass.lpfnWndProc = _BaseWndProc;
			wndClass.lpszClassName = m_ClassName.c_str();

			if ((!RegisterClass(&wndClass)) && GetLastError() == 1410) {
				for (uInt i = 0; i < 100; ++i) {
					const std::wstring newClassName = m_ClassName + std::to_wstring(i);
					wndClass.lpszClassName = newClassName.c_str();

					if (RegisterClass(&wndClass)) {
						m_ClassName = newClassName;
						wndClass.lpszClassName = m_ClassName.c_str();
						break;
					}
					else if (GetLastError() != 1410) {
						String errorMsg = "Failed to create window class.\nClass name: \"";
						errorMsg += wndClass.lpszClassName;
						errorMsg += "\". Error code: ";
						errorMsg += GetLastError();
						Raise(errorMsg);
					}
				}
			}

			m_pParam = this;

			_CreateWindow();

			m_IsOpened = true;
		}

		void PeekMessages() {
			while (PeekMessage(&m_Msg, nullptr, 0, 0, PM_REMOVE)) {
				TranslateMessage(&m_Msg);
				DispatchMessage(&m_Msg);
			}
		}
		Bool PopEvent(Event* pEvent) {
			if (!pEvent)
				Raise("pEvent is nullptr");

			if (m_Events.size() == 0)
				return false;

			(*pEvent) = m_Events.front();
			m_Events.pop();
			return (pEvent->Type != Event::Types::NONE);
		}

		void Close() noexcept {
			if (m_hwnd)
				SendMessage(m_hwnd, WM_CLOSE, 0, 0);
		}

		void Destroy() noexcept {
			m_IsOpened = false;
			HandleWindow::Destroy();
		}

		Bool IsOpened() const noexcept {
			return m_IsOpened;
		}
		IntRect GetClientRect() const noexcept {
			return m_ClientRect;
		}

		void SetProcedure(Procedure procedure) {
			m_Procedure = procedure;
		}

	private:
		std::queue<Event> m_Events;
		Procedure m_Procedure;
		MSG m_Msg;
		HMENU m_hMenu;
		Bool m_IsOpened;

	private:
		virtual void _WMCreate(const CREATESTRUCT* pWindowStruct)
		{ 
		}
		virtual void _WMResize(const uInt2D& windowSize)
		{ 
		}
		virtual Bool _WMNCPaint(HDC& hdc) { 
			return true;
		}
		virtual void _WMPaint(HDC& hdc, PAINTSTRUCT& paint) 
		{ 
		}
		virtual void _WMCommand(const Long& param_1, const Long& param_2) 
		{ 
		}

		void _AddEvent(const Event::Types& type, const DWord& value) {
			m_Events.push({ type, (Long)value });
			if (m_Events.size() > 25)
				m_Events.pop();
		}

		static LRESULT CALLBACK _BaseWndProc(HWND hwnd, uInt uMsg, WPARAM wParam, LPARAM lParam) {
			try {
				Window* pThis = nullptr;
				if (uMsg == WM_CREATE) {
					const CREATESTRUCT* pWindowStruct = reinterpret_cast<CREATESTRUCT*>(lParam);
					pThis = reinterpret_cast<Window*>(pWindowStruct->lpCreateParams);
					if (!pThis)
						Raise("Window is nullptr");

					pThis->m_hwnd = hwnd;
					pThis->_AddEvent(Event::Types::WINDOW_CREATED, 0);
					pThis->_WMCreate(pWindowStruct);
					SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<Long>(pThis));
				}
				else {
					pThis = reinterpret_cast<Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
				}

				if (!pThis)
					return DefWindowProc(hwnd, uMsg, wParam, lParam);

				switch (uMsg) {
				case WM_VSCROLL: {
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
						return 0;
					}

					scrollInfo.fMask = SIF_POS;
					SetScrollInfo(hwnd, SB_VERT, &scrollInfo, TRUE);
					GetScrollInfo(hwnd, SB_VERT, &scrollInfo);

					Int yScroll = currentPos - scrollInfo.nPos;
					ScrollWindow(hwnd, 0, yScroll, NULL, NULL);
				}
					return 0;
				case WM_RBUTTONDOWN:
				case WM_LBUTTONDOWN:
					SetFocus(hwnd);
					break;
				case WM_COMMAND:
					pThis->_WMCommand(wParam, lParam);
					break;
				case WM_SIZE:
					pThis->_AddEvent(Event::Types::WINDOW_RESIZE, 0);
					pThis->_WMResize(pThis->m_WindowRect.RightBottom);
					break;
				case WM_KEYDOWN:
					pThis->_AddEvent(Event::Types::KEY_DOWN, wParam);
					break;
				case WM_KEYUP:
					pThis->_AddEvent(Event::Types::KEY_UP, wParam);
					break;

				case WM_NCMOUSEHOVER:
					pThis->_AddEvent(Event::Types::NOT_CLIENT_MOUSE_HOVER, wParam);
					break;
				case WM_NCMOUSELEAVE:
					pThis->_AddEvent(Event::Types::NOT_CLIENT_MOUSE_LEAVE, wParam);
					break;
				case WM_NCMOUSEMOVE:
					pThis->_AddEvent(Event::Types::NOT_CLIENT_MOUSE_MOVE, wParam);
					break;

				case WM_MOUSEMOVE:
					pThis->_AddEvent(Event::Types::MOUSE_MOVE, wParam);
					break;
				case WM_MOUSEACTIVATE:
					pThis->_AddEvent(Event::Types::MOUSE_ACTIVATE, wParam);
					break;
				case WM_MOUSEHOVER:
					pThis->_AddEvent(Event::Types::MOUSE_HOVER, wParam);
					break;
				case WM_MOUSELEAVE:
					pThis->_AddEvent(Event::Types::MOUSE_LEAVE, wParam);
					break;
				case WM_MOUSEHWHEEL:
					pThis->_AddEvent(Event::Types::MOUSE_HWHEEL, wParam);
					break;
				case WM_MOUSEWHEEL:
					pThis->_AddEvent(Event::Types::MOUSE_WHEEL, GET_WHEEL_DELTA_WPARAM(wParam));
					break;

				case WM_NCPAINT:
				{
					HDC hdc = GetDCEx(hwnd, nullptr, DCX_WINDOW | DCX_INTERSECTRGN);
					const Bool result = pThis->_WMNCPaint(hdc);
					if (!result)
						return 0;
				}
				break;

				case WM_PAINT:
				{
					PAINTSTRUCT paint;
					HDC hdc = BeginPaint(pThis->m_hwnd, &paint);
					pThis->_WMPaint(hdc, paint);
					EndPaint(pThis->m_hwnd, &paint);
				}
				break;
				case WM_CLOSE:
					pThis->_AddEvent(Event::Types::WINDOW_CLOSE, 0);
					pThis->m_IsOpened = false;
					break;

				case WM_DESTROY:
					pThis->Destroy();
					PostQuitMessage(0);

					wglDeleteContext(wglGetCurrentContext());
					wglMakeCurrent(nullptr, nullptr);

					SetWindowLongPtr(pThis->m_hwnd, GWLP_USERDATA, 0);
					return NULL;
				}

				if (pThis->m_Procedure)
					return pThis->m_Procedure(uMsg, wParam, lParam);
				return DefWindowProc(hwnd, uMsg, wParam, lParam);
			}
			catch (const Nt::Error& error) {
				error.Show();
			}
			catch (const std::exception& error) {
				ErrorBoxA(error.what(), "Error");
			}
			return NULL;
		}
	};

	NT_API extern String FileDialog(cwString FilePath, cwString Filter, const Bool& IsOpenFile) noexcept;
	__inline String OpenFileDialog(cwString FilePath, cwString Filter) noexcept {
		return FileDialog(FilePath, Filter, true);
	}
	__inline String SaveAsFileDialog(cwString FilePath, cwString Filter) noexcept {
		return FileDialog(FilePath, Filter, false);
	}
}