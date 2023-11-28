#pragma once

#include <wchar.h>
#include <ft2build.h>
#include <freetype/freetype.h>

#pragma comment(lib, "freetype")

namespace Nt {
#	define FT_Assert(FT_Result) if (FT_Result != FT_Err_Ok) Raise(FT_Error_String(FT_Result));

	class Font {
	public:
		struct Character {
			uInt2D Size;
			uInt2D Bearing;
			uInt TextureID;
			uInt Advance;
		};

	public:
		Font() {
			_InitializeLibrary();
		}
		Font(LPCSTR FontName) {
			_InitializeLibrary();
			LoadFromFile(FontName);
		}
		~Font() {
			_Delete();
		}

		void LoadFromFile(LPCSTR FontName) {
			if (m_Name && strcmp(m_Name, FontName) == 0)
				return;
			if (m_IsLoaded)
				_Delete();

			m_Name = FontName;

			FT_Assert(FT_New_Face(m_Lib, m_Name, 0, &m_Face));

			SetPixelSizes(48);

			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			for (uInt i = 0; i < 256; ++i) {
				FT_Assert(FT_Load_Char(m_Face, i, FT_LOAD_RENDER));

				const FT_Bitmap& Bitmap = m_Face->glyph->bitmap;
				const uInt BufferSize = Bitmap.width * Bitmap.rows;

				uInt TextureID = 0;
				if (BufferSize > 0) {
					std::vector<Byte4D> BufferRGBA;
					for (uInt i = 0; i < BufferSize; ++i)
						BufferRGBA.push_back({ 0, 0, 0, Bitmap.buffer[i] });

					glGenTextures(1, &TextureID);
					glBindTexture(GL_TEXTURE_2D, TextureID);
					glTexImage2D(
						GL_TEXTURE_2D,
						0,
						GL_RGBA,
						Bitmap.width,
						Bitmap.rows,
						0,
						GL_RGBA,
						GL_UNSIGNED_BYTE,
						BufferRGBA.data()
					);

					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				}

				m_Textures.push_back(TextureID);

				Character Char = { };
				Char.TextureID = TextureID;
				Char.Size.x = Bitmap.width;
				Char.Size.y = Bitmap.rows;
				Char.Bearing.x = m_Face->glyph->bitmap_left;
				Char.Bearing.y = m_Face->glyph->bitmap_top;
				Char.Advance = m_Face->glyph->advance.x;
				m_Characters.insert(std::pair(i, Char));
			}
			glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

			m_IsLoaded = true;
		}


		Character GetCharacter(const Byte& Glyph) {
			return m_Characters[Glyph];
		}
		Bool IsLoaded() const noexcept {
			return m_IsLoaded;
		}

		void SetPixelSizes(const uInt& Width, const uInt& Height) {
			FT_Assert(FT_Set_Pixel_Sizes(m_Face, Width, Height));
		}
		void SetPixelSizes(const uInt& Size) {
			SetPixelSizes(0, Size);
		}

	private:
		std::map<Byte, Character> m_Characters;
		std::vector<uInt> m_Textures;
		FT_Library m_Lib = nullptr;
		FT_Face m_Face = nullptr;
		LPCSTR m_Name = nullptr;
		bool m_IsLoaded = false;

	private:
		void _InitializeLibrary() {
			FT_Assert(FT_Init_FreeType(&m_Lib));
		}
		void _Delete() {
			if (m_IsLoaded) {
				glDeleteTextures(m_Textures.size(), m_Textures.data());

				FT_Assert(FT_Done_Face(m_Face));
				FT_Assert(FT_Done_FreeType(m_Lib));
				m_IsLoaded = false;
			}
		}
	};

#	undef FT_Assert
}