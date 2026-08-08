#pragma once

#include <Nt/Core/Math/Vector.h>
#include <Nt/Graphics/Resources/Texture.h>
#include <unordered_map>

namespace Nt {
	struct FreeType;

	class FTFont {
	public:
		struct Character {
			std::unique_ptr<Texture> pGlyph;
			uInt2D Size;
			Int2D Bearing;
			uInt Advance;
		};

	public:
		NT_API FTFont();
		NT_API FTFont(const std::string& filePath);
		NT_API ~FTFont();

		NT_API void LoadFromFile(const std::string& filePath);

		NT_API const Character& GetCharacter(const Byte& glyph) const noexcept;
		NT_API NT_NODISCARD Bool IsLoaded() const noexcept;

		NT_API void SetPixelSizes(const uInt& width, const uInt& height);
		NT_API void SetPixelSizes(const uInt& size);

	private:
		std::unordered_map<Byte, Character> m_Characters;
		std::unique_ptr<FreeType> m_FreeType;
		std::string m_FilePath;
		Bool m_IsLoaded = false;

	private:
		NT_API void _InitializeLibrary();
		NT_API void _Delete();
	};
}