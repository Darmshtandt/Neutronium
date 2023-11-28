#pragma once

namespace Nt {
	class Text {
	public:
		Text() noexcept :
			m_BackgroundColor(RGB(255, 255, 255)),
			m_Format(0),
			m_Color(0),
			m_Weight(400),
			m_FontSize({ 6, 16 }),
			m_IsEnabledBackground(false)
		{ 
		}
		Text(const Int2D& position, const String& text) noexcept :
			m_Position(position),
			m_Text(text),
			m_Format(0),
			m_BackgroundColor(RGB(255, 255, 255)),
			m_Color(0),
			m_Weight(400),
			m_FontSize({ 6, 16 }),
			m_IsEnabledBackground(false)
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

			SIZE textRectSize;
			GetTextExtentPoint32(hdc, m_Text.c_str(), m_Text.length(), &textRectSize);
			
			RECT rect;
			rect.left = m_Position.x;
			rect.top = m_Position.y;
			rect.right = rect.left + textRectSize.cx;
			rect.bottom = rect.top + textRectSize.cy;
			const Int result = DrawText(hdc, m_Text.c_str(), m_Text.size(), &rect, m_Format);

			if (result == FALSE)
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

		uInt GetFormat() const noexcept {
			return m_Format;
		}
		String GetText() const noexcept {
			return m_Text;
		}
		Int2D GetPosition() const noexcept {
			return m_Position;
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

		void SetFormat(const uInt& format) noexcept {
			m_Format = format;
		}
		void SetText(const String& text) noexcept {
			m_Text = text;
		}
		void SetPosition(const Int2D& position) noexcept {
			m_Position = position;
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
		Int2D m_Position;
		uInt m_Format;
		COLORREF m_BackgroundColor;
		COLORREF m_Color;
		uInt2D m_FontSize;
		uInt m_Weight;
		Bool m_IsEnabledBackground;
	};
}