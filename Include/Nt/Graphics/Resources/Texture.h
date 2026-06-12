#pragma once

#include <Nt/Graphics/Resources/Image.h>

namespace Nt {
	enum MinFilter : uShort {
		MIN_NONE = 0x0000,
		MIN_NEAREST = 0x2600,
		MIN_LINEAR = 0x2601,
		MIN_NEAREST_MIPMAP_NEAREST = 0x2700,
		MIN_LINEAR_MIPMAP_NEAREST = 0x2701,
		MIN_NEAREST_MIPMAP_LINEAR = 0x2702,
		MIN_LINEAR_MIPMAP_LINEAR = 0x2703,
	};
	enum MagFilter : uShort {
		MAG_NONE = 0x0000,
		MAG_NEAREST = 0x2600,
		MAG_LINEAR = 0x2601
	};
	enum Wrap : uShort {
		WRAP_NONE = 0x0000,
		WRAP_CLAMP = 0x2900,
		WRAP_REPEAT = 0x2901,
		WRAP_CLAMP_TO_BORDER = 0x812D,
		WRAP_CLAMP_TO_EDGE = 0x812F,
		WRAP_MIRRORED_REPEAT = 0x8370,
		WRAP_MIRROR_CLAMP_TO_EDGE = 0x8743,
	};

	class NT_API Texture final : public IResource {
	public:
		struct Parameters {
			MinFilter MinFilter = MIN_NEAREST;
			MagFilter MagFilter = MAG_NEAREST;
			Int MinLOD = 0;
			Int MaxLOD = 0;
			Wrap S = WRAP_REPEAT;
			Wrap T = WRAP_REPEAT;
		};

	public:
		Texture() noexcept = default;
		Texture(const String& fileName);
		Texture(const Texture& newTexture);
		Texture(Texture&& otherTexture) noexcept;
		~Texture() override;

		void LoadFromFile(const String& filePath) override;
		void Release() override;

		void Create(const uInt& channelsCount, const uInt2D& size, Byte* pData);
		void Delete();

		void GenerateMipmap() const;

		void FlipVertically();
		void Rotate_90_Degrees(const Bool& toRight);

		void Bind() const;
		void BindUnit(const uInt& unitID) const;

		void SetParameters(const Parameters& parameters);
		void SetMinFiler(const MinFilter& filter);
		void SetMagFiler(const MagFilter& filter);
		void SetMinLOD(const Int& value);
		void SetMaxLOD(const Int& value);
		void SetWrapS(const Wrap& wrap);
		void SetWrapT(const Wrap& wrap);

		NT_NODISCARD std::type_index GetType() const override;
		NT_NODISCARD MinFilter GetMinFiler() const noexcept;
		NT_NODISCARD MagFilter GetMagFiler() const noexcept;
		NT_NODISCARD Int GetMinLOD() const noexcept;
		NT_NODISCARD Int GetMaxLOD() const noexcept;
		NT_NODISCARD Wrap GetWrapS() const noexcept;
		NT_NODISCARD Wrap GetWrapT() const noexcept;

		NT_NODISCARD String GetFilePath() const noexcept override;
		NT_NODISCARD uInt2D GetSize() const noexcept;
		NT_NODISCARD const Byte* GetData() const noexcept;
		NT_NODISCARD Byte* GetData() noexcept;
		NT_NODISCARD uInt GetChannelCount() const noexcept;

		NT_NODISCARD uInt GetID() const noexcept;
		NT_NODISCARD Bool IsCreated() const noexcept;

	private:
		mutable Parameters m_Parameters;
		Image m_Image;
		uInt m_ID = 0;
		Int m_ColorComponent = 0;

	private:
		void Create();
	};
}