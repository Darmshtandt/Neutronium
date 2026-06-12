// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <Nt/Graphics/Resources/Mesh.h>
#include <Nt/Graphics/Resources/MeshLoaders.h>
#include <Nt/Graphics/Resources/ResourceHandle.h>

namespace Nt {
	Mesh::Mesh(const String& fileName) {
		Mesh::LoadFromFile(fileName);
	}
	Mesh::Mesh(const Shape& shape, const UsageDraw& usage) {
		SetShape(shape, usage);
	}
	Mesh::~Mesh() {
		Mesh::Release();
	}

	void Mesh::LoadFromFile(const String& filePath) {
		m_FilePath = filePath;
		SetShape(OBJ().LoadFromFile(filePath));
	}
	void Mesh::Release() {
		m_Shape = { };
		m_VertexArray.Clear();
	}

	void Mesh::UpdateVertices(const Vertices_t& vertices) {
		Assert(!vertices.empty(), "The number of vertices is 0");

		m_Shape.Vertices = vertices;
		m_VertexArray.UpdateVBO(m_Shape.Vertices.size(), m_Shape.Vertices.data());
	}

	String Mesh::GetFilePath() const noexcept {
		return m_FilePath;
	}

	std::type_index Mesh::GetType() const {
		return typeid(Mesh);
	}

	const Shape& Mesh::GetShape() const noexcept {
		return m_Shape;
	}

	const Vertices_t& Mesh::GetVertices() const noexcept {
		return m_Shape.Vertices;
	}

	const Indices_t& Mesh::GetIndices() const noexcept {
		return m_Shape.Indices;
	}

	Float3D Mesh::GetScale() const noexcept {
		return m_Scale;
	}

	uInt Mesh::GetVerticesCount() const noexcept {
		if (m_Shape.Indices.empty())
			return m_Shape.Vertices.size();
		return m_Shape.Indices.size();
	}

	void Mesh::SetShape(const Shape& newShape, const UsageDraw& usage) {
		SetVertices(newShape.Vertices, usage);
		SetIndices(newShape.Indices, usage);
	}

	void Mesh::SetVertices(const Vertices_t& vertices, const UsageDraw& usage) {
		if (usage != USAGE_STATICDRAW && usage != USAGE_DYNAMICDRAW)
			Raise("Non-existent usage specified");

		Vertices_t* pShapeVertices = &m_Shape.Vertices;
		(*pShapeVertices) = vertices;

		for (Vertex& vertex : m_Shape.Vertices)
			vertex.Position *= Float4D(m_Scale, 1.f);
			
		m_VertexArray.SetVBOData(pShapeVertices->size(), pShapeVertices->data(), usage);
	}
	void Mesh::SetIndices(const Indices_t& indices, const UsageDraw& usage) {
		if (usage != USAGE_STATICDRAW && usage != USAGE_DYNAMICDRAW)
			Raise("Non-existent usage specified");

		m_Shape.Indices = indices;
		if (!m_Shape.Indices.empty())
			m_VertexArray.SetEBOData(m_Shape.Indices.size(), m_Shape.Indices.data(), usage);
	}

	void Mesh::SetColor(const Float4D& color) noexcept {
		for (Vertex& vert : m_Shape.Vertices)
			vert.Color = color;
	}
	void Mesh::SetScale(Float3D scale) {
		if (m_Scale == scale)
			return;

		if (scale.x == 0.f)
			scale.x = FLT_EPSILON;
		if (scale.y == 0.f)
			scale.y = FLT_EPSILON;
		if (scale.z == 0.f || std::isnan(std::fabsf(scale.z)))
			scale.z = FLT_EPSILON;

		const Float3D value = scale / m_Scale;
		for (Vertex& vertex : m_Shape.Vertices)
			vertex.Position *= Float4D(value, 1.f);
			
		m_VertexArray.UpdateVBO(m_Shape.Vertices.size(), m_Shape.Vertices.data());
		m_Scale = scale;
	}

	void Mesh::Bind() const noexcept {
		m_VertexArray.Bind();
	}

	Mesh& Mesh::operator = (const Shape& shape) {
		SetShape(shape);
		return *this;
	}

	template class NT_API ResourceHandle<Mesh>;
}