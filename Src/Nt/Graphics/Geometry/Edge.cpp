// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <Nt/Graphics/Geometry/Edge.h>

namespace Nt {
	_CONSTEXPR20 Edge::Edge(const Vertex& a, const Vertex& b) noexcept :
		Vertices{ a, b }
	{
	}

	_CONSTEXPR20 Bool Edge::IsCollinear(const Edge& edge) const noexcept {
		const Float3D ab = Vertices[1].Position.xyz - Vertices[0].Position.xyz;
		const Float3D cd = edge.Vertices[1].Position.xyz - edge.Vertices[0].Position.xyz;

		return (ab.GetCross(cd) == 0);
	}
	_CONSTEXPR20 Bool Edge::HasPoint(const Vertex& vertex) const noexcept {
		return HasPoint(vertex.Position.xyz);
	}
	_CONSTEXPR20 Bool Edge::HasPoint(const Float3D& point) const noexcept {
		const Float3D& ab = Vertices[1].Position.xyz - Vertices[0].Position.xyz;
		const Float3D& ap = point - Vertices[0].Position.xyz;

		if (ab.GetCross(ap).LengthSquare() > FLT_EPSILON)
			return false;

		const Float& dotProduct = ap.Dot(ab);
		const Float& segmentLengthSquared = ab.Dot(ab);
		return (dotProduct >= 0.f && dotProduct <= segmentLengthSquared);
	}
}