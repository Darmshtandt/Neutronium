#pragma once

namespace Nt::GDI {
	class Font {
	public:
		enum Weight {
			WEIGHT_DONTCARE = 0,
			WEIGHT_THIN = 100,
			WEIGHT_EXTRALIGHT = 200,
			WEIGHT_LIGHT = 300,
			WEIGHT_NORMAL = 400,
			WEIGHT_MEDIUM = 500,
			WEIGHT_SEMIBOLD = 600,
			WEIGHT_BOLD = 700,
			WEIGHT_EXTRABOLD = 800,
			WEIGHT_HEAVY = 900,
		};
		enum CharSet : Byte {
			CHARSET_ANSI = ANSI_CHARSET,
			CHARSET_BALTIC = BALTIC_CHARSET,
			CHARSET_CHINESEBIG5 = CHINESEBIG5_CHARSET,
			CHARSET_DEFAULT = DEFAULT_CHARSET,
			CHARSET_EASTEUROPE = EASTEUROPE_CHARSET,
			CHARSET_GB2312 = GB2312_CHARSET,
			CHARSET_GREEK = GREEK_CHARSET,
			CHARSET_HANGUL = HANGUL_CHARSET,
			CHARSET_MAC = MAC_CHARSET,
			CHARSET_OEM = OEM_CHARSET,
			CHARSET_RUSSIAN = RUSSIAN_CHARSET,
			CHARSET_SHIFTJIS = SHIFTJIS_CHARSET,
			CHARSET_SYMBOL = SYMBOL_CHARSET,
			CHARSET_TURKISH = TURKISH_CHARSET,
			CHARSET_VIETNAMESE = VIETNAMESE_CHARSET
		};
		enum OutPrecision : Byte {
			OUTPRECISION_CHARACTER = OUT_CHARACTER_PRECIS,
			OUTPRECISION_DEFAULT = OUT_DEFAULT_PRECIS,
			OUTPRECISION_DEVICE = OUT_DEVICE_PRECIS,
			OUTPRECISION_OUTLINE = OUT_OUTLINE_PRECIS,
			OUTPRECISION_PS_ONLY = OUT_PS_ONLY_PRECIS,
			OUTPRECISION_RASTER = OUT_RASTER_PRECIS,
			OUTPRECISION_STRING = OUT_STRING_PRECIS,
			OUTPRECISION_STROKE = OUT_STROKE_PRECIS,
			OUTPRECISION_TT_ONLY = OUT_TT_ONLY_PRECIS,
			OUTPRECISION_TT = OUT_TT_PRECIS,
		};
		enum ClipPrecision : Byte {
			CLIPPRECISION_CHARACTER = CLIP_CHARACTER_PRECIS,
			CLIPPRECISION_DEFAULT = CLIP_DEFAULT_PRECIS,
			CLIPPRECISION_DFA_DISABLE = CLIP_DFA_DISABLE,
			CLIPPRECISION_EMBEDDED = CLIP_EMBEDDED,
			CLIPPRECISION_LH_ANGLES = CLIP_LH_ANGLES,
			CLIPPRECISION_MASK = CLIP_MASK,
			CLIPPRECISION_STROKE = CLIP_STROKE_PRECIS,
			CLIPPRECISION_TT_ALWAYS = CLIP_TT_ALWAYS,
		};
		enum Quality : Byte {
			QUALITY_ANTIALIASED = ANTIALIASED_QUALITY,
			QUALITY_CLEARTYPE = CLEARTYPE_QUALITY,
			QUALITY_DEFAULT = DEFAULT_QUALITY,
			QUALITY_DRAFT = DRAFT_QUALITY,
			QUALITY_NONANTIALIASED = NONANTIALIASED_QUALITY,
			QUALITY_PROOF = PROOF_QUALITY,
		};
		enum Pitch : Byte {
			PITCH_DEFAULT,
			PITCH_FIXED,
			PITCH_VARIABLE,
		};
		enum Family : Byte {
			FAMILY_DECORATIVE = FF_DECORATIVE,
			FAMILY_DONTCARE = FF_DONTCARE,
			FAMILY_MODERN = FF_MODERN,
			FAMILY_ROMAN = FF_ROMAN,
			FAMILY_SCRIPT = FF_SCRIPT,
			FAMILY_SWISS = FF_SWISS,
		};

