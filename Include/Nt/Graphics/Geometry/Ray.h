#pragma once

#include <Nt/Core/Math/Matrix4x4.h>
#include <Nt/Graphics/Geometry/Face.h>

namespace Nt {
	struct Ray {
		NT_CONSTEXPR Ray() = default;
		NT_CONSTEXPR Ray(const Float3D& start, const Float3D& end) noexcept :
			Start(start),
			End(end)
		{ 
		}

		NT_NODISCARD
		Bool IntersectTriangleTest(const Face& face) const {
			Float3D temp;
			return IntersectTriangleTest(face, temp);
		}

		NT_NODISCARD
		Bool IntersectTriangleTest(const Float3D points[3]) const {
			Float3D temp;
			return IntersectTriangleTest(points, temp);
		}

		NT_NODISCARD
		Bool IntersectTriangleTest(const Face& face, Float3D& resultIntersectionPoint) const {
			const Float3D points[3] = {
				face.Vertices[0].Position.xyz,
				face.Vertices[1].Position.xyz,
				face.Vertices[2].Position.xyz
			};

			return IntersectTriangleTest(points, resultIntersectionPoint);
		}

		NT_NODISCARD
		Bool IntersectTriangleTest(const Float3D points[3], Float3D& resultIntersectionPoint) const {
			const Float3D ab = points[1] - points[0];
			const Float3D ac = points[2] - points[0];

			const Float3D rayDirection(Direction());

			const Float3D rayCrossAC = rayDirection.GetCross(ac);
			const Float determinant = ab.Dot(rayCrossAC);
			if (Abs(determinant) < FLT_EPSILON)
				return false;

			const Float inverseDeterminant = 1.f / determinant;
			const Float3D rayStartToA = Start - points[0];

			const Float intersectionParam1 = rayStartToA.Dot(rayCrossAC) * inverseDeterminant;
			if (intersectionParam1 < 0.f || intersectionParam1 > 1.f)
				return false;

			const Float3D Q = rayStartToA.GetCross(ab);

			const Float intersectionParam2 = rayDirection.Dot(Q) * inverseDeterminant;
			if (intersectionParam2 < 0.f || intersectionParam1 + intersectionParam2 > 1.f)
				return false;

			const Float rayToTriangle = ac.Dot(Q) * inverseDeterminant;
			if (rayToTriangle < FLT_EPSILON || rayToTriangle * rayToTriangle >= LengthSquare())
				return false;

			resultIntersectionPoint = Start + rayDirection * rayToTriangle;
			return true;
		}

		NT_NODISCARD_CONSTEXPR Float LengthSquare() const noexcept {
			return (End - Start).LengthSquare();
		}

		NT_NODISCARD Float Length() const noexcept {
			return (End - Start).Length();
		}

		NT_NODISCARD Float3D Direction() const noexcept {
			return (End - Start).GetNormalize();
		}

		Float3D Start;
		Float3D End;
	};

	NT_NODISCARD
	__inline Ray RayFromPoint2D(const Float2D& point, const Float3D& rayStart, const uInt2D& windowSize, const Matrix4x4& projection, const Matrix4x4& view) {
		Ray ray;
		ray.Start = rayStart;
		ray.End = ray.Start;

		Float4D clipCoords;
		clipCoords.x = (2.f * point.x) / windowSize.x - 1.f;
		clipCoords.y = 1.f - (2.f * point.y) / windowSize.y;
		clipCoords.z = -1.f;
		clipCoords.w = 1.f;

		if (projection._44 == 0.f) {
			Float4D eyeCoords = projection.GetInverse() * clipCoords;
			eyeCoords.z = -1.0f;
			eyeCoords.w = 0.0f;

			const Float4D worldCoords = view.GetInverse() * eyeCoords;
			ray.End += Float3D(worldCoords).GetNormalize() * 1000.f;
		}
		else {
			const Matrix4x4 inverseViewProjection = (projection * view).GetInverse();
			Float4D worldStart = inverseViewProjection * clipCoords;
			worldStart /= worldStart.w;

			const Float3D rayOrigin(worldStart);
			const Float3D cameraForward(view._31, view._32, view._33);

			ray.Start = rayOrigin;
			ray.End = ray.Start + cameraForward.GetNormalize() * 1000.f;
		}

		return ray;
	}
}