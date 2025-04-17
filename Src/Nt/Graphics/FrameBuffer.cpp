// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <Nt/Graphics/FrameBuffer.h>
#include <Nt/Core/Log.h>

#include <GL/GLEW.h>
#include <GL/GL.h>

namespace Nt {
	FrameBuffer::FrameBuffer(FrameBuffer&& buffer) noexcept :
		m_ID(buffer.m_ID),
		m_TextureID(buffer.m_TextureID)
	{
		buffer.m_ID = buffer.m_TextureID = 0;
	}
	FrameBuffer::~FrameBuffer() {
		Delete();
	}

	void FrameBuffer::Create() {
		if (m_ID != 0) {
			Log::Warning("Framebuffer already created");
			return;
		}

		glGenFramebuffers(1, &m_ID);
		Bind(Type::DEFAULT);
	}

	void FrameBuffer::Bind(const Type& type) const {
		Assert(m_ID != 0, "Framebuffer not created");

		glBindFramebuffer(uInt(type), m_ID);
	}
	void FrameBuffer::Unbind(const Type& type) const noexcept {
		glBindFramebuffer(uInt(type), 0);
	}

	void FrameBuffer::Blit(const uIntRect& sourceRect, const uIntRect& destinationRect, const BufferBit& bufferBit, const MagFilter& filter) const noexcept {
		glBlitFramebuffer(
			sourceRect.Left, sourceRect.Top, sourceRect.Right, sourceRect.Bottom,
			destinationRect.Left, destinationRect.Top, destinationRect.Right, destinationRect.Bottom,
			uInt(bufferBit), uInt(filter));
	}

	void FrameBuffer::Invalidate(const Type& type, const uInt& attachment) {
		glInvalidateFramebuffer(uInt(type), 1, &attachment);
	}

	void FrameBuffer::SetRenderBuffer(const RenderBuffer& buffer, const uInt& attachment) const {
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, buffer.GetID());
	}

	void FrameBuffer::SetTexture2D(const Texture* pTexture, const uInt& attachment, const uInt& level) {
		Assert(RequireNotNull(pTexture)->GetID() != 0, "Texture not created");

		glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, pTexture->GetID(), level);
		Assert(GetStatus() != Status::COMPLETE, "Failed to set texture to framebuffer");
	}

	void FrameBuffer::Delete() {
		if (m_ID != 0) {
			glDeleteFramebuffers(1, &m_ID);
			m_ID = 0;
		}
	}

	FrameBuffer::Status FrameBuffer::GetStatus() noexcept {
		return Status(glCheckFramebufferStatus(GL_FRAMEBUFFER));
	}

	uInt FrameBuffer::GetID() const noexcept {
		return m_ID;
	}
	Bool FrameBuffer::IsCreated() const noexcept {
		return (m_ID != 0);
	}
}