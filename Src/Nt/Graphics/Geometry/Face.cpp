// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <Nt/Graphics/Geometry/Face.h>
#include <Nt/Graphics/Geometry/RayCast.h>

namespace Nt {
	_CONSTEXPR20 Face::Face(const Vertex& a, const Vertex& b, const Vertex& c) noexcept :
		Vertices { a, b, c }
	{
	}
	
	_CONSTEXPR20 void Face::Split(const Plane& plane, std::vector<Face>& frontPart, std::vector<Face>& backPart) const {
		std::vector<Vertex> frontVertices, backVertices;

		for (uInt i = 0; i < 3; ++i) {
			const Vertex& current = Vertices[i];
			const Vertex& next = Vertices[(i + 1) % 3];

			const Float d1 = plane.DistanceTo(current.Position.xyz);
			const Float d2 = plane.DistanceTo(next.Position.xyz);

			if (d1 >= 0)
				frontVertices.push_back(current);
			if (d1 <= 0)
				backVertices.push_back(current);

			const Bool isEdgeIntersectsPlane = (d1 > 0 && d2 < 0) || (d1 < 0 && d2 > 0);
			if (isEdgeIntersectsPlane) {
				const Float coefficient = d1 / (d1 - d2);

				Vertex intersection = current;
				intersection.Position.xyz += (next.Position.xyz - current.Position.xyz) * coefficient;
				intersection.TexCoord += (next.TexCoord - current.TexCoord) * coefficient;
				intersection.Color += (next.Color - current.Color) * coefficient;
				intersection.Normal += (next.Normal - current.Normal) * coefficient;

				frontVertices.push_back(intersection);
				backVertices.push_back(intersection);
			}
		}

		auto triangulate = [](std::vector<Vertex>& vertices) {
			if (vertices.size() != 4)
				return;

			vertices.push_back(vertices[0]);
			vertices.push_back(vertices[2]);
			};

		if (frontVertices.size() == 4)
			triangulate(frontVertices);
		if (backVertices.size() == 4)
			triangulate(backVertices);

		for (uInt i = 2; i < frontVertices.size(); i += 3)
			frontPart.emplace_back(frontVertices[i - 2], frontVertices[i - 1], frontVertices[i]);

		for (uInt i = 2; i < backVertices.size(); i += 3)
			backPart.emplace_back(backVertices[i - 2], backVertices[i - 1], backVertices[i]);
	}

	_CONSTEXPR20 Float3D Face::GetNormal() const noexcept {
		const Float3D edge1(Vertices[1].Position - Vertices[0].Position);
		const Float3D edge2(Vertices[2].Position - Vertices[0].Position);
		return edge1.GetCross(edge2);
	}
	_CONSTEXPR20 Float3D Face::GetCenter() const noexcept {
		Float3D center;
		for (const Vertex& vertex : Vertices)
			center += vertex.Position.xyz;

		return center / 3.f;
	}

	_CONSTEXPR20 Edge Face::GetEdge(const uInt& index) const {
		if (index >= 3)
			Raise("Out of range");

		return Edge(Vertices[index], Vertices[(index + 1) % 3]);
	}

	_CONSTEXPR20 Bool Face::HasIntersection(const Face& face) const noexcept {
		Float3D temp;
		return HasIntersection(face, temp);
	}
	_CONSTEXPR20 Bool Face::HasIntersection(const Face& face, Float3D& intersectionPoint) const noexcept {
		const Ray thisEdges[3] = {
			{ Vertices[0].Position.xyz, Vertices[1].Position.xyz },
			{ Vertices[0].Position.xyz, Vertices[2].Position.xyz },
			{ Vertices[1].Position.xyz, Vertices[2].Position.xyz }
		};
		const Ray otherEdges[3] = {
			{ face.Vertices[0].Position.xyz, face.Vertices[1].Position.xyz },
			{ face.Vertices[0].Position.xyz, face.Vertices[2].Position.xyz },
			{ face.Vertices[1].Position.xyz, face.Vertices[2].Position.xyz }
		};

		for (uInt i = 0; i < 3; ++i) {
			if (thisEdges[i].IntersectTriangleTest(face, intersectionPoint))
				return true;
			if (otherEdges[i].IntersectTriangleTest(*this, intersectionPoint))
				return true;
		}

		return false;
	}

	_CONSTEXPR20 Bool Face::HasPointInEdge(const Float3D& point) const noexcept {
		for (uInt i = 0; i < 3; ++i) {
			if (Edge(Vertices[i], Vertices[(i + 1) % 3]).HasPoint(point))
				return true;
		}
		return false;
	}

	_CONSTEXPR20 Bool Face::IsConnected(const Face& other) const {
		for (const Vertex& pointA : Vertices) {
			for (const Vertex& pointB : other.Vertices) {
				if (pointA.Position.xyz == pointB.Position.xyz)
					return true;
			}
		}

		return false;
	}

	_CONSTEXPR20 Bool Face::operator==(const Face& face) const noexcept {
		for (uInt i = 0; i < 3; ++i) {
			if (Vertices[i] != face.Vertices[i])
				return true;
		}
		return false;
	}
	_CONSTEXPR20 void Face::SetEdge(const Edge& edge, const uInt& index) {
		if (index >= 3)
			Raise("Out of range");

		Vertices[index] = edge.Vertices[0];
		Vertices[(index + 1) % 3] = edge.Vertices[1];
	}
}