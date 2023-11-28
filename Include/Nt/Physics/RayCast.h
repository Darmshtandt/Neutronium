#pragma once

namespace Nt {
	struct Ray {
		explicit Ray() = default;
		Ray(const Float3D& start, const Float3D& end) noexcept :
			Start(start),
			End(end)
		{ }

		Float Length() const noexcept {
			return (End - Start).Length();
		}
		Float3D Direction() const noexcept {
			return (End - Start).GetNormalize();
		}

		Float3D Start;
		Float3D End;
	};

	__inline Bool MollerTrumbor(const Ray& ray, const Float3D triangle[3]) {
		const Float3D ba = triangle[1] - triangle[0];
		const Float3D ca = triangle[2] - triangle[0];

		const Float3D ray_ca_Normal = ray.End.GetCross(ca);
		const Float determinant = ba.Dot(ray_ca_Normal);
		if (std::abs(determinant) < 0.000001)
			return false;

		const Float inverseDeterminant = 1.0 / determinant;
		const Float3D rayToA = ray.Start - triangle[0];
		const Float intersectionParam1 = rayToA.Dot(ray_ca_Normal) * inverseDeterminant;
		if (intersectionParam1 < 0 || intersectionParam1 > 1)
			return false;

		const Float3D rayToA_Normal = rayToA.GetCross(ba);
		const Float intersectionParam2 = ray.End.Dot(rayToA_Normal) * inverseDeterminant;
		if (intersectionParam2 < 0 || intersectionParam1 + intersectionParam2 > 1)
			return false;
		return true;
	}
	__inline Bool RayCastTest(const Ray& ray, const Shape& shape) {
		const Bool isUsedIndices = (shape.Indices.size() != 0);
		const uInt max_i = (isUsedIndices) ? shape.Indices.size() : shape.Vertices.size();
		for (uInt i = 2; i < max_i; i += 3) {
			const Float3D triangle[3] = { };
			if (isUsedIndices) {
				triangle[0] = shape.Vertices[shape.Indices[i - 2]].Position;
				triangle[1] = shape.Vertices[shape.Indices[i - 1]].Position;
				triangle[2] = shape.Vertices[shape.Indices[i - 0]].Position;
			}
			else {
				triangle[0] = shape.Vertices[i - 2].Position;
				triangle[1] = shape.Vertices[i - 1].Position;
				triangle[2] = shape.Vertices[i - 0].Position;
			}
			if (MollerTrumbor(ray, triangle))
				return true;
		}
		return false;
	}
}