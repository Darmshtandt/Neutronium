#pragma once

#include <Nt/Core/Input.h>
#include <Nt/Core/Math/Rect.h>
#include <Nt/Core/Log.h>

#include <Nt/Graphics/System/Menu.h>
#include <Nt/Graphics/System/WinAPI.h>

#ifdef _WINDEF_
#	include <shobjidl.h>
#endif

namespace Nt {
	class EventBus;

	inline _CONSTEXPR20 Int DEFAULT_VALUE = ((Int)0x80000000);

	NT_API Int2D GetCursorPosition() noexcept;
	NT_API Int2D GetMonitorSize() noexcept;

	NT_API IntRect AdjustWindowRect(const IntRect& windowRect, const DWord& styles, const Bool& isHasMenu) noexcept;
	NT_API IntRect AdjustWindowRectEx(const IntRect& windowRect, const DWord& styles, const DWord& exStyles, const Bool& isHasMenu) noexcept;

#ifdef _WINDEF_
	NT_API IntRect GetClientRect(HWND hwnd) noexcept;
	NT_API IntRect GetWindowRect(HWND hwnd) noexcept;

	NT_API IntRect AdjustedWindowRect(HWND hwnd, const DWord& styles, const Bool& isHasMenu) noexcept;
	NT_API IntRect AdjustedWindowRectEx(HWND hwnd, const DWord& styles, const DWord& exStyles, const Bool& isHasMenu) noexcept;

	NT_API COLORREF VectorToColorRef(const Byte3D& color);
	NT_API Byte3D ColorRefToVector(const COLORREF& color);

	NT_API HBRUSH CreateSolidBrush(const Byte3D& color) noexcept;

	NT_API void DrawFrameRect(HDC hdc, const IntRect& rect, const Int& lineWeight, const Byte3D& color);

	using HandleWindowID = HWND;
#else
	using HandleWindowID = void*;
#endif

	class HandleWindow {
	public:
		struct OtherEvent final {
			uInt uMsg = static_cast<uInt>(-1);
			uInt wParam = static_cast<uInt>(-1);
			Long lParam = -1;
			Int HandleID = 0;
		};
		struct ExceptionEvent {
			std::string Text;
			Int HandleID = 0;
		};

		struct KillFocusEvent final
		{			
		};

		struct KeyDownEvent final {
			Int2D Position;
			Key Key;
			Int HandleID = 0;
		};
		struct KeyUpEvent final {
			Int2D Position;
			Key Key;
			Int HandleID = 0;
		};

		struct CommandEvent final {
			uInt wParam = static_cast<uInt>(-1);
			Long lParam = -1;
			Int HandleID = 0;
		};

		enum ZOrder {
			ZORDER_NONE = 0,
			ZORDER_NOTOPMOST = -2,
			ZORDER_TOPMOST = -1,
			ZORDER_TOP = 0,
			ZORDER_BOTTOM = 1,
		};
		enum WindowPositionFlags {
			POSITIONFLAG_NONE = 0x0000,
			POSITIONFLAG_ASYNC = 0x4000,
			POSITIONFLAG_DEFERERASE = 0x2000,
			POSITIONFLAG_DRAWFRAME = 0x0020,
			POSITIONFLAG_FRAMECHANGED = POSITIONFLAG_DRAWFRAME,
			POSITIONFLAG_HIDEWINDOW = 0x0080,
			POSITIONFLAG_NOACTIVATE = 0x0010,
			POSITIONFLAG_NOCOPYBITS = 0x0100,
			POSITIONFLAG_NOMOVE = 0x0002,
			POSITIONFLAG_NOOWNERZORDER = 0x0200,
			POSITIONFLAG_NOREDRAW = 0x0008,
			POSITIONFLAG_NOREPOSITION = 0x0200,
			POSITIONFLAG_NOSENDCHANGING = 0x0400,
			POSITIONFLAG_NOSIZE = 0x0001,
			POSITIONFLAG_NOZORDER = 0x0004,
			POSITIONFLAG_SHOWWINDOW = 0x0040
		};
		enum Index {
			INDEX_EXSTYLE = (-20),
			INDEX_HINSTANCE = (-6),
			INDEX_HWNDPARENT = (-8),
			INDEX_ID = (-12),
			INDEX_STYLE = (-16),
			INDEX_USERDATA = (-21),
			INDEX_WNDPROC = (-4),
		};
		enum Styles : uLong {
			STYLE_NONE = 0x00000000L,
			
