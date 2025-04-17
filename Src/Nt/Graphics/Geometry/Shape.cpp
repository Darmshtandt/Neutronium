// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <Nt/Graphics/Geometry/Shape.h>

namespace Nt {
	Shape::Shape(const std::vector<Face>& faces) {
		AddAllFaces(faces);
	}
	void Shape::Write(std::ostream& Stream) const {
		Serialization::WriteAll(Stream, Vertices, Indices);
	}
	void Shape::Read(std::istream& Stream) {
		Serialization::ReadAll(Stream, Vertices, Indices);
	}

	_CONSTEXPR20 uInt Shape::Sizeof() const noexcept {
		return sizeof(*this);
	}
	_CONSTEXPR20 uInt Shape::ClassType() const noexcept {
		return 0;
	}

	ISerialization* Shape::New([[maybe_unused]] const uInt& classType) {
		return new Shape;
	}

	void AddVertices(Shape* pShape, const Vertex* pVertices, const uInt& count) {
		RequireNotNull(pShape);
		RequireNotNull(pVertices);

		for (uInt i = 0; i < count; ++i) {
			const Vertices_t::const_iterator iterator =
				std::find(pShape->Vertices.cbegin(), pShape->Vertices.cend(), pVertices[i]);
			if (iterator != pShape->Vertices.cend()) {
				pShape->Indices.push_back(std::distance(pShape->Vertices.cbegin(), iterator));
			}
			else {
				pShape->Indices.push_back(pShape->Vertices.size());
				pShape->Vertices.push_back(pVertices[i]);
			}
		}
	}

	Shape& Shape::AddVertex(const Vertex& vertex) {
		AddVertices(this, &vertex, 1);
		return (*this);
	}
	Shape& Shape::AddEdge(const Edge& edge) {
		AddVertices(this, edge.Vertices, 2);
		return (*this);
	}
	Shape& Shape::AddFace(const Face& face) {
		AddVertices(this, face.Vertices.data(), 3);
		return (*this);
	}

	Shape& Shape::AddAllFaces(const std::vector<Face>& faces) {
		for (const Face& face : faces)
			AddVertices(this, face.Vertices.data(), 3);

		return (*this);
	}
	Shape& Shape::Add(const Shape& shape){
		for (const Index_t& index : shape.Indices) {
			const Vertex& vertex = shape.Vertices[index];

			const Vertices_t::const_iterator iterator = std::find(Vertices.cbegin(), Vertices.cend(), vertex);

			if (iterator != Vertices.cend()) {
				Indices.push_back(std::distance(Vertices.cbegin(), iterator));
			}
			else {
				Indices.push_back(Vertices.size());
				Vertices.push_back(vertex);
			}
		}

		return (*this);
	}

	Shape& Shape::Move(const Float3D& offset) {
		if (offset == Float3D(0.f, 0.f, 0.f))
			return (*this);

		for (Vertex& vertex : Vertices)
			vertex.Position.xyz += offset;
		return (*this);
	}

	std::vector<Face> Shape::GetFaces() const {
		std::vector<Face> faces;
		if (!Indices.empty()) {
			for (uInt i = 2; i < Indices.size(); i += 3) {
				faces.emplace_back(
					Vertices[Indices[i - 2]],
					Vertices[Indices[i - 1]],
					Vertices[Indices[i]]
					);
			}
		}
		else {
			for (uInt i = 2; i < Vertices.size(); i += 3)
				faces.emplace_back(Vertices[i - 2], Vertices[i - 1], Vertices[i]);
		}

		return faces;
	}
}