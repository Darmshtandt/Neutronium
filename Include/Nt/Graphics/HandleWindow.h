#pragma once

namespace Nt {
	inline constexpr Int DEFAULT_VALUE = CW_USEDEFAULT;

	NT_API Int2D GetCursorPosition() noexcept;
	NT_API Int2D GetMonitorSize() noexcept;

	NT_API IntRect GetClientRect(HWND hwnd) noexcept;
	NT_API IntRect GetWindowRect(HWND hwnd) noexcept;

	NT_API IntRect AdjustWindowRect(const IntRect& windowRect, const DWord& styles, const Bool& isHasMenu) noexcept;
	NT_API IntRect AdjustWindowRectEx(const IntRect& windowRect, const DWord& styles, const DWord& exStyles, const Bool& isHasMenu) noexcept;

	__inline IntRect AdjustedWindowRect(HWND hwnd, const DWord& styles, const Bool& isHasMenu) noexcept {
		return AdjustWindowRect(GetWindowRect(hwnd), styles, isHasMenu);
	}
	__inline IntRect AdjustedWindowRectEx(HWND hwnd, const DWord& styles, const DWord& exStyles, const Bool& isHasMenu) noexcept {
		return AdjustWindowRectEx(GetWindowRect(hwnd), styles, isHasMenu, exStyles);
	}

	NT_API COLORREF VectorToColorRef(const Byte3D& color);
	NT_API Byte3D ColorRefToVector(const COLORREF& color);

	NT_API HBRUSH CreateSolidBrush(const Byte3D& color) noexcept;

