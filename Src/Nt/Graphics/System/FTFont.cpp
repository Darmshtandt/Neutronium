#define FT_CONFIG_OPTION_ERROR_STRINGS

#include <GL/GLEW.h>
#include <Nt/Graphics/System/FTFont.h>
#include <include/ft2build.h>
#include <include/freetype/freetype.h>

namespace Nt {
	NT_NODISCARD NT_CONSTEXPR
	const Char* FT_ErrorString(const FT_Error& error) noexcept {
		switch (error) {
		case 0:  
			return "no error";
		case 1:  
			return "cannot open resource";
		case 2:  
			return "unknown file format";
		case 3:  
			return "broken file";
		default:
			break;
		}

		return "unknown error";
	}

#	define FT_Assert(FT_Result) \
	if (FT_Result != FT_Err_Ok) \
		Raise(FT_ErrorString(FT_Result));

	struct FreeType final {
		FT_Library Lib;
		FT_Face Face;
	};

	FTFont::FTFont() : m_FreeType(std::make_unique<FreeType>()) {
		_InitializeLibrary();
	}

	FTFont::FTFont(const std::string& filePath) : m_FreeType(std::make_unique<FreeType>()) {
		_InitializeLibrary();
		LoadFromFile(filePath);
	}

	FTFont::~FTFont() {
		_Delete();
	}

	void FTFont::LoadFromFile(const std::string& filePath) {
		if (filePath == m_FilePath)
			return;
		if (m_IsLoaded)
			_Delete();

		m_FilePath = filePath;
		FT_Library& lib = m_FreeType->Lib;
		FT_Face& face = m_FreeType->Face;

		m_Characters.clear();
		FT_Assert(FT_New_Face(lib, m_FilePath.c_str(), 0, &face));
		if (FT_Select_Charmap(face, FT_ENCODING_UNICODE))
			FT_Assert(FT_Select_Charmap(face, FT_ENCODING_MS_SYMBOL));

		SetPixelSizes(48);

		Int oldUnpack;
		glGetIntegerv(GL_UNPACK_ALIGNMENT, &oldUnpack);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		for (uInt code = 0; code < 256; ++code) {
			//FT_Assert(FT_Load_Char(face, glyphIndex, FT_LOAD_RENDER));
			FT_Assert(FT_Load_Char(face, code, FT_LOAD_DEFAULT | FT_LOAD_NO_BITMAP));
			FT_Assert(FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL));

			const FT_GlyphSlot& glyph = face->glyph;
			const FT_Bitmap& bitmap = glyph->bitmap;
			const uInt bufferSize = std::abs(bitmap.pitch) * bitmap.rows;

			const Int pitch = bitmap.pitch;
			const Int rows = static_cast<Int>(bitmap.rows);
			const Int width = static_cast<Int>(bitmap.width);
			if (rows == 0 || width == 0) {
				m_Characters[code];
				continue;
			}

			Byte* pImageData = new Byte[bufferSize * 4];
			ZeroMemory(pImageData, bufferSize * 4);

			uInt it = 0;
			for (Int y = rows - 1; y >= 0; --y) {
				const Byte* pRow;
				if (pitch >= 0) {
					pRow = bitmap.buffer + y * pitch;
				}
				else {
					pRow = bitmap.buffer + (rows - 1 - y) * (-pitch);
				}

				for (Int x = 0; x < width; ++x) {
					Pixel pixel;
					pixel.Vector.rgb = { 255, 255, 255 };

					switch (bitmap.pixel_mode) {
					case FT_PIXEL_MODE_MONO: {
						const Byte byte = pRow[x >> 3];
						const Byte mask = static_cast<Byte>(0x80 >> (x & 7));
						pixel.Vector.a = (byte & mask) ? 255u : 0u;
					}
						break;

					case FT_PIXEL_MODE_GRAY:
					default:
						pixel.Vector.a = pRow[x];
						break;
					}

					memcpy(&pImageData[it], pixel.Array, 4);

					it += 4;
				}
			}

			Character character = { };
			character.Size = { bitmap.width, bitmap.rows };
			character.Bearing = { glyph->bitmap_left, glyph->bitmap_top };
			character.Advance = glyph->advance.x;
			character.pGlyph = std::make_unique<Texture>();
			character.pGlyph->Create(4, character.Size, pImageData);

			if (code == static_cast<uInt>(' ') && character.Advance == 0)
				character.Advance = face->units_per_EM * 0.25f;

			m_Characters[code] = std::move(character);
		}
		glPixelStorei(GL_UNPACK_ALIGNMENT, oldUnpack);

		m_IsLoaded = true;
	}

	const FTFont::Character& FTFont::GetCharacter(const Byte& glyph) const noexcept {
		return m_Characters.at(glyph);
	}

	Bool FTFont::IsLoaded() const noexcept {
		return m_IsLoaded;
	}

	void FTFont::SetPixelSizes(const uInt& width, const uInt& height) {
		FT_Assert(FT_Set_Pixel_Sizes(m_FreeType->Face, width, height));
	}

	void FTFont::SetPixelSizes(const uInt& size) {
		SetPixelSizes(0, size);
	}

	void FTFont::_InitializeLibrary() {
		FT_Assert(FT_Init_FreeType(&m_FreeType->Lib));
	}

	void FTFont::_Delete() {
		if (m_IsLoaded) {
			m_Characters.clear();

			FT_Assert(FT_Done_Face(m_FreeType->Face));
			FT_Assert(FT_Done_FreeType(m_FreeType->Lib));
			m_IsLoaded = false;
		}
	}

#	undef FT_Assert
}