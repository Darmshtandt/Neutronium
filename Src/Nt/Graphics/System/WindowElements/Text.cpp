#include <Nt/Core/WinMinimal.h>

#undef NOUSER
#undef NOGDI
#undef NODRAWTEXT
#undef NOWINOFFSETS
#undef NOMSG

#include <windows.h>
#include <shobjidl.h>

#include <Nt/Graphics/System/WindowElements/Text.h>

namespace Nt {
	Text::Text(const Int2D& position, const String& text) noexcept :
		m_Rect(position, Int2D()),
		m_Text(text) {
	}
	Text::Text(const IntRect& rect, const String& text) noexcept :
		m_Rect(rect),
		m_Text(text) {
	}

	void Text::Draw(const HandleWindow& window) {
		Draw(window.GetDC());
	}

	void Text::Draw(const HDC& hdc) {
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

	void Text::AddFormat(const uInt& format) noexcept {
		m_Format |= format;
	}

	void Text::RemoveFormat(const uInt& format) noexcept {
		m_Format &= ~format;
	}

	void Text::EnableBackground() noexcept {
		m_IsEnabledBackground = true;
	}

	void Text::DisableBackground() noexcept {
		m_IsEnabledBackground = false;
	}

	void Text::EnableDefaultRectSize() noexcept {
		m_IsEnabledDefaultRectSize = true;
	}

	void Text::DisableDefaultRectSize() noexcept {
		m_IsEnabledDefaultRectSize = false;
	}

	uInt Text::GetFormat() const noexcept {
		return m_Format;
	}

	Nt::String Text::GetText() const noexcept {
		return m_Text;
	}

	Nt::IntRect Text::GetRect() const noexcept {
		return m_Rect;
	}

	Nt::uInt2D Text::GetFontSize() const noexcept {
		return m_FontSize;
	}

	Nt::uInt3D Text::GetBackgroundColor() const noexcept {
		return ColorRefToVector(m_BackgroundColor);
	}

	Nt::uInt3D Text::GetTextColor() const noexcept {
		return ColorRefToVector(m_Color);
	}

	Bool Text::IsEnabledBackground() const noexcept {
		return m_IsEnabledBackground;
	}

	Bool Text::IsEnabledDefaultRectSize() const noexcept {
		return m_IsEnabledDefaultRectSize;
	}

	void Text::SetFormat(const uInt& format) noexcept {
		m_Format = format;
	}

	void Text::SetText(const String& text) noexcept {
		m_Text = text;
	}

	void Text::SetRect(const IntRect& rect) noexcept {
		m_Rect = rect;
	}

	void Text::SetPosition(const Int2D& position) noexcept {
		m_Rect.LeftTop = position;
	}

	void Text::SetSize(const Int2D& size) noexcept {
		m_Rect.RightBottom = size;
	}

	void Text::SetBackgroundColor(const Byte3D& color) noexcept {
		m_BackgroundColor = VectorToColorRef(color);
	}

	void Text::SetColor(const Byte3D& color) noexcept {
		m_Color = VectorToColorRef(color);
	}

	void Text::SetWeight(const uInt& weight) noexcept {
		m_Weight = weight;
	}

	void Text::SetFontSize(const uInt2D& size) noexcept {
		m_FontSize = size;
	}

}