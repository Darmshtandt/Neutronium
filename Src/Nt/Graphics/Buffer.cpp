// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <GL/GLEW.h>

#include <Nt/Core/Utilities.h>
#include <Nt/Graphics/Buffer.h>

namespace Nt {
	Buffer::Buffer(const Target& target) :
		m_Target(target)
	{
		_Create();
	}
	Buffer::Buffer(const Buffer& other) :
		m_pData(other.m_pData),
		m_Target(other.m_Target),
		m_Usage(other.m_Usage),
		m_Size(other.m_Size)
	{
		_Create();
		SetData(m_Size, m_pData, m_Usage);
	}
	Buffer::~Buffer() noexcept {
		if (m_ID != 0)
			glDeleteBuffers(1, &m_ID);
	}

	void Buffer::Clear() noexcept {
		SetData(0, nullptr, m_Usage);
	}

	void Buffer::SetData(const uInt& size, void* pData, const UsageDraw& usage) noexcept {
		if (m_pData != pData)
			m_pData = pData;
		if (m_Size != size)
			m_Size = size;
		if (m_Usage != usage)
			m_Usage = usage;

		glBindBuffer(m_Target, m_ID);
		glBufferData(m_Target, static_cast<Int>(m_Size), m_pData, m_Usage);
	}
	void Buffer::SetSubData(const uInt& offset, const uInt& size, const void* pData) const {
		Assert((offset + size <= m_Size), "Out of range");

		glBindBuffer(m_Target, m_ID);
		glBufferSubData(m_Target, static_cast<Int>(offset), static_cast<Int>(size), pData);
	}

	void* Buffer::Map(const Access& access) const noexcept {
		glBindBuffer(m_Target, m_ID);
		return glMapBuffer(m_Target, static_cast<uInt>(access));
	}
	void Buffer::Unmap() const noexcept {
		glBindBuffer(m_Target, m_ID);
		glUnmapBuffer(m_Target);
	}

	void Buffer::Bind() const noexcept {
		glBindBuffer(m_Target, m_ID);
	}
	void Buffer::Unbind() const noexcept {
		glBindBuffer(m_Target, 0);
	}

	Buffer& Buffer::operator = (const Buffer& other) noexcept {
		if (this == &other)
			return *this;

		m_pData = other.m_pData;
		m_Usage = other.m_Usage;
		m_Size = other.m_Size;

		glBindBuffer(m_Target, m_ID);
		glBufferData(m_Target, static_cast<Int>(m_Size), m_pData, m_Usage);

		return *this;
	}
		
	UsageDraw Buffer::GetUsage() const noexcept {
		return m_Usage;
	}
	Buffer::Target Buffer::GetTarget() const noexcept {
		return m_Target;
	}
	void* Buffer::GetDataPtr() const noexcept {
		return m_pData;
	}
	uInt Buffer::GetSize() const noexcept {
		return m_Size;
	}
	uInt Buffer::GetID() const noexcept {
		return m_ID;
	}

	void Buffer::_Create() {
		glGenBuffers(1, &m_ID);
		Assert(m_ID != 0, "Failed to created buffer");
	}
}