#pragma once

#include <Nt/Graphics/System/HandleWindow.h>

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
			CHARSET_ANSI = 0,
			CHARSET_BALTIC = 186,
			CHARSET_CHINESEBIG5 = 136,
			CHARSET_DEFAULT = 1,
			CHARSET_EASTEUROPE = 238,
			CHARSET_GB2312 = 134,
			CHARSET_GREEK = 161,
			CHARSET_HANGUL = 129,
			CHARSET_MAC = 77,
			CHARSET_OEM = 255,
			CHARSET_RUSSIAN = 204,
			CHARSET_SHIFTJIS = 128,
			CHARSET_SYMBOL = 2,
			CHARSET_TURKISH = 162,
			CHARSET_VIETNAMESE = 163
		};
		enum OutPrecision : Byte {
			OUTPRECISION_CHARACTER = 2,
			OUTPRECISION_DEFAULT = 0,
			OUTPRECISION_DEVICE = 5,
			OUTPRECISION_OUTLINE = 8,
			OUTPRECISION_PS_ONLY = 10,
			OUTPRECISION_RASTER = 6,
			OUTPRECISION_STRING = 1,
			OUTPRECISION_STROKE = 3,
			OUTPRECISION_TT_ONLY = 7,
			OUTPRECISION_TT = 4,
		};
		enum ClipPrecision : Byte {
			CLIPPRECISION_CHARACTER = 1,
			CLIPPRECISION_DEFAULT = 0,
			CLIPPRECISION_DFA_DISABLE = (4 << 4),
			CLIPPRECISION_EMBEDDED = (8 << 4),
			CLIPPRECISION_LH_ANGLES = (1 << 4),
			CLIPPRECISION_MASK = 0xf,
			CLIPPRECISION_STROKE = 2,
			CLIPPRECISION_TT_ALWAYS = (2 << 4),
		};
		enum Quality : Byte {
			QUALITY_ANTIALIASED = 4,
			QUALITY_CLEARTYPE = 5,
			QUALITY_DEFAULT = 0,
			QUALITY_DRAFT = 1,
			QUALITY_NONANTIALIASED = 3,
			QUALITY_PROOF = 2,
		};
		enum Pitch : Byte {
			PITCH_DEFAULT,
			PITCH_FIXED,
			PITCH_VARIABLE,
		};
		enum Family : Byte {
			FAMILY_DECORATIVE = (5 << 4),
			FAMILY_DONTCARE = (0 << 4),
			FAMILY_MODERN = (3 << 4),
			FAMILY_ROMAN = (1 << 4),
			FAMILY_SCRIPT = (4 << 4),
			FAMILY_SWISS = (2 << 4),
		};

	public:
		Font() noexcept = default;

		void Create();
		void Update();
		void Delete();
		void SetFontToWindow(const HandleWindow& window);

		void EnableItalic() noexcept;
		void EnableUnderline() noexcept;
		void EnableStrikeOut() noexcept;
		void DisableItalic() noexcept;
		void DisableUnderline() noexcept;
		void DisableStrikeOut() noexcept;

		cwString GetName() const noexcept;
		uInt2D GetSize() const noexcept;
		Int GetEscapement() const noexcept;
		Int GetOrientation() const noexcept;
		Weight GetWeight() const noexcept;
		CharSet GetCharSet() const noexcept;
		OutPrecision GetOutPrecision() const noexcept;
		ClipPrecision GetClipPrecision() const noexcept;
		Quality GetQuality() const noexcept;
		Pitch GetPitch() const noexcept;
		Family GetFamily() const noexcept;
		Bool IsItalic() const noexcept;
		Bool IsUnderline() const noexcept;
		Bool IsStrikeOut() const noexcept;

		void SetSize(const uInt2D& size) noexcept;
		void SetEscapement(const Int& escapement) noexcept;
		void SetOrientation(const Int& orientation) noexcept;
		void SetWeight(const Weight& weight) noexcept;
		void SetCharSet(const CharSet& charSet) noexcept;
		void SetOutPrecision(const OutPrecision& outPrecision) noexcept;
		void SetClipPrecision(const ClipPrecision& clipPrecision) noexcept;
		void SetQuality(const Quality& quality) noexcept;
		void SetPitch(const Pitch& pitch) noexcept;
		void SetFamily(const Family& family) noexcept;

	private:
		HWND m_hwnd = nullptr;
		HFONT m_hFont = nullptr;
		uInt2D m_Size;
		Int m_Escapement = 0;
		Int m_Orientation = 0;
		Weight m_Weight = WEIGHT_NORMAL;
		cwString m_Name = L"Sans-serif";

		CharSet m_CharSet = CHARSET_DEFAULT;
		OutPrecision m_OutPrecision = OUTPRECISION_DEFAULT;
		ClipPrecision m_ClipPrecision = CLIPPRECISION_DEFAULT;
		Quality m_Quality = QUALITY_DEFAULT;
		Pitch m_Pitch = PITCH_DEFAULT;
		Family m_Family = FAMILY_DONTCARE;

		Bool m_IsItalic = false;
		Bool m_IsUnderline = false;
		Bool m_IsStrikeOut = false;
	};
}