// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <filesystem>

#include <GL/GLEW.h>
#include <GL/GL.h>

#include <Nt/Core/Colors.h>
#include <Nt/Graphics/Resources/Mesh.h>
//#include <Nt/Graphics/Resources/MeshLoaders.h>

namespace Nt {
	Mesh::Mesh() :
		m_pVertexArray(std::make_unique<VertexArray>()),
		m_pShape(std::make_unique<Shape>())
	{
	}
	Mesh::Mesh(const Nt::String& fileName) : Mesh() {
		Mesh::LoadFromFile(fileName);
	}
	Mesh::Mesh(const Shape& newShape, const UsageDraw& usage) : Mesh() {
		SetShape(newShape, usage);
	}
	Mesh::Mesh(const Mesh& newMesh) :
		m_IsUsedIndexBuffer(newMesh.m_IsUsedIndexBuffer),
		m_Scale(newMesh.m_Scale),
		m_pVertexArray(std::make_unique<VertexArray>()),
		m_pShape(std::make_unique<Shape>())
	{
		if (newMesh.m_pShape == nullptr)
			return;

		m_pShape->Vertices = newMesh.m_pShape->Vertices;
		m_pShape->Indices = newMesh.m_pShape->Indices;
			
		m_pVertexArray->SetVBOData(m_pShape->Vertices.size(), m_pShape->Vertices.data());
		m_pVertexArray->SetEBOData(m_pShape->Indices.size(), m_pShape->Indices.data());
	}
	Mesh::Mesh(Mesh&& otherMesh) :
		m_IsUsedIndexBuffer(otherMesh.m_IsUsedIndexBuffer),
		m_Scale(otherMesh.m_Scale),
		m_pVertexArray(std::move(otherMesh.m_pVertexArray)),
		m_pShape(std::move(otherMesh.m_pShape))
	{
	}
	Mesh::~Mesh() {
		Mesh::Release();
	}

	void Mesh::Write(std::ostream& Stream) const {
		Serialization::WriteAll(Stream, m_FilePath);

		if (m_FilePath.empty()) {
			Shape shape = { };
			if (m_pShape != nullptr)
				shape = (*m_pShape);

			Serialization::WriteAll(Stream, shape);
		}
	}
	void Mesh::Read(std::istream& Stream) {
		Serialization::ReadAll(Stream, m_FilePath);

		if (m_FilePath.empty()) {
			Shape shape;
			Serialization::ReadAll(Stream, shape);
			SetShape(shape);
		}
	}

	void Mesh::LoadFromFile(const Nt::String& filePath) {
		m_FilePath = filePath;

		std::ifstream file(m_FilePath);
		if (!file.is_open())
			Raise("Failed to open: " + m_FilePath);

		std::vector<Float4D> positions;
		std::vector<Float3D> colors;
		std::vector<Float3D> texCoords;
		std::vector<Float3D> normals;

		Shape loadedShape;
		uInt LineNumber = 0;

		String line;
		while (std::getline(file, line)) {
			++LineNumber;

			if (line == "" || line == "\r")
				continue;

			const String errorMessage =
				"Failed to load model.\nLine: " + std::to_string(LineNumber);

			std::vector<String> splitedString = line.Split(' ');

			const std::string type = splitedString[0];
			if (type == "mtllib" || type == "#" || type == "usemtl" || type == "o")
				continue;

			Float3D position;
			Float3D color;

			if (type[0] != 'f') {
				const uInt parametersCount = splitedString.size() - 1;

				if (parametersCount == 0)
					Raise(errorMessage);

				for (uInt i = 0; i < parametersCount; ++i) {
					const String& parameter = splitedString[i + 1];

					if (!parameter.IsFloat())
						Raise(errorMessage);

					if (i < 3)
						position[i] = parameter;
					else
						color[i - 3] = parameter;
				}
			}

			Float4D data(position, 1.f);

			switch (type[0]) {
			case 's':
			case 'g':
			case 'o':
			case '#':
				continue;

			case 'v':
				if (type == "v") {
					colors.push_back(color);
					positions.push_back(data);
				}
				else if (type == "vt") {
					texCoords.emplace_back(data);
				}
				else if (type == "vn") {
					normals.push_back(data);
				}
				else if (type == "vp") {

				}
				else {
					Raise(errorMessage);
				}
				break;

			case 'f':
				for (uInt i = 1; i < splitedString.size(); ++i) {
					std::vector<String> splitedData = splitedString[i].Split('/');

					if (splitedData.size() <= 1)
						Raise(errorMessage);

					const Int vertexIndex = Int(splitedData[0]) - 1;

					Vertex vertex = { };
					vertex.Position = positions[vertexIndex];
					vertex.Color =
						(!colors.empty()) ? Float4D(colors[vertexIndex], 1.f) : Colors::White;

					if (!splitedData[1].empty())
						vertex.TexCoord = texCoords[Int(splitedData[1]) - 1];

					if (!splitedData[2].empty())
						vertex.Normal = normals[Int(splitedData[2]) - 1];

					loadedShape.Vertices.push_back(vertex);
				}
				break;

			default:
				Raise(errorMessage);
			}
		}
		file.close();

		SetShape(loadedShape);
	}
	void Mesh::Release() {
		m_IsUsedIndexBuffer = false;
		m_pShape.reset();
	}

