#include <GL/GLEW.h>
#include <fstream>

#include <Nt/Core/Defines.h>
#include <Nt/Core/NtTypes.h>
#include <Nt/Core/String.h>
#include <Nt/Core/Utilities.h>
#include <Nt/Core/Log.h>

#include <Nt/Core/Math/Vectors.h>
#include <Nt/Core/Math/Rect.h>
#include <Nt/Core/Math/Matrix3x3.h>
#include <Nt/Core/Math/Matrix4x4.h>

#include <Nt/Core/Serialization.h>

#include <Nt/Graphics/Geometry.h>
#include <Nt/Graphics/VertexArray.h>

namespace Nt {
	VertexArray::VertexArray() noexcept {
		_Create();
	}
	VertexArray::VertexArray(const VertexArray& Array) noexcept {
		_Create();
		Array.CopyTo(this);
	}
	VertexArray::~VertexArray() noexcept {
		glDeleteVertexArrays(1, &m_VAO);
		glDeleteBuffers(1, &m_VBO);
		glDeleteBuffers(1, &m_EBO);
	}

	void VertexArray::CopyTo(VertexArray* pVertexArray) const {
		if (pVertexArray == nullptr)
			Raise("pVertexArray is nullptr");

		const uInt NumVertices = GetNumVertices();
		const uInt NumIndices = GetNumIndices();

		Vertex* Vertices = new Vertex[NumVertices];
		Index_t* Indices = new Index_t[NumIndices];

		memcpy(Vertices, m_VBOData, NumVertices * sizeof(Vertex));
		memcpy(Indices, m_EBOData, NumIndices * sizeof(Index_t));

		pVertexArray->SetVBOData(NumVertices, Vertices, GetVBOUsage());
		pVertexArray->SetEBOData(NumIndices, Indices, GetEBOUsage());
	}

	void VertexArray::UpdateVBO(const uInt& NumVertices, Vertex* pData) {
		if (m_VAO == 0)
			Raise("VAO is not created");
		if (m_VBO == 0)
			Raise("VBO is not created");

		glBindVertexArray(m_VAO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertex) * NumVertices, pData);
	}
	void VertexArray::UpdateEBO(const uInt& NumIndices, Vertex* pData) {
		if (m_VAO == 0)
			Raise("VAO is not created");
		if (m_EBO == 0)
			Raise("EBO is not created");

		glBindVertexArray(m_VAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(Vertex) * NumIndices, pData);
	}

	void VertexArray::SetVBOData(const uInt& NumVertices, Vertex* pData, const uInt& Usage) noexcept {
		m_VBOUsage = Usage;
		m_NumVertices = NumVertices;
		m_VBOData = pData;
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * m_NumVertices, m_VBOData, Usage);
	}

	void VertexArray::SetEBOData(const uInt& NumIndices, Index_t* pData, const uInt& Usage) noexcept {
		m_EBOUsage = Usage;
		m_NumIndices = NumIndices;
		m_EBOData = pData;
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Index_t) * m_NumIndices, m_EBOData, Usage);
	}

	void VertexArray::Set() const noexcept {
		glBindVertexArray(m_VAO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	}

	Vertex* VertexArray::GetVBOData() const noexcept {
		return reinterpret_cast<Vertex*>(m_VBOData);
	}
	Index_t* VertexArray::GetEBOData() const noexcept {
		return reinterpret_cast<Index_t*>(m_EBOData);
	}

	uInt VertexArray::GetNumVertices() const noexcept {
		return m_NumVertices;
	}
	uInt VertexArray::GetNumIndices() const noexcept {
		return m_NumIndices;
	}

	uInt VertexArray::GetVBOUsage() const noexcept {
		return m_VBOUsage;
	}
	uInt VertexArray::GetEBOUsage() const noexcept {
		return m_EBOUsage;
	}

	void VertexArray::_Create() {
		glGenVertexArrays(1, &m_VAO);
		glBindVertexArray(m_VAO);
		if (m_VAO == 0)
			Raise("Failed to create VAO");

		m_NumVertices = 0;
		glGenBuffers(1, &m_VBO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		if (m_VBO == 0) {
			glDeleteVertexArrays(1, &m_VAO);
			Raise("Failed to create VBO");
		}

		m_NumIndices = 0;
		glGenBuffers(1, &m_EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
		if (m_EBO == 0) {
			glDeleteVertexArrays(1, &m_VAO);
			glDeleteBuffers(1, &m_VBO);
			Raise("Failed to create EBO");
		}

		uInt Offset = 0;
		for (uInt i = 0; i < 4; ++i) {
			glEnableVertexAttribArray(i);

			const bool IsTexCoord = (i == 2);
			const uInt Size = (IsTexCoord) ? 3 : 4;
			glVertexAttribPointer(i, Size, GL_FLOAT, GL_FALSE, sizeof(Vertex), (LPCVOID)(Offset));

			Offset += Size * sizeof(float);
		}
	}
}