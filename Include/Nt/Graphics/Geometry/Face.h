#pragma once

#include <Nt/Graphics/Geometry/Edge.h>
#include <Nt/Graphics/Geometry/Plane.h>

namespace Nt {
	struct Face {
		_CONSTEXPR20 Face() noexcept = default;
		NT_API _CONSTEXPR20 Face(const Vertex& a, const Vertex& b, const Vertex& c) noexcept;

		NT_API _CONSTEXPR20 void Split(const Plane& plane, std::vector<Face>& frontPart, std::vector<Face>& backPart) const;

		NT_API _NODISCARD _CONSTEXPR20 Float3D GetNormal() const noexcept;
		NT_API _NODISCARD _CONSTEXPR20 Float3D GetCenter() const noexcept;

		NT_API _NODISCARD _CONSTEXPR20 Edge GetEdge(const uInt& index) const;

		NT_API _NODISCARD _CONSTEXPR20 Bool HasIntersection(const Face& face) const noexcept;
		NT_API _NODISCARD _CONSTEXPR20 Bool HasIntersection(const Face& face, Float3D& intersectionPoint) const noexcept;
		NT_API _NODISCARD _CONSTEXPR20 Bool HasPointInEdge(const Float3D& point) const noexcept;
		NT_API _NODISCARD _CONSTEXPR20 Bool IsConnected(const Face& other) const;

		NT_API _NODISCARD _CONSTEXPR20 Bool operator == (const Face& face) const noexcept;

		NT_API _CONSTEXPR20 void SetEdge(const Edge& edge, const uInt& index);

		std::array<Vertex, 3> Vertices;
	};
}