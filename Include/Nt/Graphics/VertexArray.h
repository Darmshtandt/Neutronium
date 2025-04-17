#pragma once

#include <Nt/Graphics/Geometry/Shape.h>
#include <Nt/Graphics/Buffer.h>

namespace Nt {
	class VertexArray {
	public:
		NT_API VertexArray();
		NT_API VertexArray(const VertexArray& Array) noexcept;
		NT_API ~VertexArray() noexcept;

		NT_API void CopyTo(VertexArray* pVertexArray) const;

		NT_API void UpdateVBO(const uInt& NumVertices, Vertex* pData);
		NT_API void UpdateEBO(const uInt& NumIndices, Vertex* pData);

		NT_API void SetVBOData(const uInt& NumVertices, const Vertex* pData, const UsageDraw& Usage = USAGE_DYNAMICDRAW) noexcept;
		NT_API void SetEBOData(const uInt& NumIndices, const Index_t* pData, const UsageDraw& Usage = USAGE_DYNAMICDRAW) noexcept;

		NT_API void Bind() const noexcept;

		NT_API const Buffer& GetVertexBuffer() const noexcept;
		NT_API const Buffer& GetElementBuffer() const noexcept;

		NT_API const Vertex* const GetVerticesData() const noexcept;
		NT_API const Index_t* const GetIndicesData() const noexcept;

		NT_API uInt GetNumVertices() const noexcept;
		NT_API uInt GetNumIndices() const noexcept;
		NT_API uInt GetID() const noexcept;

	private:
		Buffer m_VertexBuffer;
		Buffer m_ElementBuffer;
		uInt m_ID;

	private:
		NT_API void _Create();
	};
}