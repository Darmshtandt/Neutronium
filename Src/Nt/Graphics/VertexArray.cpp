// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <GL/GLEW.h>
#include <fstream>

#include <Nt/Core/Utilities.h>
#include <Nt/Core/Log.h>

#include <Nt/Core/Math/Rect.h>
#include <Nt/Core/Math/Matrix3x3.h>
#include <Nt/Core/Math/Matrix4x4.h>

#include <Nt/Core/Serialization.h>

#include <Nt/Graphics/Buffer.h>
#include <Nt/Graphics/VertexArray.h>

namespace Nt {
	VertexArray::VertexArray() :
		m_VertexBuffer(),
		m_ElementBuffer(),
		m_ID(0)
	{
		_Create();
	}

	VertexArray::VertexArray(const VertexArray& array) noexcept {
		_Create();

		array.CopyTo(this);
	}

	VertexArray::~VertexArray() noexcept {
		glDeleteVertexArrays(1, &m_ID);
	}

	void VertexArray::CopyTo(VertexArray* pVertexArray) const {
		RequireNotNull(pVertexArray);

		const uInt numVertices = m_VertexBuffer.GetDataSize() / sizeof(Vertex);
		if (numVertices == 0)
			return;

		Vertex* vertices = new Vertex[numVertices];

		memcpy(vertices, m_VertexBuffer.GetDataPtr(), numVertices * sizeof(Vertex));
		pVertexArray->SetVBOData(numVertices, vertices, m_VertexBuffer.GetUsage());

		const uInt numIndices = m_ElementBuffer.GetDataSize() / sizeof(Index_t);
		if (numIndices > 0) {
			Index_t* indices = new Index_t[numIndices];

			memcpy(indices, m_ElementBuffer.GetDataPtr(), numIndices * sizeof(Index_t));
			pVertexArray->SetEBOData(numIndices, indices, m_ElementBuffer.GetUsage());

			delete[] indices;
		}

		delete[] vertices;
	}

	void VertexArray::UpdateVBO(const uInt& numVertices, Vertex* pData) {
		if (m_ID == 0)
			Raise("VAO not created");
		if (!m_VertexBuffer.IsCreated())
			Raise("VBO not created");

		glBindVertexArray(m_ID);

		m_VertexBuffer.Bind();
		m_VertexBuffer.SetSubData(0, sizeof(Vertex) * numVertices, pData);
	}
	void VertexArray::UpdateEBO(const uInt& numIndices, Vertex* pData) {
		if (m_ID == 0)
			Raise("VAO not created");
		if (!m_ElementBuffer.IsCreated())
			Raise("EBO not created");

		glBindVertexArray(m_ID);

		m_ElementBuffer.Bind();
		m_ElementBuffer.SetSubData(0, sizeof(Vertex) * numIndices, pData);
	}

	void VertexArray::SetVBOData(const uInt& numVertices, const Vertex* pData, const UsageDraw& usage) noexcept {
		m_VertexBuffer.Bind();
		m_VertexBuffer.SetData(sizeof(Vertex) * numVertices, pData, usage);
	}

	void VertexArray::SetEBOData(const uInt& numIndices, const Index_t* pData, const UsageDraw& usage) noexcept {
		m_ElementBuffer.Bind();
		m_ElementBuffer.SetData(sizeof(Index_t) * numIndices, pData, usage);
	}

	void VertexArray::Bind() const noexcept {
		glBindVertexArray(m_ID);

		m_VertexBuffer.Bind();
		m_ElementBuffer.Bind();
	}

	const Buffer& VertexArray::GetVertexBuffer() const noexcept {
		return m_VertexBuffer;
	}
	const Buffer& VertexArray::GetElementBuffer() const noexcept {
		return m_ElementBuffer;
	}

	const Vertex* const VertexArray::GetVerticesData() const noexcept {
		return reinterpret_cast<const Vertex* const>(m_VertexBuffer.GetDataPtr());
	}
	const Index_t* const VertexArray::GetIndicesData() const noexcept {
		return reinterpret_cast<const Index_t* const>(m_ElementBuffer.GetDataPtr());
	}

	uInt VertexArray::GetNumVertices() const noexcept {
		return m_VertexBuffer.GetDataSize() / sizeof(Vertex);
	}
	uInt VertexArray::GetNumIndices() const noexcept {
		return m_ElementBuffer.GetDataSize() / sizeof(Index_t);
	}
	uInt VertexArray::GetID() const noexcept {
		return m_ID;
	}

	void VertexArray::_Create() {
		glGenVertexArrays(1, &m_ID);
		glBindVertexArray(m_ID);

		if (m_ID == 0)
			Raise("Failed to create VAO");

		m_VertexBuffer.Create(Buffer::ARRAY);
		m_ElementBuffer.Create(Buffer::ELEMENT_ARRAY);

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