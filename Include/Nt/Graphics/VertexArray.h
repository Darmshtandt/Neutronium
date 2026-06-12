#pragma once

#include <Nt/Graphics/Geometry/Shape.h>
#include <Nt/Graphics/Buffer.h>

namespace Nt {
	class NT_API VertexArray {
	public:
		VertexArray();
		VertexArray(const VertexArray& other);
		VertexArray(VertexArray&& other) noexcept = default;
		~VertexArray() noexcept;

		void Clear() noexcept;

		void UpdateVBO(const uInt& numVertices, Vertex* pData) const;
		void UpdateEBO(const uInt& numIndices, Vertex* pData) const;

		void SetVBOData(const uInt& numVertices, Vertex* pData, const UsageDraw& usage = USAGE_DYNAMICDRAW) noexcept;
		void SetEBOData(const uInt& numIndices, Index_t* pData, const UsageDraw& usage = USAGE_DYNAMICDRAW) noexcept;

		void Bind() const noexcept;

		VertexArray& operator = (const VertexArray& other) noexcept;
		VertexArray& operator = (VertexArray&& other) noexcept = default;

		NT_NODISCARD const Buffer& GetVertexBuffer() const noexcept;
		NT_NODISCARD const Buffer& GetElementBuffer() const noexcept;

		NT_NODISCARD const Vertex* GetVerticesData() const noexcept;
		NT_NODISCARD const Index_t* GetIndicesData() const noexcept;

		NT_NODISCARD uInt GetNumVertices() const noexcept;
		NT_NODISCARD uInt GetNumIndices() const noexcept;
		NT_NODISCARD uInt GetID() const noexcept;

	private:
		Buffer m_VertexBuffer;
		Buffer m_ElementBuffer;
		uInt m_ID;

	private:
		void _Create();
	};
}