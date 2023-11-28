#pragma once

namespace Nt {
	class Texture : public Image {
	public:
		enum MinFilter {
			MIN_NEAREST = GL_NEAREST,
			MIN_LINEAR = GL_LINEAR,
			MIN_NEAREST_MIPMAP_NEAREST = GL_NEAREST_MIPMAP_NEAREST,
			MIN_LINEAR_MIPMAP_NEAREST = GL_LINEAR_MIPMAP_NEAREST,
			MIN_NEAREST_MIPMAP_LINEAR = GL_NEAREST_MIPMAP_LINEAR,
			MIN_LINEAR_MIPMAP_LINEAR = GL_LINEAR_MIPMAP_LINEAR,
		};
		enum MagFilter {
			MAG_NEAREST = GL_NEAREST,
			MAG_LINEAR = GL_LINEAR
		};
		enum Wrap {
			WRAP_CLAMP_TO_BORDER = GL_CLAMP_TO_BORDER, 
			WRAP_CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE, 
			WRAP_MIRRORED_REPEAT = GL_MIRRORED_REPEAT, 
			WRAP_MIRROR_CLAMP_TO_EDGE = GL_MIRROR_CLAMP_TO_EDGE,
			WRAP_REPEAT = GL_REPEAT,
			WRAP_CLAMP = GL_CLAMP,
		};

	public:
		NT_API Texture() noexcept;
		NT_API Texture(const Texture& NewTexture) noexcept;
		NT_API Texture(const String& FileName);
		NT_API ~Texture();

		constexpr uInt Sizeof() const noexcept override {
			return sizeof(*this);
		}

		NT_API void FlipVerticaly();
		NT_API void Rotate_90_Degrees(const Bool& toRight);

		NT_API void SetMinFiler(const MinFilter& filer) const noexcept;
		NT_API void SetMagFiler(const MagFilter& filer) const noexcept;
		NT_API void SetMinLOD(const Int& value) const noexcept;
		NT_API void SetMaxLOD(const Int& value) const noexcept;
		NT_API void SetWrapS(const Wrap& wrap) const noexcept;
		NT_API void SetWrapT(const Wrap& wrap) const noexcept;

		NT_API void Set() const;

		NT_API uInt GetID() const noexcept;

	private:
		uInt m_ID;

	private:
		NT_API void _Create();
		NT_API void _LoadFromFile() override;
		NT_API void _Release() override;
	};
}