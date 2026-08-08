#pragma once

#include <Nt/Core/Serialization.h>
#include <Nt/Graphics/Geometry/Edge.h>
#include <Nt/Graphics/Geometry/Face.h>

namespace Nt {
	using Index_t = uInt;
	using Vertices_t = std::vector<Vertex>;
	using Indices_t = std::vector<Index_t>;

	struct Shape : public ISerialization {
		Shape() noexcept = default;
		NT_API Shape(const std::vector<Face>& faces);

		NT_API void Write(std::ostream& Stream) const override;
		NT_API void Read(std::istream& Stream) override;

		NT_API _NODISCARD _CONSTEXPR20 uInt Sizeof() const noexcept override;
		NT_API _NODISCARD _CONSTEXPR20 uInt ClassType() const noexcept override;

		NT_API _NODISCARD static ISerialization* New([[maybe_unused]] const uInt& classType);

		NT_API Shape& AddVertex(const Vertex& vertex);
		NT_API Shape& AddEdge(const Edge& edge);
		NT_API Shape& AddFace(const Face& face);
		NT_API Shape& AddAllFaces(const std::vector<Face>& faces);
		NT_API Shape& Add(const Shape& shape);

		NT_API Shape& Move(const Float3D& offset);

		NT_API _NODISCARD std::vector<Face> GetFaces() const;

		Vertices_t Vertices;
		Indices_t Indices;
	};
}