#pragma once

#include <Nt/Graphics/Geometry/Vertex.h>

namespace Nt {
	struct Edge {
		_CONSTEXPR20 Edge() noexcept = default;
		NT_API _CONSTEXPR20 Edge(const Vertex& a, const Vertex& b) noexcept;

		NT_API _CONSTEXPR20 Bool IsCollinear(const Edge& edge) const noexcept;
		NT_API _CONSTEXPR20 Bool HasPoint(const Vertex& vertex) const noexcept;
		NT_API _CONSTEXPR20 Bool HasPoint(const Float3D& point) const noexcept;

		Vertex Vertices[2];
	};
}