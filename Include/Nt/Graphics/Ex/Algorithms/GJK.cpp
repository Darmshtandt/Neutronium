#include <Nt/Graphics/Ex/Algorithms/GJK.h>

namespace NtEx {
	using namespace Nt;

	std::pair<Bool, Simplex> GJK::CheckCollision(const ColliderPair& colliderPair) {
		Simplex simplex;
		simplex.Add(Support(colliderPair, { 1.f, 0.f, 0.f }));

		Float3D direction = -simplex[0];
		do {
			Float3D point = Support(colliderPair, direction);
			if (point.Dot(direction) <= FLT_EPSILON)
				return std::make_pair(false, Simplex());

			simplex.Add(point);
		}
		while (!NextSimplex(simplex, direction));
		return std::make_pair(true, simplex);
	}

	Float3D GJK::FindFurthestPoint(const Collider::Data& collider, const Float3D& direction) {
		const Matrix4x4& worldMatrix = collider.LocalWorld;
		const Float3D directionInWorld = 
			(worldMatrix.GetInverse() * Float4D(direction, 1.f)).xyz.GetNormalize();

		Float4D maxPoint;
		Float maxDistance = -FLT_MAX;
		for (const Float4D& point : collider.Points) {
			const Float distance = point.xyz.Dot(directionInWorld);
			if (maxDistance < distance) {
				maxDistance = distance;
				maxPoint = point;
			}
		}
		return (worldMatrix * maxPoint).xyz;
	}

	Float3D GJK::Support(const ColliderPair& colliderPair, const Float3D& direction) {
		return
			FindFurthestPoint(colliderPair.first, direction) -
			FindFurthestPoint(colliderPair.second, -direction);
	}

	Bool GJK::Line(Simplex& simplex, Float3D& direction) {
		const Float3D a = simplex[0];
		const Float3D b = simplex[1];

		const Float3D ab = b - a;
		const Float3D ao = -a;

		if (ab.Dot(ao) > FLT_EPSILON) {
			direction = ab.GetCross(ao).GetCross(ab);
		}
		else {
			simplex = { a };
			direction = ao;
		}
		return false;
	}

	Bool GJK::Triangle(Simplex& simplex, Float3D& direction) {
		const Float3D a = simplex[0];
		const Float3D b = simplex[1];
		const Float3D c = simplex[2];

		const Float3D ab = b - a;
		const Float3D ac = c - a;
		const Float3D ao = -a;

		const Float3D abc = ab.GetCross(ac);

		if (abc.GetCross(ac).Dot(ao) > FLT_EPSILON) {
			if (ac.Dot(ao) > FLT_EPSILON) {
				simplex = { a, c };
				direction = ac.GetCross(ao).GetCross(ac);
			}
			else {
				simplex = { a, b };
				return Line(simplex, direction);
			}
		}
		else if (ab.GetCross(abc).Dot(ao) > 0) {
			simplex = { a, b };
			return Line(simplex, direction);
		}
		else {
			if (abc.Dot(ao) > FLT_EPSILON) {
				direction = abc;
			}
			else {
				simplex = { a, c, b };
				direction = -abc;
			}
		}
		return false;
	}

	Bool GJK::Polyhedron(Simplex& simplex, Float3D& direction) {
		const Float3D a = simplex[0];
		const Float3D b = simplex[1];
		const Float3D c = simplex[2];
		const Float3D d = simplex[3];

		const Float3D ab = b - a;
		const Float3D ac = c - a;
		const Float3D ad = d - a;
		const Float3D ao = -a;

		const Float3D abc = ab.GetCross(ac);
		const Float3D acd = ac.GetCross(ad);
		const Float3D adb = ad.GetCross(ab);

		if (abc.Dot(ao) > FLT_EPSILON) {
			simplex = { a, b, c };
			return Triangle(simplex, direction);
		}
		if (acd.Dot(ao) > FLT_EPSILON) {
			simplex = { a, c, d };
			return Triangle(simplex, direction);
		}
		if (adb.Dot(ao) > FLT_EPSILON) {
			simplex = { a, d, b };
			return Triangle(simplex, direction);
		}

		direction = { };
		return true;
	}

	Bool GJK::NextSimplex(Simplex& simplex, Float3D& direction) {
		switch (simplex.GetSize()) {
		case 2:
			return Line(simplex, direction);
		case 3:
			return Triangle(simplex, direction);
		case 4:
			return Polyhedron(simplex, direction);
		default:
			break;
		}
		return true;
	}

	Matrix4x4 GJK::TransformWorldMatrix(const Matrix4x4& world) noexcept {
		Matrix4x4 transformedWorld = world.Transposition();

		const Float3D left = world[0];
		const Float3D up = world[1];
		const Float3D lookAt = world[2];
		const Float3D eye = world[3];

		transformedWorld[0].xyz = left / left.LengthSquare();
		transformedWorld[0].w = -eye.Dot(left) / left.LengthSquare();

		transformedWorld[1].xyz = up / up.LengthSquare();
		transformedWorld[1].w = -eye.Dot(up) / up.LengthSquare();

		transformedWorld[2].xyz = lookAt / lookAt.LengthSquare();
		transformedWorld[2].w = -eye.Dot(lookAt) / lookAt.LengthSquare();
		transformedWorld[3][3] = 1.f;

		return transformedWorld;
	}
}
