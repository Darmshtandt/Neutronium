// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <Nt/Graphics/Geometry/Plane.h>

namespace Nt {
	_CONSTEXPR20 Plane::Plane() noexcept :
		Normal(0, 0, 0), 
		Distance(0) 
	{
	}
	_CONSTEXPR20 Plane::Plane(const Face& face) noexcept {
		const Float3D ab = face.Vertices[1].Position.xyz - face.Vertices[0].Position.xyz;
		const Float3D ac = face.Vertices[2].Position.xyz - face.Vertices[0].Position.xyz;

		Normal = ab.GetCross(ac).GetNormalize();
		Distance = -Normal.Dot(face.Vertices[0].Position.xyz);
	}

	_NODISCARD _CONSTEXPR20
	Float Plane::DistanceTo(const Float3D& point) const noexcept {
		return Normal.Dot(point) + Distance;
	}

	_NODISCARD _CONSTEXPR20
	Plane::Position Plane::ClassifyFace(const Face& face) const noexcept {
		Int frontCount = 0;
		Int backCount = 0;

		for (const Vertex& vertex : face.Vertices) {
			const Float distance = DistanceTo(vertex.Position.xyz);

			if (distance > 0)
				frontCount++;
			else if (distance < 0)
				backCount++;
		}

		if (frontCount > 0 && backCount > 0) 
			return SPANNING;
		else if (frontCount > 0) 
			return FRONT;
		else if (backCount > 0) 
			return BACK;

		return COPLANAR;
	}
	_CONSTEXPR20 Bool Plane::operator == (const Plane& plane) const noexcept {
		return (Distance == plane.Distance && Normal == plane.Normal);
	}
}