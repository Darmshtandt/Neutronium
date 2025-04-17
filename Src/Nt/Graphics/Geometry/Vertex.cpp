// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <Nt/Graphics/Geometry/Vertex.h>

namespace Nt {
	_CONSTEXPR20 Vertex::Vertex(const Float3D& position, const Float3D& normal, const Float3D& texCoords, const Float4D& color) noexcept :
		Position(position, 1.f),
		Normal(normal, 1.f),
		TexCoord(texCoords),
		Color(color)
	{
	}

	_CONSTEXPR20 Bool Vertex::operator == (const Vertex& other) const noexcept {
		return (
			Position == other.Position && Normal == other.Normal &&
			TexCoord == other.TexCoord && Color == other.Color
			);
	}
}