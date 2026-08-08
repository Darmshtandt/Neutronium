#pragma once

#include <Nt/Graphics/Ex/Components/Physics.h>
#include <Nt/Physics/Simplex.h>
#include <Nt/Core/Math/Vector.h>

namespace NtEx {
	using namespace Nt;

	class NT_API GJK final {
	public:
		NT_NODISCARD static Float3D Support(const ColliderPair& pair, const Float3D& direction);

		NT_NODISCARD static
			std::pair<Bool, Simplex> CheckCollision(const ColliderPair& pair);

	private:
		NT_NODISCARD static Float3D FindFurthestPoint(const Collider::Data& collider, const Float3D& direction);

		NT_NODISCARD static Bool Line(Simplex& simplex, Float3D& direction);
		NT_NODISCARD static Bool Triangle(Simplex& simplex, Float3D& direction);
		NT_NODISCARD static Bool Polyhedron(Simplex& simplex, Float3D& direction);

		NT_NODISCARD static Bool NextSimplex(Simplex& simplex, Float3D& direction);

		NT_NODISCARD static Matrix4x4 TransformWorldMatrix(const Matrix4x4& world) noexcept;
	};
}