	void Mesh::UpdateVertices(const Vertices_t& vertices) {
		if (vertices.empty())
			Raise("The number of vertices is 0");

		RequireNotNull(m_pShape)->Vertices = vertices;
		m_pVertexArray->UpdateVBO(m_pShape->Vertices.size(), m_pShape->Vertices.data());
	}

	String Mesh::GetFilePath() const noexcept {
		return m_FilePath;
	}

	std::type_index Mesh::GetType() const {
		return typeid(Mesh);
	}


	const Shape& Mesh::GetShape() const noexcept {
		return *m_pShape.get();
	}

	const Vertices_t& Mesh::GetVertices() const {
		return RequireNotNull(m_pShape)->Vertices;
	}

	const Indices_t& Mesh::GetIndices() const {
		return RequireNotNull(m_pShape)->Indices;
	}

	Float3D Mesh::GetScale() const noexcept {
		return m_Scale;
	}

	Bool Mesh::IsUsedIndexBuffer() const noexcept {
		return m_IsUsedIndexBuffer;
	}

	void Mesh::SetShape(const Shape& newShape, const UsageDraw& usage) {
		SetVertices(newShape.Vertices, usage);
		SetIndices(newShape.Indices, usage);
	}

	void Mesh::SetVertices(const Vertices_t& vertices, const UsageDraw& usage) {
		if (usage != USAGE_STATICDRAW && usage != USAGE_DYNAMICDRAW)
			Raise("Non-existent usage specified");

		Vertices_t* pShapeVertices = &RequireNotNull(m_pShape)->Vertices;
		(*pShapeVertices) = vertices;

		for (Vertex& vertex : m_pShape->Vertices)
			vertex.Position *= Float4D(m_Scale, 1.f);
			
		m_pVertexArray->SetVBOData(pShapeVertices->size(), pShapeVertices->data(), usage);
	}
	void Mesh::SetIndices(const Indices_t& indices, const UsageDraw& usage) {
		if (usage != USAGE_STATICDRAW && usage != USAGE_DYNAMICDRAW)
			Raise("Non-existent usage specified");

		Indices_t* pShapeIndices = &RequireNotNull(m_pShape)->Indices;
		(*pShapeIndices) = indices;

		m_IsUsedIndexBuffer = (pShapeIndices->size() > 0);
		if (m_IsUsedIndexBuffer)
			m_pVertexArray->SetEBOData(pShapeIndices->size(), pShapeIndices->data(), usage);
	}

	void Mesh::SetColor(const Float4D& color) {
		RequireNotNull(m_pShape);
		for (Vertex& Vert : m_pShape->Vertices)
			Vert.Color = color;
	}
	void Mesh::SetScale(Float3D scale) {
		RequireNotNull(m_pShape);
		if (m_Scale == scale)
			return;

		if (scale.x == 0.f)
			scale.x = FLT_EPSILON;
		if (scale.y == 0.f)
			scale.y = FLT_EPSILON;
		if (scale.z == 0.f || std::isnan(std::fabsf(scale.z)))
			scale.z = FLT_EPSILON;

		const Float3D value = scale / m_Scale;
		for (Vertex& vertex : m_pShape->Vertices)
			vertex.Position *= Float4D(value, 1.f);
			
		m_pVertexArray->UpdateVBO(m_pShape->Vertices.size(), m_pShape->Vertices.data());
		m_Scale = scale;
	}

	void Mesh::Bind() const noexcept {
		m_pVertexArray->Bind();
	}

	Mesh& Mesh::operator = (const Shape& shape) {
		SetShape(shape);
		return *(this);
	}
}