	public:
		Font() :
			m_hwnd(nullptr),
			m_hFont(nullptr),
			m_Name(L"Sans-serif"),
			m_Weight(WEIGHT_NORMAL),
			m_CharSet(CHARSET_DEFAULT),
			m_OutPrecision(OUTPRECISION_DEFAULT),
			m_ClipPrecision(CLIPPRECISION_DEFAULT),
			m_Quality(QUALITY_DEFAULT),
			m_Pitch(PITCH_DEFAULT),
			m_IsItalic(false),
			m_IsUnderline(false),
			m_IsStrikeOut(false)
		{ }

		void Create() {
			if (m_hFont != nullptr) {
				Log::Warning("font already created");
				return;
			}

			if (m_Name == nullptr)
				Raise("Font name is nullptr");

			m_hFont = CreateFont(
				m_Size.y, m_Size.x,
				m_Escapement, m_Orientation, m_Weight,
				m_IsItalic, m_IsUnderline, m_IsStrikeOut,
				m_CharSet, m_OutPrecision, m_ClipPrecision, m_Quality,
				(Byte)m_Pitch | m_Family, m_Name);

			if (m_hFont == nullptr)
				Raise("Failed to create font");
		}
		void Update() {
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
		void Delete() {
			if (m_hFont != nullptr) {
				DeleteObject(m_hFont);
				m_hFont = nullptr;
			}
		}
		void SetFontToWindow(const HandleWindow& window) {
			m_hwnd = window.GetHandle();
			if (m_hFont == nullptr)
				Raise("Font is not created");
			SendMessage(m_hwnd, WM_SETFONT, (WPARAM)m_hFont, MAKELPARAM(TRUE, 0));
		}

		void EnableItalic() noexcept {
			m_IsItalic = true;
		}
		void EnableUnderline() noexcept {
			m_IsUnderline = true;
		}
		void EnableStrikeOut() noexcept {
			m_IsStrikeOut = true;
		}
		void DisableItalic() noexcept {
			m_IsItalic = false;
		}
		void DisableUnderline() noexcept {
			m_IsUnderline = false;
		}
		void DisableStrikeOut() noexcept {
			m_IsStrikeOut = false;
		}

		cwString GetName() const noexcept {
			return m_Name;
		}
		uInt2D GetSize() const noexcept {
			return m_Size;
		}
		Int GetEscapement() const noexcept {
			return m_Escapement; 
		}
		Int GetOrientation() const noexcept {
			return m_Orientation; 
		}
		Int GetWeight() const noexcept {
			return m_Weight; 
		}
		Int GetCharSet() const noexcept {
			return m_CharSet;
		}
		Int GetOutPrecision() const noexcept {
			return m_OutPrecision;
		}
		Int GetClipPrecision() const noexcept {
			return m_ClipPrecision;
		}
		Int GetQuality() const noexcept {
			return m_Quality;
		}
		Int GetPitch() const noexcept {
			return m_Pitch;
		}
		Int GetFamily() const noexcept {
			return m_Family;
		}
		Bool IsItalic() const noexcept {
			return m_IsItalic;
		}
		Bool IsUnderline() const noexcept {
			return m_IsUnderline;
		}
		Bool IsStrikeOut() const noexcept {
			return m_IsStrikeOut;
		}

		void SetSize(const uInt2D& size) noexcept {
			m_Size = size;
		}
		void SetEscapement(const Int& escapement) noexcept {
			m_Escapement = escapement;
		}
		void SetOrientation(const Int& orientation) noexcept {
			m_Orientation = orientation;
		}
		void SetWeight(const Int& weight) noexcept {
			m_Weight = weight;
		}
		void SetCharSet(const CharSet& charSet) noexcept {
			m_CharSet = charSet;
		}
		void SetOutPrecision(const OutPrecision& outPrecision) noexcept {
			m_OutPrecision = outPrecision;
		}
		void SetClipPrecision(const ClipPrecision& clipPrecision) noexcept {
			m_ClipPrecision = clipPrecision;
		}
		void SetQuality(const Quality& quality) noexcept {
			m_Quality = quality;
		}
		void SetPitch(const Pitch& pitch) noexcept {
			m_Pitch = pitch;
		}
		void SetFamily(const Family& family) noexcept {
			m_Family = family;
		}

	private:
		HWND m_hwnd;
		HFONT m_hFont;
		uInt2D m_Size;
		Int m_Escapement;
		Int m_Orientation;
		Int m_Weight;
		cwString m_Name;

		CharSet m_CharSet;
		OutPrecision m_OutPrecision;
		ClipPrecision m_ClipPrecision;
		Quality m_Quality;
		Pitch m_Pitch;
		Family m_Family;

		Bool m_IsItalic;
		Bool m_IsUnderline;
		Bool m_IsStrikeOut;
	};
}