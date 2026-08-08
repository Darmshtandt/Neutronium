#include <Nt/Graphics/Ex/Algorithms/EPA.h>
#include <Nt/Graphics/Ex/Algorithms/GJK.h>

namespace NtEx {
	using namespace Nt;

	Bool EdgeIndices::operator == (const EdgeIndices& edge) const noexcept {
		return A == edge.A && B == edge.B;
	}

	CollisionPoint EPA::CheckCollision(const Simplex& simplex, const ColliderPair& colliderPair) {
		Polytope polytope = simplex.ToPolytope();

		std::vector<FaceIndices> faces = {
			{ 0, 1, 2 },
			{ 0, 3, 1 },
			{ 0, 2, 3 },
			{ 1, 3, 2 }
		};

		NormalContainer normalContainer = PolytopeToNormals(polytope, faces);
		uInt& minFace = normalContainer.NearFaceIndex;

		Float3D minNormal = normalContainer[minFace].Normal;
		Float minDistance = FLT_MAX;

		const uInt pointCount = 
			colliderPair.first.Points.size() +
			colliderPair.second.Points.size();

		for (uInt i = 0; i < pointCount; ++i) {
			minNormal = normalContainer[minFace].Normal;
			minDistance = normalContainer[minFace].Distance;

			const Float3D support = GJK::Support(colliderPair, minNormal);
			const Float supportDistance = minNormal.Dot(support);
			if (std::abs(supportDistance - minDistance) <= Epsilon)
				break;

			minDistance = FLT_MAX;
			std::vector<EdgeIndices> uniqueEdges;

			uInt faceID = 0;
			for (const FaceNormal& faceNormal : normalContainer.Normals) {
				if (faceNormal.Normal.Dot(support) > 0.f) {
					const FaceIndices& face = faces[faceID];

					AddUniqueEdge(uniqueEdges, face.A, face.B);
					AddUniqueEdge(uniqueEdges, face.B, face.C);
					AddUniqueEdge(uniqueEdges, face.C, face.A);

					std::swap(*(faces.begin() + faceID), *(faces.end() - 1));
					faces.erase(faces.end() - 1);
				}
				else {
					++faceID;
				}
			}

			std::vector<FaceIndices> newFaces;
			newFaces.reserve(uniqueEdges.size());
			for (auto [edgeIndex1, edgeIndex2] : uniqueEdges)
				newFaces.push_back({ edgeIndex1, edgeIndex2, polytope.size() });
			polytope.push_back(support);

			faces.insert(faces.end(), newFaces.begin(), newFaces.end());

			normalContainer = PolytopeToNormals(polytope, faces);
		}

		if (std::abs(minDistance - FLT_MAX) < Epsilon)
			return CollisionPoint { minNormal, 0.f };
		return CollisionPoint { minNormal, minDistance + Epsilon };
	}

	void EPA::AddUniqueEdge(std::vector<EdgeIndices>& edges, uInt a, uInt b) {
		auto reverse = std::find(edges.begin(), edges.end(), EdgeIndices(b, a));
		if (reverse != edges.end())
			edges.erase(reverse);
		else
			edges.emplace_back(a, b);
	}

	EPA::NormalContainer EPA::PolytopeToNormals(const Polytope& polytope, const std::vector<FaceIndices>& faces) {
		NormalContainer container;
		container.Reserve(faces.size());

		Float minDistance = FLT_MAX;
		for (uInt i = 0; i < faces.size(); ++i) {
			const FaceIndices& face = faces[i];

			const Float3D& a = polytope[face.A];
			const Float3D& b = polytope[face.B];
			const Float3D& c = polytope[face.C];

			Float3D normal = (b - a).GetCross(c - a).GetNormalize();
			if (normal.LengthSquare() < Epsilon) 
				continue;

			Float distance = normal.Dot(a);
			if (distance < -Epsilon) {
				normal = -normal;
				distance = -distance;
			}

			container.EmplaceBack(normal, distance);
			if (distance < minDistance) {
				container.NearFaceIndex = i;
				minDistance = distance;
			}
		}

		return container;
	}
}