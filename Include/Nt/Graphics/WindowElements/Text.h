#pragma once

namespace Nt {
	class Text {
	public:
		enum Format {
			FORMAT_TOP = DT_TOP,
			FORMAT_LEFT = DT_LEFT,
			FORMAT_CENTER = DT_CENTER,
			FORMAT_RIGHT = DT_RIGHT,
			FORMAT_VCENTER = DT_VCENTER,
			FORMAT_BOTTOM = DT_BOTTOM,
			FORMAT_WORDBREAK = DT_WORDBREAK,
			FORMAT_SINGLELINE = DT_SINGLELINE,
			FORMAT_EXPANDTABS = DT_EXPANDTABS,
			FORMAT_TABSTOP = DT_TABSTOP,
			FORMAT_NOCLIP = DT_NOCLIP,
			FORMAT_EXTERNALLEADING = DT_EXTERNALLEADING,
			FORMAT_CALCRECT = DT_CALCRECT,
			FORMAT_NOPREFIX = DT_NOPREFIX,
			FORMAT_INTERNAL = DT_INTERNAL,
			FORMAT_EDITCONTROL = DT_EDITCONTROL,
			FORMAT_PATH_ELLIPSIS = DT_PATH_ELLIPSIS,
			FORMAT_END_ELLIPSIS = DT_END_ELLIPSIS,
			FORMAT_MODIFYSTRING = DT_MODIFYSTRING,
			FORMAT_RTLREADING = DT_RTLREADING,
			FORMAT_WORD_ELLIPSIS = DT_WORD_ELLIPSIS,
			FORMAT_NOFULLWIDTHCHARBREAK = DT_NOFULLWIDTHCHARBREAK,
			FORMAT_HIDEPREFIX = DT_HIDEPREFIX,
			FORMAT_PREFIXONLY = DT_PREFIXONLY
		};

	public:
		Text() noexcept :
			m_BackgroundColor(RGB(255, 255, 255)),
			m_Format(0),
			m_Color(0),
			m_Weight(400),
			m_FontSize({ 6, 16 }),
			m_IsEnabledBackground(false),
			m_IsEnabledDefaultRectSize(true)
		{
		}
		Text(const Int2D& position, const String& text) noexcept :
			m_Rect(position, Int2D()),
			m_Text(text),
			m_Format(0),
			m_BackgroundColor(RGB(255, 255, 255)),
			m_Color(0),
			m_Weight(400),
			m_FontSize({ 6, 16 }),
			m_IsEnabledBackground(false),
			m_IsEnabledDefaultRectSize(true)
		{
		}
		Text(const IntRect& rect, const String& text) noexcept :
			m_Rect(rect),
			m_Text(text),
			m_Format(0),
			m_BackgroundColor(RGB(255, 255, 255)),
			m_Color(0),
			m_Weight(400),
			m_FontSize({ 6, 16 }),
			m_IsEnabledBackground(false),
			m_IsEnabledDefaultRectSize(true)
		{
		}

		void Draw(const HandleWindow& window) {
			Draw(window.GetDC());
		}
		void Draw(const HDC& hdc) {
			if (m_IsEnabledBackground)
				SetBkMode(hdc, OPAQUE);
			else
				SetBkMode(hdc, TRANSPARENT);

			const COLORREF prevBackgroundColor = ::GetBkColor(hdc);
			if (m_IsEnabledBackground && prevBackgroundColor != m_BackgroundColor)
				::SetBkColor(hdc, m_BackgroundColor);

			const COLORREF prevTextColor = ::GetTextColor(hdc);
			if (prevTextColor != m_Color)
				::SetTextColor(hdc, m_Color);

			const HFONT hFont = CreateFont(
				m_FontSize.y, m_FontSize.x,
				0, 0, m_Weight,
				FALSE, FALSE, FALSE,
				DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
				CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY,
				DEFAULT_PITCH | FF_DONTCARE,
				L"Areal");
			SelectObject(hdc, hFont);

			RECT rect = m_Rect;
			if (m_IsEnabledDefaultRectSize) {
				SIZE textRectSize;
				GetTextExtentPoint32(hdc, m_Text.c_str(), m_Text.length(), &textRectSize);

				rect.right = rect.left + textRectSize.cx;
				rect.bottom = rect.top + textRectSize.cy;
			}
			else {
				rect.right += rect.left;
				rect.bottom += rect.top;
			}

			if (!DrawText(hdc, m_Text.c_str(), m_Text.size(), &rect, m_Format))
				Raise("Failed to draw text");

			DeleteObject(hFont);
			if (prevTextColor != m_Color)
				::SetTextColor(hdc, prevTextColor);

			if (m_IsEnabledBackground && prevBackgroundColor != m_BackgroundColor)
				::SetBkColor(hdc, prevBackgroundColor);
		}

		void AddFormat(const uInt& format) noexcept {
			m_Format |= format;
		}
		void RemoveFormat(const uInt& format) noexcept {
			m_Format &= ~format;
		}

		void EnableBackground() noexcept {
			m_IsEnabledBackground = true;
		}
		void DisableBackground() noexcept {
			m_IsEnabledBackground = false;
		}
		void EnableDefaultRectSize() noexcept {
			m_IsEnabledDefaultRectSize = true;
		}
		void DisableDefaultRectSize() noexcept {
			m_IsEnabledDefaultRectSize = false;
		}

		uInt GetFormat() const noexcept {
			return m_Format;
		}
		String GetText() const noexcept {
			return m_Text;
		}
		IntRect GetRect() const noexcept {
			return m_Rect;
		}
		uInt2D GetFontSize() const noexcept {
			return m_FontSize;
		}
		uInt3D GetBackgroundColor() const noexcept {
			return ColorRefToVector(m_BackgroundColor);
		}
		uInt3D GetTextColor() const noexcept {
			return ColorRefToVector(m_Color);
		}
		Bool IsEnabledBackground() const noexcept {
			return m_IsEnabledBackground;
		}
		Bool IsEnabledDefaultRectSize() const noexcept {
			return m_IsEnabledDefaultRectSize;
		}

		void SetFormat(const uInt& format) noexcept {
			m_Format = format;
		}
		void SetText(const String& text) noexcept {
			m_Text = text;
		}
		void SetRect(const IntRect& rect) noexcept {
			m_Rect = rect;
		}
		void SetPosition(const Int2D& position) noexcept {
			m_Rect.LeftTop = position;
		}
		void SetSize(const Int2D& size) noexcept {
			m_Rect.RightBottom = size;
		}
		void SetBackgroundColor(const Byte3D& color) noexcept {
			m_BackgroundColor = VectorToColorRef(color);
		}
		void SetColor(const Byte3D& color) noexcept {
			m_Color = VectorToColorRef(color);
		}
		void SetWeight(const uInt& weight) noexcept {
			m_Weight = weight;
		}
		void SetFontSize(const uInt2D& size) noexcept {
			m_FontSize = size;
		}

	private:
		std::wstring m_Text;
		IntRect m_Rect;
		uInt m_Format;
		COLORREF m_BackgroundColor;
		COLORREF m_Color;
		uInt2D m_FontSize;
		uInt m_Weight;
		Bool m_IsEnabledBackground;
		Bool m_IsEnabledDefaultRectSize;
	};
}