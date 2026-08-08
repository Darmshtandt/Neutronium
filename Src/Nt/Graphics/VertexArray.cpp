// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <GL/GLEW.h>
#include <Nt/Graphics/VertexArray.h>

namespace Nt {
	VertexArray::VertexArray() :
		m_VertexBuffer(Buffer::ARRAY),
		m_ElementBuffer(Buffer::ELEMENT_ARRAY)
	{
		_Create();
	}

	VertexArray::VertexArray(const VertexArray& other) : 
		m_VertexBuffer(other.m_VertexBuffer),
		m_ElementBuffer(other.m_ElementBuffer)
	{
		_Create();
	}

	VertexArray::~VertexArray() noexcept {
		if (m_ID != 0)
			glDeleteVertexArrays(1, &m_ID);
	}

	void VertexArray::Clear() noexcept {
		m_VertexBuffer.Clear();
		m_ElementBuffer.Clear();
	}

	void VertexArray::UpdateVBO(const uInt& numVertices, Vertex* pData) const {
		glBindVertexArray(m_ID);

		m_VertexBuffer.Bind();
		m_VertexBuffer.SetSubData(0, sizeof(Vertex) * numVertices, pData);
	}
	void VertexArray::UpdateEBO(const uInt& numIndices, Vertex* pData) const {
		glBindVertexArray(m_ID);

		m_ElementBuffer.Bind();
		m_ElementBuffer.SetSubData(0, sizeof(Vertex) * numIndices, pData);
	}

	void VertexArray::SetVBOData(const uInt& numVertices, Vertex* pData, const UsageDraw& usage) noexcept {
		m_VertexBuffer.Bind();
		m_VertexBuffer.SetData(sizeof(Vertex) * numVertices, pData, usage);
	}

	void VertexArray::SetEBOData(const uInt& numIndices, Index_t* pData, const UsageDraw& usage) noexcept {
		m_ElementBuffer.Bind();
		m_ElementBuffer.SetData(sizeof(Index_t) * numIndices, pData, usage);
	}

	void VertexArray::Bind() const noexcept {
		glBindVertexArray(m_ID);

		m_VertexBuffer.Bind();
		m_ElementBuffer.Bind();
	}

	VertexArray& VertexArray::operator = (const VertexArray& other) noexcept {
		if (this == &other)
			return *this;

		m_VertexBuffer = other.m_VertexBuffer;
		m_ElementBuffer = other.m_ElementBuffer;

		return *this;
	}

	_NODISCARD const Buffer& VertexArray::GetVertexBuffer() const noexcept {
		return m_VertexBuffer;
	}
	_NODISCARD const Buffer& VertexArray::GetElementBuffer() const noexcept {
		return m_ElementBuffer;
	}

	_NODISCARD const Vertex* VertexArray::GetVerticesData() const noexcept {
		return static_cast<const Vertex*>(m_VertexBuffer.GetDataPtr());
	}
	_NODISCARD const Index_t* VertexArray::GetIndicesData() const noexcept {
		return static_cast<const Index_t*>(m_ElementBuffer.GetDataPtr());
	}

	_NODISCARD uInt VertexArray::GetNumVertices() const noexcept {
		return m_VertexBuffer.GetSize() / sizeof(Vertex);
	}
	_NODISCARD uInt VertexArray::GetNumIndices() const noexcept {
		return m_ElementBuffer.GetSize() / sizeof(Index_t);
	}
	_NODISCARD uInt VertexArray::GetID() const noexcept {
		return m_ID;
	}

	void VertexArray::_Create() {
		glGenVertexArrays(1, &m_ID);
		glBindVertexArray(m_ID);
		Assert(m_ID != 0, "Failed to create VertexArray");

		m_VertexBuffer.Bind();
		m_ElementBuffer.Bind();

		uInt offset = 0;
		for (uInt i = 0; i < 4; ++i) {
			glEnableVertexAttribArray(i);

			const Bool isTexCoord = (i == 2);
			const uInt size = (isTexCoord) ? 3 : 4;

			glVertexAttribPointer(i, size, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const Void*)(offset));

			offset += size * sizeof(Float);
		}
	}
}