			STYLE_BORDER = 0x00800000L,
			STYLE_CHILD = 0x40000000L,
			STYLE_CLIPCHILDREN = 0x02000000L,
			STYLE_CLIPSIBLINGS = 0x04000000L,
			STYLE_DISABLED = 0x08000000L,
			STYLE_DLGFRAME = 0x00400000L,
			STYLE_CAPTION = STYLE_DLGFRAME | STYLE_BORDER,
			STYLE_GROUP = 0x00020000L,
			STYLE_HSCROLL = 0x00100000L,
			STYLE_ICONIC = 0x20000000L,
			STYLE_MAXIMIZE = 0x01000000L,
			STYLE_MAXIMIZEBOX = 0x00010000L,
			STYLE_MINIMIZE = 0x20000000L,
			STYLE_MINIMIZEBOX = 0x00020000L,
			STYLE_OVERLAPPED = 0x00000000L,
			STYLE_POPUP = 0x80000000L,
			STYLE_SYSMENU = 0x00080000L,
			STYLE_TABSTOP = 0x00010000L,
			STYLE_THICKFRAME = 0x00040000L,
			STYLE_VISIBLE = 0x10000000L,
			STYLE_VSCROLL = 0x00200000L,

			STYLE_CHILDWINDOW = STYLE_CHILD,
			STYLE_SIZEBOX = STYLE_THICKFRAME,
			STYLE_TILED = STYLE_OVERLAPPED,
			STYLE_POPUPWINDOW = STYLE_POPUP | STYLE_BORDER | STYLE_SYSMENU,
			STYLE_OVERLAPPEDWINDOW = STYLE_OVERLAPPED | STYLE_CAPTION | STYLE_SYSMENU | STYLE_THICKFRAME | STYLE_MINIMIZEBOX | STYLE_MAXIMIZEBOX,
			STYLE_TILEDWINDOW = STYLE_OVERLAPPEDWINDOW,
		};

	public:
		NT_API HandleWindow() noexcept;
		NT_API HandleWindow(HandleWindow&& window);
		NT_API HandleWindow(const IntRect& rect, const String& name);
		virtual ~HandleWindow() noexcept = default;
#ifdef _WINDEF_
		NT_API HandleWindow(const HWND& hwnd);
#endif

		NT_API virtual void Create(const IntRect& windowRect, const String& name);

		NT_API void Hide();
		NT_API void Show();
		NT_API void ShowMaximized();

		NT_API Bool EnableWindow() noexcept;
		NT_API Bool DisableWindow() noexcept;

		NT_API Int2D ScreenToClient(const Int2D& cursorPosition) const noexcept;

		NT_API virtual Long AddStyles(const DWord& styles) noexcept;
		NT_API virtual Long RemoveStyles(const DWord& styles) noexcept;
		NT_API virtual Long AddExStyles(const DWord& styles) noexcept;
		NT_API virtual Long RemoveExStyles(const DWord& styles) noexcept;

		NT_API void EnableMenu() noexcept;
		NT_API void DisableMenu() noexcept;

		NT_API void Destroy() noexcept;

		NT_API Bool InvalidateRect(const IntRect* pRect, const Bool& isErased) noexcept;

		NT_API HandleWindow& operator = (HandleWindow&& window);

		NT_API virtual void SetBackgroundColor(const Byte3D& color);
		NT_API void SetIcon(const String& iconPath);
		NT_API void SetIconSmall(const String& iconPath);

#ifdef _WINDEF_
		NT_API void SetIcon(const HICON& hIcon);
		NT_API void SetIconSmall(const HICON& hIcon);

