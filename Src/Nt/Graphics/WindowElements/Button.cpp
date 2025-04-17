// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <Nt/Core/WinMinimal.h>

#undef NOUSER
#undef NOWINSTYLES
#undef NOWINOFFSETS
#undef NOGDI
#undef NOMSG

#include <windows.h>
#include <windowsx.h>
#include <shobjidl.h>
#include <CommCtrl.h>

#include <Nt/Graphics/WindowElements/Button.h>

namespace Nt {
	const Int2D Button::DefaultSize(GetSystemMetrics(SM_CXMENUCHECK), GetSystemMetrics(SM_CYMENUCHECK));

	Button::Button(HandleWindow& parent, const IntRect& rect, const uInt& id, const String& text) {
		Button::SetID(id);
		Button::SetParent(parent);
		Button::Create(rect, text);
	}

	void Button::Create(const String& text) {
		if (m_WindowRect.Right == 0)
			m_WindowRect.Right = DefaultSize.x;

		if (m_WindowRect.Bottom == 0)
			m_WindowRect.Bottom = DefaultSize.y;

		Create(m_WindowRect, text);
	}
	void Button::Create(const IntRect& rect, const String& text) {
		m_Name = text;
		m_ClassName = WC_BUTTON;
		m_WindowRect = rect;
		m_Styles |= (BS_PUSHBUTTON | BS_TEXT | WS_TABSTOP);
		m_ExStyles |= WS_EX_CLIENTEDGE;

		_CreateWindow();

		SetWindowLongPtr(m_hwnd, GWLP_USERDATA, reinterpret_cast<Long>(this));
		SetWindowSubclass(m_hwnd, _ButtonProc, 0, 0);
	}

	void Button::OnClick() {
		if (m_OnClick)
			m_OnClick();
	}

	void Button::SetOnClick(std::function<void()> onClick) {
		m_OnClick = onClick;
	}

	void Button::SetImage(const GDI::Bitmap& image) {
		if (m_hwnd != nullptr && image.IsCreated()) {
			Nt::uIntRect imageRect;
			imageRect.RightBottom = image.GetSize();

			Nt::uIntRect scaleRect;
			scaleRect.RightBottom = uInt2D(GetClientRect().RightBottom) - 4;

			if (imageRect.Right > imageRect.Bottom) {
				scaleRect.Top = imageRect.Right - imageRect.Bottom;
				scaleRect.Top /= imageRect.Right / GetClientRect().Right * 2;
				imageRect.Bottom = imageRect.Right;
			}
			else if (imageRect.Right < imageRect.Bottom) {
				scaleRect.Left = imageRect.Bottom - imageRect.Right;
				scaleRect.Left /= imageRect.Bottom / GetClientRect().Bottom * 2;
				imageRect.Right = imageRect.Bottom;
			}

			HBITMAP hBitmap = GDI::ScaleHBitmap(m_hdc, image.GetHandle(),
				imageRect, scaleRect);

			_SendMessage(BM_SETIMAGE, IMAGE_BITMAP, reinterpret_cast<LPARAM>(hBitmap));
			InvalidateRect(nullptr, true);
		}
	}
	void Button::RemoveImage() {
		if (m_hwnd == nullptr)
			return;

		HBITMAP hBitmap = reinterpret_cast<HBITMAP>(_SendMessage(BM_GETIMAGE, IMAGE_BITMAP, 0));
		if (hBitmap != nullptr) {
			_SendMessage(BM_SETIMAGE, IMAGE_BITMAP, 0);
			DeleteObject(hBitmap);
		}
	}

	void Button::SetCheck(const Bool& isChecked) noexcept {
		Button_SetCheck(m_hwnd, isChecked);
	}

	Bool Button::IsChecked() const noexcept {
		return Button_GetCheck(m_hwnd);
	}

	LRESULT CALLBACK Button::_ButtonProc(HWND hwnd, uInt uMsg, WPARAM wParam,
		LPARAM lParam, [[maybe_unused]] UINT_PTR uIdSubclass, [[maybe_unused]] DWORD_PTR dwRefData)
	{
		Button* pThis = reinterpret_cast<Button*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
		if (pThis != nullptr) {
			switch (uMsg) {
			case BM_CLICK:
				if (pThis->m_OnClick)
					pThis->m_OnClick();
				break;
			}
		}
		return DefSubclassProc(hwnd, uMsg, wParam, lParam);
	}
}