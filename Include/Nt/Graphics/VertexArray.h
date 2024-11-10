#pragma once

namespace Nt {
	enum eUsageDraw {
		USAGE_STREAMDRAW = GL_STREAM_DRAW,
		USAGE_STREAMREAD = GL_STREAM_READ,
		USAGE_STREAMCOPY = GL_STREAM_COPY,

		USAGE_STATICDRAW = GL_STATIC_DRAW,
		USAGE_STATICREAD = GL_STATIC_READ,
		USAGE_STATICCOPY = GL_STATIC_COPY,

		USAGE_DYNAMICDRAW = GL_DYNAMIC_DRAW,
		USAGE_DYNAMICREAD = GL_DYNAMIC_READ,
		USAGE_DYNAMICCOPY = GL_DYNAMIC_COPY
	};

	class VertexArray {
	public:
		NT_API VertexArray() noexcept;
		NT_API VertexArray(const VertexArray& Array) noexcept;
		NT_API ~VertexArray() noexcept;

		NT_API void CopyTo(VertexArray* pVertexArray) const;

		NT_API void UpdateVBO(const uInt& NumVertices, Vertex* pData);
		NT_API void UpdateEBO(const uInt& NumIndices, Vertex* pData);

		NT_API void SetVBOData(const uInt& NumVertices, Vertex* pData, const uInt& Usage = USAGE_DYNAMICDRAW) noexcept;
		NT_API void SetEBOData(const uInt& NumIndices, Index_t* pData, const uInt& Usage = USAGE_DYNAMICDRAW) noexcept;

		NT_API void Set() const noexcept;

		NT_API Vertex* GetVBOData() const noexcept;
		NT_API Index_t* GetEBOData() const noexcept;

		NT_API uInt GetNumVertices() const noexcept;
		NT_API uInt GetNumIndices() const noexcept;

		NT_API uInt GetVBOUsage() const noexcept;
		NT_API uInt GetEBOUsage() const noexcept;

	private:
		void* m_VBOData;
		void* m_EBOData;

		uInt m_VAO;
		uInt m_VBO;
		uInt m_EBO;

		uInt m_VBOUsage;
		uInt m_EBOUsage;

		uInt m_NumVertices;
		uInt m_NumIndices;

	private:
		NT_API void _Create();
	};
}