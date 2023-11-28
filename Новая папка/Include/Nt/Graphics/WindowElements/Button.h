#pragma once

namespace Nt {
	class Button : public HandleWindow {
	public:
		Button() noexcept = default;
		Button(HandleWindow& parent, const IntRect& rect, const uInt& id, const String& text) {
			SetID(id);
			SetParent(parent);
			Create(rect, text);
		}

		void Create(const IntRect& rect, const String& text) override {
			m_Name = text;
			m_ClassName = WC_BUTTON;
			m_Rect = rect;
			m_Styles |= (BS_PUSHBUTTON | BS_TEXT | WS_TABSTOP);
			m_ExStyles |= WS_EX_CLIENTEDGE;

			_CreateWindow();
			SetWindowLongPtr(m_hwnd, GWLP_USERDATA, reinterpret_cast<Long>(this));
			SetWindowSubclass(m_hwnd, _ButtonProc, 0, 0);
		}

		void SetOnClick(std::function<void()> onClick) {
			m_OnClick = onClick;
		}

		void SetImage(const GDI::Bitmap& image) {
			if (m_hwnd && image.IsCreated()) {
				Nt::uIntRect imageRect;
				imageRect.RightBottom = image.GetSize();

				Nt::uIntRect scaleRect;
				scaleRect.RightBottom = GetRect().RightBottom;
				if (imageRect.Right > imageRect.Bottom) {
					scaleRect.Top = imageRect.Right - imageRect.Bottom;
					scaleRect.Top /= imageRect.Right / GetRect().Right * 2;
					imageRect.Bottom = imageRect.Right;
				}
				else if (imageRect.Right < imageRect.Bottom) {
					scaleRect.Left = imageRect.Bottom - imageRect.Right;
					scaleRect.Left /= imageRect.Bottom / GetRect().Bottom * 2;
					imageRect.Right = imageRect.Bottom;
				}

				HBITMAP hBitmap = GDI::ScaleHBitmap(m_hdc, image.GetHandle(),
					imageRect, scaleRect);

				SendMessage(m_hwnd, BM_SETIMAGE,
					IMAGE_BITMAP, reinterpret_cast<LPARAM>(hBitmap));
				InvalidateRect(m_hwnd, NULL, TRUE);
			}
		}
		void RemoveImage() {
			if (m_hwnd)
				SendMessage(m_hwnd, BM_SETIMAGE, IMAGE_BITMAP, 0);
		}

		void SetCheck(const Bool& isChecked) noexcept {
			Button_SetCheck(m_hwnd, isChecked);
		}

		Bool IsChecked() const noexcept {
			return Button_GetCheck(m_hwnd);
		}

	private:
		std::function<void()> m_OnClick;

	private:
		static LRESULT CALLBACK _ButtonProc(HWND hwnd, uInt uMsg, WPARAM wParam,
			LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData) {
			Button* pThis = reinterpret_cast<Button*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
			if (pThis) {
				switch (uMsg) {
				case WM_COMMAND:
					switch (HIWORD(wParam)) {
					case BN_CLICKED:
						if (pThis->m_OnClick)
							pThis->m_OnClick();
						break;
					}
					break;
				}
			}
			return DefSubclassProc(hwnd, uMsg, wParam, lParam);
		}
	};
}