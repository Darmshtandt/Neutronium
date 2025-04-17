#pragma once

#include <Nt/Core/Math/Vector.h>

namespace Nt {
	enum class InternalFormat {
		RGB4 = 0x804F,
		RGB5 = 0x8050,
		RGB8 = 0x8051,
		RGB10 = 0x8052,
		RGB12 = 0x8053,
		RGB16 = 0x8054,

		RGBA2 = 0x8055,
		RGBA4 = 0x8056,
		RGB5_A1 = 0x8057,
		RGBA8 = 0x8058,
		RGB10_A2 = 0x8059,
		RGBA12 = 0x805A,
		RGBA16 = 0x805B,

		DEPTH24_STENCIL8 = 0x88F0,
	};

	class RenderBuffer {
	public:
		RenderBuffer() noexcept = default;
		NT_API RenderBuffer(RenderBuffer&& buffer) noexcept;
		NT_API ~RenderBuffer() noexcept;

		NT_API void Create();
		NT_API void Delete() noexcept;

		NT_API void Bind() const noexcept;
		NT_API void Unbind() const noexcept;

		NT_API void StorageMultisample(const uInt& samples, const InternalFormat& format, const uInt2D& size) const noexcept;

		NT_API _NODISCARD uInt GetID() const noexcept;

	private:
		uInt m_ID;
	};
}