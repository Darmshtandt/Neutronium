#pragma once

#include <Nt/Core/Math/Vector.h>

namespace Nt {
	struct Vertex {
		_CONSTEXPR20 Vertex() noexcept = default;
		NT_API _CONSTEXPR20 Vertex(const Float3D& XYZ, const Float3D& nXnYnZ, const Float3D& UVW, const Float4D& RGBA) noexcept;

		NT_API _CONSTEXPR20 Bool operator == (const Vertex& other) const noexcept;

		Float4D Position;
		Float4D Normal;
		Float3D TexCoord;
		Float4D Color;
	};
}