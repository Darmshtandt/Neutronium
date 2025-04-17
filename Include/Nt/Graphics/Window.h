#pragma once

#include <functional>
#include <queue>

#include <Nt/Graphics/HandleWindow.h>
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

	public:
		NT_API Window() noexcept;
		NT_API Window(const IntRect& rect, const String& name);
		NT_API Window(const Int2D& size, const String& name);
		NT_API Window(const String& name);
		NT_API ~Window();

		NT_API virtual void Create(const String& name);
		NT_API virtual void Create(const Int2D& size, const String& name);
		NT_API virtual void Create(const IntRect& windowRect, const String& name);

		NT_API void RegisterWindowListener(const std::shared_ptr<WindowListener>& listener);
		NT_API void RegisterKeyboardListener(const std::shared_ptr<KeyboardListener>& listener);
		NT_API void RegisterMouseListener(const std::shared_ptr<MouseListener>& listener);

		NT_API void UnregisterWindowListener(const std::shared_ptr<WindowListener>& listener);
		NT_API void UnregisterKeyboardListener(const std::shared_ptr<KeyboardListener>& listener);
		NT_API void UnregisterMouseListener(const std::shared_ptr<MouseListener>& listener);

		NT_API void PeekMessages();
		NT_API Bool PeekMessages(Event* pEvent);

		NT_API void Close() noexcept;
		NT_API void Destroy() noexcept;

		NT_API _NODISCARD Bool IsOpened() const noexcept;
		NT_API _NODISCARD Int2D GetPosition() const noexcept;
		NT_API _NODISCARD Int2D GetSize() const noexcept;
		NT_API _NODISCARD Int2D GetClientSize() const noexcept;
		NT_API _NODISCARD IntRect GetClientRect() const noexcept;

		NT_API void SetProcedure(Procedure procedure) noexcept;

	private:
		std::list<std::shared_ptr<WindowListener>> m_WindowListeners;
		std::list<std::shared_ptr<KeyboardListener>> m_KeyboardListeners;
		std::list<std::shared_ptr<MouseListener>> m_MouseListeners;
		std::queue<Event> m_Events;
		Procedure m_Procedure;
#ifdef _WINDEF_
		MSG m_Msg;
		HMENU m_hMenu;
#endif
		Bool m_IsOpened;

	private:
#ifdef _WINDEF_
		virtual void _Creation([[maybe_unused]] const CREATESTRUCT* pWindowStruct)
		{ 
		}
		virtual void _Resize([[maybe_unused]] const uInt2D& windowSize)
		{ 
		}
		virtual Bool _NoClientPaint([[maybe_unused]] HDC& hdc) {
			return true;
		}
		virtual void _Paint([[maybe_unused]] HDC& hdc, [[maybe_unused]] PAINTSTRUCT& paint)
		{ 
		}
		virtual void _Command([[maybe_unused]] const Long& param_1, [[maybe_unused]] const Long& param_2)
		{ 
		}
#endif

		void _AddEvent(const Event::Types& type, const DWord& value);

#ifdef _WINDEF_
		static NT_API Window* _GetWindowFromUserData(HWND hwnd, const uInt& uMsg, LPARAM lParam);
		NT_API void _VerticalScroll(HWND hwnd, WPARAM wParam);

		static NT_API LRESULT CALLBACK _BaseWndProc(HWND hwnd, uInt uMsg, WPARAM wParam, LPARAM lParam);
#endif
	};

	NT_API extern String FileDialog(cwString FilePath, cwString Filter, const Bool& IsOpenFile);
	NT_API String OpenFileDialog(cwString FilePath, cwString Filter) noexcept;
	NT_API String SaveAsFileDialog(cwString FilePath, cwString Filter) noexcept;
}