		NT_API void SetInstance(const HINSTANCE& hInstance) noexcept;
		NT_API virtual void SetParentHandle(const HWND& hParent);
#endif
		NT_API Long SetWindowInfo(const Index& index, const Long& data) noexcept;
		NT_API Bool SetMenu(Menu* pMenu) noexcept;
		NT_API Long SetID(const Int& newID) noexcept;
		NT_API virtual void SetParent(const HandleWindow& parentWindow);
		NT_API Bool SetWindowPos(const IntRect& rect, const WindowPositionFlags& flags) noexcept;
		NT_API Bool SetWindowPos(const ZOrder& order, const IntRect& rect, const WindowPositionFlags& flags) noexcept;
		NT_API void SetPosition(const Int2D& newPosition) noexcept;
		NT_API void SetSize(const Int2D& newSize) noexcept;
		NT_API void SetWindowRect(const IntRect& newRect) noexcept;
		NT_API void SetName(const String& name);
		NT_API uInt SetStyles(const uInt& styles);
		NT_API uInt SetExStyles(const uInt& exStyles);
		NT_API void SetEventBus(const std::weak_ptr<EventBus>& bus) noexcept;

#ifdef _WINDEF_
		NT_API HWND GetParentHandle() const noexcept;
		NT_API HWND GetHandle() const noexcept;
		NT_API HDC GetDC() const noexcept;
		NT_API HDC GetWindowDC() const noexcept;
		NT_API HINSTANCE GetInstance() const noexcept;
#endif
		NT_NODISCARD NT_API std::weak_ptr<EventBus> GetEventBus() noexcept;
		NT_NODISCARD NT_API Long GetWindowInfo(const Index& index) const noexcept;
		NT_NODISCARD NT_API String GetName() const;
		NT_NODISCARD NT_API Menu* GetMenu() noexcept;
		NT_NODISCARD NT_API IntRect GetClientRect() const noexcept;
		NT_NODISCARD NT_API IntRect GetWindowRect() const noexcept;
		NT_NODISCARD NT_API HandleWindow GetParent() const noexcept;
		NT_NODISCARD NT_API Int GetID() const noexcept;
		NT_NODISCARD NT_API void* GetParamPtr() const noexcept;
		NT_NODISCARD NT_API Byte3D GetBackgroundColor() const noexcept;
		NT_NODISCARD NT_API uInt GetStyles() const noexcept;
		NT_NODISCARD NT_API uInt GetExStyles() const noexcept;
		NT_NODISCARD NT_API Bool IsMenuEnabled() const noexcept;
		NT_NODISCARD NT_API Bool IsShowed() const noexcept;
		NT_NODISCARD NT_API Bool IsCreated() const noexcept;

	protected:
		std::weak_ptr<EventBus> m_EventBus;
		std::wstring m_Name;
		std::wstring m_ClassName;
		Byte3D m_BackgroundColor;

		HINSTANCE m_hInstance;
		HWND m_hParent = nullptr;
		HWND m_hwnd = nullptr;
		HDC m_hdc = nullptr;

		Menu* m_pMenu = nullptr;
		void* m_pParam = nullptr;
		IntRect m_ClientRect;
		IntRect m_WindowRect;
		Int m_ID = 0;
		ZOrder m_ZOrder = ZORDER_NOTOPMOST;
		uInt m_Styles = STYLE_NONE;
		uInt m_ExStyles = STYLE_NONE;
		Bool m_IsMenuEnabled = false;
		Bool m_IsWindowEnabled = true;

	protected:
		NT_API void _CreateWindow();
		NT_NODISCARD NT_API IntRect _ComputeRealWindowRect();
		NT_NODISCARD NT_API IntRect _ComputeRealClientRect();
		NT_API Long _SendMessage(const uInt& message, const uInt& wParam, const Long& lParam) const;

		NT_API void EmmitExceptionEvent(std::string text) const;
		NT_API void EventBusProc(uInt uMsg, uInt wParam, Long lParam) const;

		NT_API static HandleWindow* GetHandleFromUserData(HWND hwnd, const uInt& uMsg, LPARAM lParam);
		NT_API static LRESULT CALLBACK SubClassProc(HWND hwnd, uInt uMsg, uInt wParam, Long lParam, uInt uIdSubclass, uLong dwRefData);
	};

#ifdef _WINDEF_
	NT_API String OpenFileDialog(const String& startPath, const _FILEOPENDIALOGOPTIONS& options);
#endif
}