#pragma once

#include <Nt/Graphics/Resources/Image.h>

namespace Nt {
	enum MinFilter {
		MIN_NONE = 0x0000,
		MIN_NEAREST = 0x2600,
		MIN_LINEAR = 0x2601,
		MIN_NEAREST_MIPMAP_NEAREST = 0x2700,
		MIN_LINEAR_MIPMAP_NEAREST = 0x2701,
		MIN_NEAREST_MIPMAP_LINEAR = 0x2702,
		MIN_LINEAR_MIPMAP_LINEAR = 0x2703,
	};
	enum MagFilter {
		MAG_NONE = 0x0000,
		MAG_NEAREST = 0x2600,
		MAG_LINEAR = 0x2601
	};
	enum Wrap {
		WRAP_NONE = 0x0000,
		WRAP_CLAMP = 0x2900,
		WRAP_REPEAT = 0x2901,
		WRAP_CLAMP_TO_BORDER = 0x812D,
		WRAP_CLAMP_TO_EDGE = 0x812F,
		WRAP_MIRRORED_REPEAT = 0x8370,
		WRAP_MIRROR_CLAMP_TO_EDGE = 0x8743,
	};

	class Texture : public IResource {
	public:
		struct Parameters {
			MinFilter MinFilter = MIN_NONE;
			MagFilter MagFilter = MAG_NONE;
			Int MinLOD = 0;
			Int MaxLOD = 0;
			Wrap S = WRAP_NONE;
			Wrap T = WRAP_NONE;
		};

	public:
		NT_API Texture() noexcept = default;
		NT_API Texture(const String& fileName);
		NT_API Texture(const Texture& newTexture);
		NT_API Texture(Texture&& otherTexture) noexcept;
		NT_API ~Texture();

		NT_API void Read(std::istream& Stream) override;

		_NODISCARD _CONSTEXPR20 uInt Sizeof() const noexcept override {
			return sizeof(*this);
		}

		NT_API void LoadFromFile(const String& filePath) override;
		NT_API void Release() override;

		NT_API void Create(const uInt& channelsCount, const uInt2D& size, std::unique_ptr<Byte[]>&& pData);
		NT_API void Delete();

		NT_API void GenerateMipmap() const;

		NT_API void FlipVerticaly();
		NT_API void Rotate_90_Degrees(const Bool& toRight);

		NT_API void Bind() const;
		NT_API void BindUnit(const uInt& unitID) const;

		NT_API void SetParameters(const Parameters& parameters);
		NT_API void SetMinFiler(const MinFilter& filter);
		NT_API void SetMagFiler(const MagFilter& filter);
		NT_API void SetMinLOD(const Int& value);
		NT_API void SetMaxLOD(const Int& value);
		NT_API void SetWrapS(const Wrap& wrap);
		NT_API void SetWrapT(const Wrap& wrap);

		NT_API _NODISCARD std::type_index GetType() const;

		NT_API _NODISCARD String GetFilePath() const noexcept override;
		NT_API _NODISCARD uInt2D GetSize() const noexcept;
		NT_API _NODISCARD const std::unique_ptr<Byte[]>& GetData() const noexcept;
		NT_API _NODISCARD std::unique_ptr<Byte[]>& GetData() noexcept;
		NT_API _NODISCARD uInt GetChannelCount() const noexcept;

		NT_API _NODISCARD uInt GetID() const noexcept;
		NT_API _NODISCARD Bool IsCreated() const noexcept;

	private:
		Image m_Image;
		Parameters m_Parameters;
		uInt m_ID = 0;
		uInt m_ColorComponent = 0;

	private:
		NT_API void Create();
	};
}