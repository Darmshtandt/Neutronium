#pragma once

#include <functional>

#include <Nt/Graphics/System/HandleWindow.h>
#include <Nt/Graphics/System/GDI/GDI.h>
#include <Nt/Graphics/System/GDI/Bitmap.h>

namespace Nt {
	class Button : public HandleWindow {
	public:
		static const Int2D DefaultSize;

		enum class Style {
			CHECKBOX = 0x00000002L,
			AUTOCHECKBOX = 0x00000003L,
			RADIOBUTTON = 0x00000004L,
			_3STATE = 0x00000005L,
			AUTO3STATE = 0x00000006L,
			AUTORADIOBUTTON = 0x00000009L,

			ICON = 0x00000040L,
			BITMAP = 0x00000080L,

			LEFT = 0x00000100L,
			RIGHT = 0x00000200L,
			CENTER = 0x00000300L,
			TOP = 0x00000400L,
			BOTTOM = 0x00000800L,
			VCENTER = 0x00000C00L,

			TEXT = 0x00000000L,
			LEFTTEXT = 0x00000020L,
			MULTILINE = 0x00002000L,

			PUSHBUTTON = 0x00000000L,
			DEFPUSHBUTTON = 0x00000001L,
			GROUPBOX = 0x00000007L,
			USERBUTTON = 0x00000008L,
			OWNERDRAW = 0x0000000BL,
			SPLITBUTTON = 0x0000000CL,
			DEFSPLITBUTTON = 0x0000000DL,
			COMMANDLINK = 0x0000000EL,
			DEFCOMMANDLINK = 0x0000000FL,
			TYPEMASK = 0x0000000FL,
			PUSHLIKE = 0x00001000L,
			NOTIFY = 0x00004000L,
			FLAT = 0x00008000L,
		};

	public:
		Button() noexcept = default;
		Button(Button&& other) noexcept = default;
		NT_API Button(HandleWindow& parent, const IntRect& rect, const uInt& id, const String& text);
		~Button() noexcept override = default;

		NT_API void Create(const String& text);
		NT_API void Create(const IntRect& rect, const String& text) override;

		NT_API void OnClick();

		NT_API void SetOnClick(std::function<void()> onClick);

		NT_API void SetImage(const GDI::Bitmap& image);
		NT_API void RemoveImage();

		NT_API void SetCheck(const Bool& isChecked) noexcept;

		NT_API Bool IsChecked() const noexcept;

	private:
		std::function<void()> m_OnClick;

	private:
#ifdef _WINDEF_
		static NT_API LRESULT CALLBACK _ButtonProc(HWND hwnd, uInt uMsg, WPARAM wParam,
			LPARAM lParam, [[maybe_unused]] UINT_PTR uIdSubclass, [[maybe_unused]] DWORD_PTR dwRefData);
#endif
	};
}