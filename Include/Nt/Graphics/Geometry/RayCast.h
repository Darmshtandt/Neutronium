#pragma once

#include <Nt/Core/Math/Matrix4x4.h>

namespace Nt {
	struct Ray {
		_CONSTEXPR20 Ray() = default;
		_CONSTEXPR20 Ray(const Float3D& start, const Float3D& end) noexcept :
			Start(start),
			End(end)
		{ 
		}

		_NODISCARD _CONSTEXPR20
		Bool IntersectTriangleTest(const Face& face) const {
			Float3D temp;
			return IntersectTriangleTest(face, temp);
		}

		_NODISCARD _CONSTEXPR20
		Bool IntersectTriangleTest(const Double3D points[3]) const {
			Float3D temp;
			return IntersectTriangleTest(points, temp);
		}

		_NODISCARD _CONSTEXPR20
		Bool IntersectTriangleTest(const Face& face, Float3D& resultIntersectionPoint) const {
			const Double3D points[3] = {
				face.Vertices[0].Position.xyz,
				face.Vertices[1].Position.xyz,
				face.Vertices[2].Position.xyz
			};

			return IntersectTriangleTest(points, resultIntersectionPoint);
		}

		_NODISCARD _CONSTEXPR20
		Bool IntersectTriangleTest(const Double3D points[3], Float3D& resultIntersectionPoint) const {
			const Double3D ab = Double3D(points[1] - points[0]);
			const Double3D ac = Double3D(points[2] - points[0]);

			const Double3D rayDirection(Direction());

			const Double3D rayCrossAC = rayDirection.GetCross(ac);
			const Double determinant = ab.Dot(rayCrossAC);
			if (Abs(determinant) < DBL_EPSILON)
				return false;

			const Double inverseDeterminant = 1.0 / determinant;
			const Double3D rayStartToA = Double3D(Start) - points[0];

			const Double intersectionParam1 = rayStartToA.Dot(rayCrossAC) * inverseDeterminant;
			if (intersectionParam1 < 0.0 || intersectionParam1 > 1.0)
				return false;

			const Double3D Q = rayStartToA.GetCross(ab);

			const Double intersectionParam2 = rayDirection.Dot(Q) * inverseDeterminant;
			if (intersectionParam2 < 0.0 || intersectionParam1 + intersectionParam2 > 1.0)
				return false;

			const Double rayToTriangle = ac.Dot(Q) * inverseDeterminant;
			if (rayToTriangle < DBL_EPSILON || rayToTriangle * rayToTriangle >= LengthSquare())
				return false;

			resultIntersectionPoint = (Start + Float3D(rayDirection) * Float(rayToTriangle));
			return true;
		}

		_NODISCARD _CONSTEXPR20
		Float LengthSquare() const noexcept {
			return (End - Start).LengthSquare();
		}

		_NODISCARD _CONSTEXPR20
		Float Length() const noexcept {
			return (End - Start).Length();
		}

		_NODISCARD _CONSTEXPR20
		Float3D Direction() const noexcept {
			return (End - Start).GetNormalize();
		}

		Float3D Start;
		Float3D End;
	};

	_NODISCARD _CONSTEXPR20
	__inline Ray RayFromPoint2D(const Float2D& point, const Float3D& rayStart, const uInt2D& windowSize, const Matrix4x4& projection, const Matrix4x4& view) {
		Ray ray;
		ray.Start = rayStart;
		ray.End = ray.Start;

		Float4D clipCoords(0.f, 0.f, -1.0f, 1.0f);
		clipCoords.x = (2.f * point.x) / windowSize.x - 1.f;
		clipCoords.y = 1.f - (2.f * point.y) / windowSize.y;

		Float4D eyeCoords = projection.GetInverse() * clipCoords;
		eyeCoords.z = -1.0f;
		eyeCoords.w = 0.0f;

		const Float4D worldCoords = view.GetInverse() * eyeCoords;
		ray.End += Float3D(worldCoords).GetNormalize() * 1000.f;

		return ray;
	}
}