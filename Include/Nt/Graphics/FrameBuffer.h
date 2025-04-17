#pragma once

#include <Nt/Core/Math/Rect.h>
#include <Nt/Graphics/RenderBuffer.h>
#include <Nt/Graphics/Resources/Texture.h>

namespace Nt {
	enum class BufferBit {
		COLOR = 0x00004000,
		DEPTH = 0x00000100,
		STENCIL = 0x00000400
	};

	class FrameBuffer {
	public:
		enum class Status : uInt {
			ATTACHMENT_OBJECT_TYPE = 0x8CD0,
			ATTACHMENT_OBJECT_NAME = 0x8CD1,
			ATTACHMENT_TEXTURE_LEVEL = 0x8CD2,
			ATTACHMENT_TEXTURE_CUBE_MAP_FACE = 0x8CD3,
			ATTACHMENT_TEXTURE_LAYER = 0x8CD4,
			COMPLETE = 0x8CD5,
			INCOMPLETE_ATTACHMENT = 0x8CD6,
			INCOMPLETE_MISSING_ATTACHMENT = 0x8CD7,
			INCOMPLETE_DRAW_BUFFER = 0x8CDB,
			INCOMPLETE_READ_BUFFER = 0x8CDC,
			UNSUPPORTED = 0x8CDD
		};
		enum class Type : uInt {
			DEFAULT = 0x8D40,
			READ = 0x8CA8,
			DRAW = 0x8CA9,
		};

	public:
		NT_API FrameBuffer() = default;
		NT_API FrameBuffer(FrameBuffer&& buffer) noexcept;
		NT_API ~FrameBuffer();

		NT_API void Create();

		NT_API void Bind(const Type& type) const;
		NT_API void Unbind(const Type& type) const noexcept;

		NT_API void Blit(const uIntRect& sourceRect, const uIntRect& destinationRect, const BufferBit& bufferBit, const MagFilter& filter) const noexcept;

		NT_API void Invalidate(const Type& type, const uInt& attachment);

		NT_API void SetRenderBuffer(const RenderBuffer& buffer, const uInt& attachment) const;

		NT_API void SetTexture2D(const Texture* pTexture, const uInt& attachment, const uInt& level);

		NT_API void Delete();

		NT_API _NODISCARD static Status GetStatus() noexcept;

		NT_API _NODISCARD uInt GetID() const noexcept;
		NT_API _NODISCARD Bool IsCreated() const noexcept;

	private:
		uInt m_ID = 0;
		uInt m_TextureID = 0;
	};
}