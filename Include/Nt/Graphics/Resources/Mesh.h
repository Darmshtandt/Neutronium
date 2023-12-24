#pragma once

namespace Nt {
	class Mesh : public IResource {
	public:
		Mesh() :
			IResource(IResource::TYPE_MESH) {
			_Initialize();
		}
		Mesh(LPCSTR FileName) :
			IResource(IResource::TYPE_MESH) {
			_Initialize();
			LoadFromFile(FileName);
		}
		Mesh(const Mesh& NewMesh) :
			IResource(NewMesh),
			m_IsUsedIndexBuffer(NewMesh.m_IsUsedIndexBuffer),
			m_Scale(NewMesh.m_Scale),
			m_pVertexArray(std::make_unique<VertexArray>(*NewMesh.m_pVertexArray))
		{
			m_pData = new Shape(NewMesh.GetShape());
		}
		Mesh(const Shape& NewShape, const uInt& Usage = USAGE_DYNAMICDRAW) :
			IResource(IResource::TYPE_MESH) {
			_Initialize();
			m_pVertexArray = std::make_unique<VertexArray>();
			SetShape(NewShape, Usage);
		}
		~Mesh() override {
			Release();
		}

		void Write(std::ostream& Stream) const override {
			IResource::Write(Stream);
			if (strlen(GetFilePath()) == 0) {
				Shape shape = { };
				if (m_pData)
					shape = *static_cast<Shape*>(m_pData);
				Serialization::WriteAll(Stream, shape);
			}
		}
		void Read(std::istream& Stream) override {
			IResource::Read(Stream);
			if (strlen(GetFilePath()) == 0) {
				Shape shape;
				Serialization::ReadAll(Stream, shape);
				SetShape(shape);
			}
		}
		constexpr uInt Sizeof() const noexcept override {
			return sizeof(*this);
		}


		const Shape& GetShape() const {
			if (!m_pData)
				Raise("Data is nullptr");
			return *static_cast<Shape*>(m_pData);
		}
		const Vertices_t& GetVertices() const {
			if (!m_pData)
				Raise("Data is nullptr");
			return static_cast<Shape*>(m_pData)->Vertices;
		}
		const Indices_t& GetIndices() const {
			if (!m_pData)
				Raise("Data is nullptr");
			return static_cast<Shape*>(m_pData)->Indices;
		}
		Float3D GetScale() const noexcept {
			return m_Scale;
		}
		Bool IsUsedIndexBuffer() const noexcept {
			return m_IsUsedIndexBuffer;
		}

		void SetShape(const Shape& NewShape, const uInt& Usage = USAGE_DYNAMICDRAW) {
			m_Usage = Usage;
			SetVertices(NewShape.Vertices, m_Usage);
			SetIndices(NewShape.Indices, m_Usage);
		}

		void SetVertices(const Vertices_t& Vertices, const uInt& Usage = USAGE_DYNAMICDRAW) {
			if (Vertices.size() == 0)
				Raise("The number of vertices is 0");
			if (Usage != USAGE_STATICDRAW && Usage != USAGE_DYNAMICDRAW)
				Raise("Non-existent usage specified");

			Shape* pShape = static_cast<Shape*>(m_pData);
			pShape->Vertices = Vertices;
			for (Vertex& vertex : pShape->Vertices)
				vertex.Position *= m_Scale;
			m_pVertexArray->SetVBOData(pShape->Vertices.size(), pShape->Vertices.data(), Usage);
		}
		void SetIndices(const Indices_t& Indices, const uInt& Usage = USAGE_DYNAMICDRAW) {
			if (Usage != USAGE_STATICDRAW && Usage != USAGE_DYNAMICDRAW)
				Raise("Non-existent usage specified");

			Shape* pShape = static_cast<Shape*>(m_pData);
			pShape->Indices = Indices;

			m_IsUsedIndexBuffer = (pShape->Indices.size() > 0);
			if (m_IsUsedIndexBuffer)
				m_pVertexArray->SetEBOData(pShape->Indices.size(), pShape->Indices.data(), Usage);
		}

		void SetColor(const Float4D& Color) {
			for (Vertex& Vert : static_cast<Shape*>(m_pData)->Vertices)
				Vert.Color = Color;
		}
		void SetScale(Float3D scale) {
			if (m_Scale == scale)
				return;

			if (scale.x == 0.f)
				scale.x = 0.001f;
			if (scale.y == 0.f)
				scale.y = 0.001f;
			if (scale.z == 0.f)
				scale.z = 0.001f;

			const Float3D value = scale / m_Scale;

			Shape* pShape = static_cast<Shape*>(m_pData);
			for (Vertex& vertex : pShape->Vertices)
				vertex.Position *= value;

			m_pVertexArray->UpdateVBO(pShape->Vertices.size(), pShape->Vertices.data());
			m_Scale = scale;
		}

		void Set() const noexcept {
			m_pVertexArray->Set();
		}

