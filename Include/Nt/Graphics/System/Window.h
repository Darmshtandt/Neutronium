#pragma once

#include <functional>
#include <queue>
#include <map>

#include <Nt/Graphics/System/HandleWindow.h>
#include <Nt/Graphics/Listeners/WindowListeners.h>

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

		enum Modifer {
			NONE = 0x0000,
			ALT = 0x0001,
			CONTROL = 0x0002,
			SHIFT = 0x0004,
			WIN = 0x0008,
			NOREPEAT = 0x4000,
		};

	public:
		NT_API Window() noexcept;
		NT_API Window(const IntRect& rect, const String& name);
		NT_API Window(const Int2D& size, const String& name);
		NT_API explicit Window(const String& name);
		NT_API ~Window() override;

		NT_API virtual void Create(const String& name);
		NT_API virtual void Create(const Int2D& size, const String& name);
		NT_API virtual void Create(const IntRect& windowRect, const String& name);

		NT_API void RegisterHotKey(const uInt& modifiers, const Key& key, const std::function<void()>& action);
		NT_API void RegisterWindowListener(const NotNull<WindowListener*>& listener);
		NT_API void RegisterKeyboardListener(const NotNull<KeyboardListener*>& listener);
		NT_API void RegisterMouseListener(const NotNull<MouseListener*>& listener);

		NT_API void UnregisterWindowListener(const NotNull<WindowListener*>& listener);
		NT_API void UnregisterKeyboardListener(const NotNull<KeyboardListener*>& listener);
		NT_API void UnregisterMouseListener(const NotNull<MouseListener*>& listener);

		NT_API void PeekMessages();
		NT_API Bool PeekMessages(Event* pEvent);

		NT_API void Close() noexcept;
		NT_API void Destroy() noexcept;

		NT_API void EnableSystemBorder() noexcept;
		NT_API void DisableSystemBorder() noexcept;

		NT_NODISCARD NT_API Bool IsOpened() const noexcept;
		NT_NODISCARD NT_API Int2D GetPosition() const noexcept;
		NT_NODISCARD NT_API Int2D GetSize() const noexcept;
		NT_NODISCARD NT_API Int2D GetClientSize() const noexcept;
		NT_NODISCARD NT_API Byte3D GetBorderColor() const noexcept;
		NT_NODISCARD NT_API IntRect GetClientRect() const noexcept;

		NT_API void SetProcedure(Procedure procedure) noexcept;
		NT_API void SetBorderColor(const Byte3D& color) noexcept;

	private:
		std::list<WindowListener*> m_WindowListeners;
		std::list<KeyboardListener*> m_KeyboardListeners;
		std::list<MouseListener*> m_MouseListeners;
		std::map<uInt, std::function<void()>> m_HotKeyMap;
		std::queue<Event> m_Events;
		Procedure m_Procedure;
		Byte3D m_BorderColor = { 255, 255, 255 };
		MSG m_Msg;
		HMENU m_hMenu;
		Bool m_EnabledSystemBorder = false;
		Bool m_IsOpened = false;

	private:
		NT_API void _AddEvent(const Event::Types& type, const DWord& value);
		NT_NODISCARD NT_API uInt _GenerateID() const noexcept;

	protected:
		NT_API virtual void _Creation([[maybe_unused]] const CREATESTRUCT* pWindowStruct);
		NT_API virtual void _Resize([[maybe_unused]] const uInt2D& windowSize);
		NT_API virtual Bool _NoClientPaint(HRGN hRegion);
		NT_API virtual void _Paint([[maybe_unused]] HDC& hdc, [[maybe_unused]] PAINTSTRUCT& paint);
		NT_API virtual void _Command([[maybe_unused]] const Long& param_1, [[maybe_unused]] const Long& param_2);

	private:
		[[deprecated]] NT_API static Window* _GetHandleFromUserData(HWND hwnd, const uInt& uMsg, LPARAM lParam);
		NT_API void _VerticalScroll(HWND hwnd, WPARAM wParam);

		NT_API static LRESULT CALLBACK _BaseWndProc(HWND hwnd, uInt uMsg, WPARAM wParam, LPARAM lParam);
	};

	NT_API extern String FileDialog(cwString FilePath, cwString Filter, const Bool& IsOpenFile);
	NT_API String OpenFileDialog(cwString FilePath, cwString Filter) noexcept;
	NT_API String SaveAsFileDialog(cwString FilePath, cwString Filter) noexcept;
}