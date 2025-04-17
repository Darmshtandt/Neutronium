// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <GL/GLEW.h>

#include <Nt/Core/Utilities.h>
#include <Nt/Graphics/Buffer.h>

namespace Nt {
	Buffer::~Buffer() {
		Delete();
	}

	void Buffer::Create(const Target& target) {
		if (m_ID != 0) {
			Log::Warning("Buffer already created");
			return;
		}

		m_Target = target;

		glGenBuffers(1, &m_ID);
		if (m_ID == 0)
			Raise("Failed to created buffer");

		Bind();
	}

	void Buffer::SetData(const uInt& size, const void* pData, const UsageDraw& usage) {
		Assert(m_ID != 0, "Buffer not created");

		m_pData = pData;
		m_DataSize = size;
		m_Usage = usage;

		Bind();
		glBufferData(m_Target, m_DataSize, m_pData, m_Usage);
	}
	void Buffer::SetSubData(const uInt& offset, const uInt& size, const void* pData) {
		Assert(m_ID != 0, "Buffer not created");
		Assert((offset + size > m_DataSize), "Out of range");

		Bind();
		glBufferSubData(m_Target, offset, size, pData);
	}

	void* Buffer::Map(const Access& access) const {
		Assert(m_ID != 0, "Buffer not created");

		Bind();
		return glMapBuffer(m_Target, uInt(access));
	}
	void Buffer::Unmap() const {
		Assert(m_ID != 0, "Buffer not created");

		Bind();
		glUnmapBuffer(m_Target);
	}

	void Buffer::Bind() const {
		Assert(m_ID != 0, "Buffer not created");

		glBindBuffer(m_Target, m_ID);
	}
	void Buffer::Unbind() const noexcept {
		glBindBuffer(m_Target, 0);
	}

	void Buffer::Delete() {
		if (m_ID == 0) {
			Log::Warning("Buffer not created");
			return;
		}

		glDeleteBuffers(1, &m_ID);

		m_ID = 0;
		m_pData = nullptr;
		m_DataSize = 0;
	}
		
	_NODISCARD UsageDraw Buffer::GetUsage() const noexcept {
		return m_Usage;
	}
	_NODISCARD Buffer::Target Buffer::GetTarget() const noexcept {
		return m_Target;
	}
	_NODISCARD const Void* Buffer::GetDataPtr() const noexcept {
		return m_pData;
	}
	_NODISCARD uInt Buffer::GetDataSize() const noexcept {
		return m_DataSize;
	}
	_NODISCARD uInt Buffer::GetID() const noexcept {
		return m_ID;
	}
	_NODISCARD Bool Buffer::IsCreated() const noexcept {
		return (m_ID != 0);
	}
}