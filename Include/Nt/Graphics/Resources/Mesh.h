#pragma once

#include <Nt/Graphics/Geometry/Shape.h>
#include <Nt/Graphics/VertexArray.h>
#include <Nt/Graphics/Resources/IResource.h>

#pragma warning(push)
#pragma warning(disable: 4251)

namespace Nt {
	class NT_API Mesh final : public IResource {
	public:
		Mesh(const String& fileName);
		Mesh(const Shape& shape, const UsageDraw& usage = USAGE_DYNAMICDRAW);

		Mesh() = default;
		Mesh(const Mesh& mesh) noexcept = default;
		Mesh(Mesh&& other) noexcept = default;
		~Mesh() override;

		void LoadFromFile(const String& filePath) override;
		void Release() override;

		void UpdateVertices(const Vertices_t& vertices);

		NT_NODISCARD String GetFilePath() const noexcept override;
		NT_NODISCARD std::type_index GetType() const override;

		NT_NODISCARD const Shape& GetShape() const noexcept;
		NT_NODISCARD const Vertices_t& GetVertices() const noexcept;
		NT_NODISCARD const Indices_t& GetIndices() const noexcept;

		NT_NODISCARD Float3D GetScale() const noexcept;		
		NT_NODISCARD uInt GetVerticesCount() const noexcept;

		void SetShape(const Shape& newShape, const UsageDraw& usage = USAGE_DYNAMICDRAW);
		void SetVertices(const Vertices_t& vertices, const UsageDraw& usage = USAGE_DYNAMICDRAW);
		void SetIndices(const Indices_t& indices, const UsageDraw& usage = USAGE_DYNAMICDRAW);

		void SetColor(const Float4D& color) noexcept;
		void SetScale(Float3D scale);

		void Bind() const noexcept;

		Mesh& operator = (const Shape& shape);
		Mesh& operator = (const Mesh& mesh) noexcept = default;
		Mesh& operator = (Mesh&& mesh) noexcept = default;

	private:
		VertexArray m_VertexArray;
		Float3D m_Scale = Float3D(1.f, 1.f, 1.f);
		std::string m_FilePath;
		Shape m_Shape;
	};
}

#pragma warning(pop)