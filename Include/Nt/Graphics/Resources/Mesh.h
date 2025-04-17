#pragma once

#include <fstream>

#include <Nt/Graphics/Geometry/Shape.h>
#include <Nt/Graphics/VertexArray.h>
#include <Nt/Graphics/Resources/IResource.h>

namespace Nt {
	class Mesh : public IResource {
	public:
		NT_API Mesh();
		NT_API Mesh(const Nt::String& fileName);
		NT_API Mesh(const Shape& newShape, const UsageDraw& usage = USAGE_DYNAMICDRAW);
		NT_API Mesh(const Mesh& newMesh);
		NT_API Mesh(Mesh&& otherMesh);
		NT_API ~Mesh() override;

		NT_API void Write(std::ostream& Stream) const override;
		NT_API void Read(std::istream& Stream) override;

		_NODISCARD _CONSTEXPR20 uInt Sizeof() const noexcept override {
			return sizeof(*this);
		}		

		NT_API void LoadFromFile(const Nt::String& filePath) override;
		NT_API void Release() override;

		NT_API void UpdateVertices(const Vertices_t& vertices);

		NT_API _NODISCARD String GetFilePath() const noexcept override;
		NT_API _NODISCARD std::type_index GetType() const;

		NT_API _NODISCARD const Shape& GetShape() const noexcept;
		NT_API _NODISCARD const Vertices_t& GetVertices() const;
		NT_API _NODISCARD const Indices_t& GetIndices() const;

		NT_API _NODISCARD Float3D GetScale() const noexcept;
		
		NT_API _NODISCARD Bool IsUsedIndexBuffer() const noexcept;

		NT_API void SetShape(const Shape& newShape, const UsageDraw& usage = USAGE_DYNAMICDRAW);
		NT_API void SetVertices(const Vertices_t& vertices, const UsageDraw& usage = USAGE_DYNAMICDRAW);
		NT_API void SetIndices(const Indices_t& Indices, const UsageDraw& Usage = USAGE_DYNAMICDRAW);

		NT_API void SetColor(const Float4D& Color);
		NT_API void SetScale(Float3D scale);

		NT_API void Bind() const noexcept;

		NT_API Mesh& operator = (const Shape& shape);

	private:
		std::string m_FilePath;
		std::unique_ptr<VertexArray> m_pVertexArray;
		std::unique_ptr<Shape> m_pShape;
		Float3D m_Scale = { 1.f, 1.f, 1.f };
		Bool m_IsUsedIndexBuffer = false;
	};
}