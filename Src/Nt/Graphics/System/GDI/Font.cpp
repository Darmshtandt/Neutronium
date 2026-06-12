#include <Nt/Core/WinMinimal.h>

#undef NOUSER
#undef NOGDI
#undef NOMSG

#include <windows.h>

#include <Nt/Graphics/System/GDI/Font.h>

namespace Nt::GDI {
	void Font::SetFamily(const Family& family) noexcept {
		m_Family = family;
	}

	void Font::SetPitch(const Pitch& pitch) noexcept {
		m_Pitch = pitch;
	}

	void Font::SetQuality(const Quality& quality) noexcept {
		m_Quality = quality;
	}

	void Font::SetClipPrecision(const ClipPrecision& clipPrecision) noexcept {
		m_ClipPrecision = clipPrecision;
	}

	void Font::SetOutPrecision(const OutPrecision& outPrecision) noexcept {
		m_OutPrecision = outPrecision;
	}

	void Font::SetCharSet(const CharSet& charSet) noexcept {
		m_CharSet = charSet;
	}

	void Font::SetWeight(const Weight& weight) noexcept {
		m_Weight = weight;
	}

	void Font::SetOrientation(const Int& orientation) noexcept {
		m_Orientation = orientation;
	}

	void Font::SetEscapement(const Int& escapement) noexcept {
		m_Escapement = escapement;
	}

	void Font::SetSize(const uInt2D& size) noexcept {
		m_Size = size;
	}

	Bool Font::IsStrikeOut() const noexcept {
		return m_IsStrikeOut;
	}

	Bool Font::IsUnderline() const noexcept {
		return m_IsUnderline;
	}

	Bool Font::IsItalic() const noexcept {
		return m_IsItalic;
	}

	Font::Family Font::GetFamily() const noexcept {
		return m_Family;
	}

	Font::Pitch Font::GetPitch() const noexcept {
		return m_Pitch;
	}

	Font::Quality Font::GetQuality() const noexcept {
		return m_Quality;
	}

	Font::ClipPrecision Font::GetClipPrecision() const noexcept {
		return m_ClipPrecision;
	}

	Font::OutPrecision Font::GetOutPrecision() const noexcept {
		return m_OutPrecision;
	}

	Font::CharSet Font::GetCharSet() const noexcept {
		return m_CharSet;
	}

	Font::Weight Font::GetWeight() const noexcept {
		return m_Weight;
	}

	Int Font::GetOrientation() const noexcept {
		return m_Orientation;
	}

	Int Font::GetEscapement() const noexcept {
		return m_Escapement;
	}

	Nt::uInt2D Font::GetSize() const noexcept {
		return m_Size;
	}

	cwString Font::GetName() const noexcept {
		return m_Name;
	}

	void Font::DisableStrikeOut() noexcept {
		m_IsStrikeOut = false;
	}

	void Font::DisableUnderline() noexcept {
		m_IsUnderline = false;
	}

	void Font::DisableItalic() noexcept {
		m_IsItalic = false;
	}

	void Font::EnableStrikeOut() noexcept {
		m_IsStrikeOut = true;
	}

	void Font::EnableUnderline() noexcept {
		m_IsUnderline = true;
	}

	void Font::EnableItalic() noexcept {
		m_IsItalic = true;
	}

	void Font::SetFontToWindow(const HandleWindow& window) {
		m_hwnd = window.GetHandle();
		if (m_hFont == nullptr)
			Raise("Font is not created");
		SendMessage(m_hwnd, WM_SETFONT, (WPARAM)m_hFont, MAKELPARAM(TRUE, 0));
	}

	void Font::Delete() {
		if (m_hFont != nullptr) {
			DeleteObject(m_hFont);
			m_hFont = nullptr;
		}
	}

	void Font::Update() {
		LOGFONT logFont = { };
		logFont.lfHeight = m_Size.y;
		logFont.lfWidth = m_Size.x;
		logFont.lfEscapement = m_Escapement;
		logFont.lfOrientation = m_Orientation;
		logFont.lfWeight = m_Weight;
		logFont.lfItalic = m_IsItalic;
		logFont.lfUnderline = m_IsUnderline;
		logFont.lfStrikeOut = m_IsStrikeOut;
		logFont.lfCharSet = m_CharSet;
		logFont.lfOutPrecision = m_OutPrecision;
		logFont.lfClipPrecision = m_ClipPrecision;
		logFont.lfQuality = m_Quality;
		logFont.lfPitchAndFamily = (Byte)m_Pitch | m_Family;

		const uInt nameLength = lstrlenW(m_Name);
		const uInt byteCount = (nameLength < 32) ? nameLength : 32;
		memcpy(logFont.lfFaceName, m_Name, sizeof(wChar) * byteCount);
		if (byteCount == 32)
			logFont.lfFaceName[31] = L'\0';

		Delete();
		m_hFont = CreateFontIndirect(&logFont);
		SendMessage(m_hwnd, WM_SETFONT, (WPARAM)m_hFont, MAKELPARAM(TRUE, 0));
	}

	void Font::Create() {
		if (m_hFont != nullptr) {
			Log::Instance().Warning("font already created");
			return;
		}

		RequireNotNull(m_Name, "Font name pointer is null");

		m_hFont = CreateFont(
			m_Size.y, m_Size.x,
			m_Escapement, m_Orientation, m_Weight,
			m_IsItalic, m_IsUnderline, m_IsStrikeOut,
			m_CharSet, m_OutPrecision, m_ClipPrecision, m_Quality,
			Byte(m_Pitch) | m_Family, m_Name);

		RequireNotNull(m_hFont, "Failed to create font");
	}
}