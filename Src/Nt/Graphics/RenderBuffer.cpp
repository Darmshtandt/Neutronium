// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <Nt/Graphics/RenderBuffer.h>
#include <Nt/Core/Log.h>

#include <GL/GLEW.h>
#include <GL/GL.h>

namespace Nt {
	RenderBuffer::RenderBuffer(RenderBuffer&& buffer) noexcept :
		m_ID(buffer.m_ID)
	{
		buffer.m_ID = 0;
	}

	RenderBuffer::~RenderBuffer() noexcept {
		Delete();
	}

	void RenderBuffer::Create() {
		if (m_ID != 0) {
			Log::Instance().Warning("Framebuffer already created");
			return;
		}

		glGenRenderbuffers(1, &m_ID);
		Assert(m_ID != 0, "Failed to create");
	}

	void RenderBuffer::Delete() noexcept {
		if (m_ID != 0) {
			glDeleteRenderbuffers(1, &m_ID);
			m_ID = 0;
		}
	}

	void RenderBuffer::Bind() const noexcept {
		glBindRenderbuffer(GL_RENDERBUFFER, m_ID);
	}
	void RenderBuffer::Unbind() const noexcept {
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
	}

	void RenderBuffer::StorageMultisample(const uInt& samples, const InternalFormat& format, const uInt2D& size) const noexcept {
		Bind();
		glRenderbufferStorageMultisample(
			GL_RENDERBUFFER, samples, uInt(format), size.x, size.y);
	}

	uInt RenderBuffer::GetID() const noexcept {
		return m_ID;
	}
}