	__inline void DrawFrameRect(HDC hdc, const IntRect& rect, const Int& lineWeight, const Byte3D color) {
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

	using HandleWindowID = HWND;

	class HandleWindow {
	public:
		enum ZOrder {
			ZORDER_NONE = 0,
			ZORDER_NOTOPMOST = -2,
			ZORDER_TOPMOST = -1,
			ZORDER_TOP = 0,
			ZORDER_BOTTOM = 1,
		};
		enum WindowPositionFlags {
			POSITIONFLAG_NONE = 0,
			POSITIONFLAG_ASYNC = SWP_ASYNCWINDOWPOS,
			POSITIONFLAG_DEFERERASE = SWP_DEFERERASE,
			POSITIONFLAG_DRAWFRAME = SWP_DRAWFRAME,
			POSITIONFLAG_FRAMECHANGED = SWP_FRAMECHANGED,
			POSITIONFLAG_HIDEWINDOW = SWP_HIDEWINDOW,
			POSITIONFLAG_NOACTIVATE = SWP_NOACTIVATE,
			POSITIONFLAG_NOCOPYBITS = SWP_NOCOPYBITS,
			POSITIONFLAG_NOMOVE = SWP_NOMOVE,
			POSITIONFLAG_NOOWNERZORDER = SWP_NOOWNERZORDER,
			POSITIONFLAG_NOREDRAW = SWP_NOREDRAW,
			POSITIONFLAG_NOREPOSITION = SWP_NOREPOSITION,
			POSITIONFLAG_NOSENDCHANGING = SWP_NOSENDCHANGING,
			POSITIONFLAG_NOSIZE = SWP_NOSIZE,
			POSITIONFLAG_NOZORDER = SWP_NOZORDER,
			POSITIONFLAG_SHOWWINDOW = SWP_SHOWWINDOW
		};

	public:
		NT_API HandleWindow() noexcept;
		explicit HandleWindow(const HWND& hwnd) {
			m_hwnd = hwnd;
			if (m_hwnd) {
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
				m_Menu = ::GetMenu(m_hwnd);
				m_ClientRect = Nt::GetClientRect(m_hwnd);
				m_WindowRect = Nt::GetWindowRect(m_hwnd);
				m_ID = GetWindowLongPtr(m_hwnd, GWLP_ID);;
				m_Styles = GetWindowLongPtr(m_hwnd, GWL_STYLE);
				m_ExStyles = GetWindowLongPtr(m_hwnd, GWL_EXSTYLE);
				m_IsMenuEnabled = (m_Menu.GetHandle() != nullptr);
				m_IsWindowEnabled = IsWindowEnabled(m_hwnd);
			}
		}
		HandleWindow(const IntRect& rect, const String& name) {
			Create(rect, name);
		}

		NT_API virtual void Create(const IntRect& windowRect, const String& name);
		NT_API HandleWindow& CreateCopy() const;

		NT_API void Hide();
		NT_API void Show();
		NT_API void ShowMaximized();

		NT_API Bool EnableWindow() noexcept;
		NT_API Bool DisableWindow() noexcept;

		NT_API Int2D ScreenToClient(const Int2D& cursorPosition) const noexcept;

		NT_API void AddStyles(const DWord& styles) noexcept;
		NT_API void RemoveStyles(const DWord& styles) noexcept;
		NT_API void AddExStyles(const DWord& styles) noexcept;
		NT_API void RemoveExStyles(const DWord& styles) noexcept;

		NT_API void EnableMenu() noexcept;
		NT_API void DisableMenu() noexcept;

		NT_API void Destroy() noexcept;

		virtual void SetBackgroundColor(const Byte3D& color) {
			m_BackgroundColor = color;
			if (m_hwnd) {
				const HBRUSH hColor = CreateSolidBrush(m_BackgroundColor);
				SetClassLongPtr(m_hwnd, GCLP_HBRBACKGROUND, (LONG_PTR)hColor);
				InvalidateRect(m_hwnd, nullptr, TRUE);
			}
		}
		void SetIcon(const HICON& hIcon) {
			if (m_hwnd != nullptr)
				SetClassLongPtr(m_hwnd, GCLP_HICON, reinterpret_cast<Long>(hIcon));
		}
		void SetIconSmall(const HICON& hIcon) {
			if (m_hwnd != nullptr)
				SetClassLongPtr(m_hwnd, GCLP_HICONSM, reinterpret_cast<Long>(hIcon));
		}
		NT_API void SetIcon(const String& iconPath);
		NT_API void SetIconSmall(const String& iconPath);

#ifdef _WINDEF_
		NT_API void SetInstance(const HINSTANCE& hInstance) noexcept;
		NT_API virtual void SetParentHandle(const HWND& hParent);
#endif
		NT_API void SetMenu(const Menu& menu) noexcept;
		NT_API void SetID(const Int& newID) noexcept;
		NT_API virtual void SetParent(const HandleWindow& parentWindow);
		NT_API Bool SetWindowPos(const IntRect& rect, const WindowPositionFlags& flags) noexcept;
		NT_API Bool SetWindowPos(const ZOrder& order, const IntRect& rect, const WindowPositionFlags& flags) noexcept;
		NT_API void SetPosition(const Int2D& newPosition) noexcept;
		NT_API void SetSize(const Int2D& newSize) noexcept;
		NT_API void SetWindowRect(const IntRect& newRect) noexcept;
		NT_API void SetName(const String& name);

#ifdef _WINDEF_
		NT_API HWND GetParentHandle() const noexcept;
		NT_API HWND GetHandle() const noexcept;
		NT_API HDC GetDC() const noexcept;
		__inline HDC GetWindowDC() const noexcept {
			return ::GetWindowDC(m_hwnd);
		}
		NT_API HINSTANCE GetInstance() const noexcept;
#endif
		NT_API Nt::String GetName() const;
		NT_API Menu GetMenu() const noexcept;
		NT_API IntRect GetClientRect() const noexcept;
		NT_API IntRect GetWindowRect() const noexcept;
		NT_API IntRect GetAdjustedWindowRect() const noexcept;
		NT_API HandleWindow GetParent() const noexcept;
		NT_API Int GetID() const noexcept;
		NT_API void* GetParamPtr() const noexcept;
		NT_API Byte3D GetBackgroundColor() const noexcept;
		__inline uInt GetStyles() const noexcept {
			return m_Styles;
		}
		__inline uInt GetExStyles() const noexcept {
			return m_ExStyles;
		}
		NT_API Bool IsMenuEnabled() const noexcept;
		NT_API Bool IsShowed() const noexcept;
		NT_API Bool IsCreated() const noexcept;

	protected:
		std::wstring m_Name;
		std::wstring m_ClassName;
		Byte3D m_BackgroundColor;
		HINSTANCE m_hInstance;
		HWND m_hParent;
		HWND m_hwnd;
		HDC m_hdc;
		Menu m_Menu;
		void* m_pParam;
		IntRect m_ClientRect;
		IntRect m_WindowRect;
		Int m_ID;
		ZOrder m_ZOrder;
		uInt m_Styles;
		uInt m_ExStyles;
		Bool m_IsMenuEnabled;
		Bool m_IsWindowEnabled;

	protected:
		NT_API void _CreateWindow();
		NT_API Long _SendMessage(const uInt& message, const uInt& wParam, const Long& lParam) const;
	};

	NT_API String OpenFileDialog(const String& startPath, const _FILEOPENDIALOGOPTIONS& options);
}