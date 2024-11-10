#pragma once

namespace Nt {
	struct Ray {
		explicit Ray() = default;
		Ray(const Float3D& start, const Float3D& end) noexcept :
			Start(start),
			End(end)
		{ 
		}

		Bool IntersectTriangleTest(const Double3D face[3], Float3D* pResultIntersectionPoint = nullptr) const {
			constexpr Double epsilon = std::numeric_limits<Double>::epsilon();

			const Double3D ab = Double3D(face[1] - face[0]);
			const Double3D ac = Double3D(face[2] - face[0]);

			const Double3D rayDirection = -Direction();

			const Double3D rayCrossAC = rayDirection.GetCross(ac);
			const Double determinant = ab.Dot(rayCrossAC);
			if (std::abs(determinant) < epsilon)
				return false;

			const Double inverseDeterminant = 1.0 / determinant;
			const Double3D rayStartToA = Double3D(Start - face[0]);

			const Double intersectionParam1 = rayStartToA.Dot(rayCrossAC) * inverseDeterminant;
			if (intersectionParam1 < 0.0 || intersectionParam1 > 1.0)
				return false;

			const Double3D Q = rayStartToA.GetCross(ab);

			const Double intersectionParam2 = rayDirection.Dot(Q) * inverseDeterminant;
			if (intersectionParam2 < 0.0 || intersectionParam1 + intersectionParam2 > 1.0)
				return false;

			const Double rayToTriangle = ac.Dot(Q) * inverseDeterminant;
			if (rayToTriangle < epsilon)
				return false;

			if (pResultIntersectionPoint != nullptr)
				(*pResultIntersectionPoint) = (Start + Float3D(rayDirection) * Float(rayToTriangle));
			return true;
		}

		Float Length() const noexcept {
			return (End - Start).Length();
		}
		Float3D Direction() const noexcept {
			return (End - Start).GetNormalize();
		}

		Float3D Start;
		Float3D End;
	};

	__inline Ray RayFromPoint2D(const Float2D point, const Float3D& rayStart, const uInt2D& windowSize, const Nt::Matrix4x4& projection, const Nt::Matrix4x4& view) {
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