#pragma once

#include <Nt/Graphics/Ex/Physics/HitBox.h>
#include <Nt/Core/Math/Matrix4x4.h>
#include <Nt/Graphics/Geometry/Primitives.h>

namespace NtEx {
	using namespace Nt;

	struct BoundingBox final {
		NT_NODISCARD Bool HasIntersection(const BoundingBox& other) const noexcept {
			return other.Min <= Max && Min <= other.Max;
		}

		NT_NODISCARD BoundingBox CalcTransformed(const Matrix4x4& localWorld) const noexcept {
			const Float3D corners[8] = {
				{ Min.x, Min.y, Min.z },
				{ Min.x, Min.y, Max.z },
				{ Min.x, Max.y, Min.z },
				{ Min.x, Max.y, Max.z },
				{ Max.x, Min.y, Min.z },
				{ Max.x, Min.y, Max.z },
				{ Max.x, Max.y, Min.z },
				{ Max.x, Max.y, Max.z }
			};

			BoundingBox box;
			for (uInt i = 0; i < 8; ++i)
				box.Grow(localWorld * Float4D(corners[i], 1.f));
			return box;
		}

		void Grow(const BoundingBox& other) noexcept {
			Grow(other.Min);
			Grow(other.Max);
		}
		void Grow(const Float3D& point) noexcept {
			Min = Min.Min(point);
			Max = Max.Max(point);
		}

		NT_NODISCARD Float3D Center() const noexcept {
			return (Max - Min) / 2.f;
		}
		NT_NODISCARD Float Area() const noexcept {
			const Float3D extent = Max - Min;
			return extent.x * extent.y + extent.y * extent.z + extent.z * extent.x;
		}

		NT_NODISCARD Bool HasSphereIntersection(const Float3D& center, Float radius) const noexcept {
			const Float3D closest = Max.Min(Min.Max(center));
			const Float3D diff = center - closest;
			const Float distanceSq = diff.LengthSquare();

			return distanceSq <= radius * radius;
		}

		Float3D Min = { FLT_MAX, FLT_MAX, FLT_MAX };
		Float3D Max = { -FLT_MAX, -FLT_MAX, -FLT_MAX };
	};


	__inline NT_NODISCARD BoundingBox CalculateBoundingBox(const PointContainer& points, const Matrix4x4& localWorld) noexcept {
		BoundingBox box;
		for (const Float4D& point : points) {
			const Float3D position = (point * localWorld).xyz;
			box.Min = box.Min.Min(position);
			box.Max = box.Max.Max(position);
		}

		return box;
	}
	__inline NT_NODISCARD BoundingBox CalculateBoundingBox(const Shape& shape, const Matrix4x4& localWorld) noexcept {
		BoundingBox box;
		for (const Vertex& vertex : shape.Vertices) {
			const Float3D position = (vertex.Position * localWorld).xyz;
			box.Min = box.Min.Min(position);
			box.Max = box.Max.Max(position);
		}

		return box;
	}
}