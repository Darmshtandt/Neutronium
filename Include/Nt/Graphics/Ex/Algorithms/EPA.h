#pragma once

#include <Nt/Graphics/Ex/Components/Physics.h>
#include <Nt/Physics/Simplex.h>
#include <Nt/Core/Math/Vector.h>

namespace NtEx {
	using namespace Nt;

	struct CollisionPoint final {
		Float3D Normal;
		Float Depth = FLT_MIN;
	};
	struct FaceNormal final {
		Float3D Normal;
		Float Distance = FLT_MIN;
	};

	struct NT_API EdgeIndices final {
		NT_NODISCARD NT_FORCE_INLINE
		Bool operator == (const EdgeIndices& edge) const noexcept;

		uInt A = 0;
		uInt B = 0;
	};
	struct FaceIndices final {
		union {
			struct {
				uInt A;
				uInt B;
				uInt C;
			};

			uInt Arr[3];
		};
	};

	class NT_API EPA final {
	public:
		NT_CONSTEXPR static Float Epsilon = 0.000001f;

	public:
		struct NormalContainer final {
			NT_FORCE_INLINE
			void Reserve(const uInt& capacity) {
				Normals.reserve(capacity);
			}

			NT_FORCE_INLINE
			void EmplaceBack(const Float3D& normal, const Float& distance) {
				Normals.emplace_back(normal, distance);
			}

			NT_NODISCARD NT_FORCE_INLINE
			const FaceNormal& operator [] (const uInt& index) const {
				Assert(index < Normals.size(), "Out of range");
				return Normals[index];
			}

			std::vector<FaceNormal> Normals;
			uInt NearFaceIndex = uInt(-1);
		};

	public:
		NT_NODISCARD static
		CollisionPoint CheckCollision(const Simplex& simplex, const ColliderPair& colliderPair);

	private:
		static void AddUniqueEdge(std::vector<EdgeIndices>& edges, uInt a, uInt b);
		NT_NODISCARD static NormalContainer PolytopeToNormals(const Polytope& polytope, const std::vector<FaceIndices>& faces);
	};
}
