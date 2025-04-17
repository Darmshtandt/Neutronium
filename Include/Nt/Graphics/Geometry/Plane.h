#pragma once

#include <Nt/Graphics/Geometry/Vertex.h>

namespace Nt {
	struct Face;

	struct Plane {
		enum Position : uInt { 
			COPLANAR = 0x0001,
			FRONT = 0x0010,
			BACK = 0x0100,
			SPANNING = 0x1000
		};

		NT_API _CONSTEXPR20 Plane() noexcept;
		NT_API _CONSTEXPR20 explicit Plane(const Face& face) noexcept;

		NT_API _NODISCARD _CONSTEXPR20
		Float DistanceTo(const Float3D& point) const noexcept;

		NT_API _NODISCARD _CONSTEXPR20
		Position ClassifyFace(const Face& face) const noexcept;

		NT_API _NODISCARD _CONSTEXPR20
		Bool operator == (const Plane& plane) const noexcept;

		Float3D Normal;
		Float Distance;
	};
}

#include <Nt/Graphics/Geometry/Face.h>