		Mesh& operator = (const Shape& shape) {
			SetShape(shape);
			return *(this);
		}

	private:
		std::unique_ptr<VertexArray> m_pVertexArray;
		Float3D m_Scale;
		uInt m_Usage;
		Bool m_IsUsedIndexBuffer;

	private:
		void _Initialize() {
			m_IsUsedIndexBuffer = false;
			m_Scale = { 1.f, 1.f, 1.f };
			m_pVertexArray.release();
			m_pVertexArray = std::make_unique<VertexArray>();
			m_pData = new Shape;
		}

		void _LoadFromFile() override {
			std::vector<Float4D> positions;
			std::vector<Float3D> texCoords;
			std::vector<Float3D> normals;

			std::ifstream file(GetFilePath());
			Shape loadedShape;

			String errorMessage = "Failed to load model.\nLine: ";
			uInt LineNumber = 0;

			String line;
			while (std::getline(file, line)) {
				++LineNumber;
				if (line == "" || line == "\r")
					continue;

				errorMessage = "Failed to load model.\nLine: ";
				errorMessage += LineNumber;

				std::vector<String> splitedString = line.Split(' ');
				const std::string type = splitedString[0];

				if (type == "mtllib")
					continue;
				else if (type == "#")
					continue;
				else if (type == "usemtl")
					continue;

				String x, y, z, w;
				if (type[0] != 'f') {
					if (splitedString.size() > 1) {
						x = splitedString[1];
						if (splitedString.size() > 2) {
							y = splitedString[2];
							if (splitedString.size() > 3) {
								z = splitedString[3];
								if (splitedString.size() > 4)
									w = splitedString[4];
							}
						}
					}
				}

				Float4D data;

				switch (type[0]) {
				case 's':
				case 'g':
				case 'o':
				case '#':
					continue;
				case 'v':
					if (type == "v") {
						const Bool isX_Incorrect = (x.length() != 0 && (!x.IsFloat()));
						const Bool isY_Incorrect = (y.length() != 0 && (!y.IsFloat()));
						const Bool isZ_Incorrect = (z.length() != 0 && (!z.IsFloat()));
						const Bool isW_Incorrect = (w.length() != 0 && (!w.IsFloat()));
						if (isX_Incorrect || isY_Incorrect || isZ_Incorrect || isW_Incorrect)
							Raise(errorMessage);

						data.x = x;
						data.y = y;
						data.z = z;

						if (w.length() > 0)
							data.w = w;
						else
							data.w = 1.f;

						positions.push_back(data);
					}
					else if (type == "vt") {
						const Bool isU_Incorrect = (!x.IsFloat());
						const Bool isV_Incorrect = (!y.IsFloat());
						const Bool isW_Incorrect = (z.length() != 0 && (!z.IsFloat()));
						if (isU_Incorrect || isV_Incorrect || isW_Incorrect || w.length() > 0)
							Raise(errorMessage);

						data.x = x;
						data.y = y;

						if (w.IsFloat())
							data.z = z;
						else
							data.z = 0.f;

						texCoords.push_back(data);
					}
					else if (type == "vn") {
						const Bool isX_Incorrect = (x.length() != 0 && (!x.IsFloat()));
						const Bool isY_Incorrect = (y.length() != 0 && (!y.IsFloat()));
						const Bool isZ_Incorrect = (z.length() != 0 && (!z.IsFloat()));
						if (isX_Incorrect || isY_Incorrect || isZ_Incorrect || w.length() > 0)
							Raise(errorMessage);

						data.x = x;
						data.y = y;
						data.z = z;
						normals.push_back(data);
					}
					else if (type == "vp") {

					}
					else {
						Raise(errorMessage);
					}
					break;
				case 'f': {
					for (uInt i = 1; i < splitedString.size(); ++i) {
						std::vector<String> splitedData = splitedString[i].Split('/');

						if (splitedData.size() <= 1)
							Raise(errorMessage);

						Int vertexIndex = -1;
						vertexIndex = stoi(splitedData[0]) - 1;

						Int texCoordIndex = -1;
						if (splitedData[1].length() > 0)
							texCoordIndex = stoi(splitedData[1]) - 1;

						Int NormalIndex = -1;
						if (splitedData[2].length() > 0)
							NormalIndex = stoi(splitedData[2]) - 1;

						Vertex vertex = { };
						vertex.Position = positions[vertexIndex];
						if (texCoordIndex != -1)
							vertex.TexCoord = texCoords[texCoordIndex];
						if (NormalIndex != -1)
							vertex.Normal = normals[NormalIndex];
						vertex.Color = Colors::White;

						loadedShape.Vertices.push_back(vertex);
					}
				}
					break;
				default:
					Raise(errorMessage);
				}
			}
			file.close();

			SetShape(loadedShape);
		}

		void _Release() override {
			m_IsUsedIndexBuffer = false;
		}
	};
}