#include "FrameBuffer.h"
#include "pch.h"
namespace openGLTask {
	CFrameBuffer::CFrameBuffer()
		:m_FrameBufferID(0), m_Width(0), m_Height(0), m_TexturesMap{}, m_RenderBuffersMap{}
	{
	}

	CFrameBuffer::~CFrameBuffer()
	{
		glDeleteFramebuffers(1, &m_FrameBufferID);
	}

	void CFrameBuffer::create()
	{
		glGenFramebuffers(1, &m_FrameBufferID);
	}
	void CFrameBuffer::bind(GLenum vTarget) const
	{
		glBindFramebuffer(vTarget, m_FrameBufferID);
	}

	void CFrameBuffer::unbind(GLenum vTarget) const
	{
		glBindFramebuffer(vTarget, 0);
	}

	void CFrameBuffer::setAttachment(GLenum vAttachmentType, const std::shared_ptr<CTexture2D>& vTexture2D, GLint vTextureLevel)
	{
		glFramebufferTexture2D(GL_FRAMEBUFFER, vAttachmentType, GL_TEXTURE_2D, vTexture2D->getID(), vTextureLevel);
		m_TexturesMap[vAttachmentType] = vTexture2D;
		m_Width = vTexture2D->getWidth();
		m_Height = vTexture2D->getHeight();
	}

	void CFrameBuffer::setAttachment(GLenum vAttachmentType, const std::shared_ptr<CRenderBuffer>& vRenderBuffer)
	{
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, vAttachmentType, GL_RENDERBUFFER, vRenderBuffer->getID());
		m_RenderBuffersMap[vAttachmentType] = vRenderBuffer;
		m_Width = vRenderBuffer->getWidth();
		m_Height = vRenderBuffer->getHeight();
	}

	const std::shared_ptr<CTexture2D>& CFrameBuffer::getAttachment(GLenum vAttachmentType)
	{
		return m_TexturesMap[vAttachmentType];
	}

	void CFrameBuffer::drawAttachments(const std::vector<GLenum>& vAttachmentsType)
	{
		glDrawBuffers((GLsizei)vAttachmentsType.size(), vAttachmentsType.data());
	}

	void CFrameBuffer::updateFrameBuffer(int vWidth, int vHeight)
	{
		m_Width = vWidth;
		m_Height = vHeight;
		for (const auto& Pair : m_TexturesMap)
			Pair.second->updateTexture2D(vWidth, vHeight, nullptr);
		for (const auto& Pair : m_RenderBuffersMap)
			Pair.second->updateRenderBuffer(vWidth, vHeight);
	}

	bool CFrameBuffer::checkComplete()
	{
		bool IsComplete = true;
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			IsComplete = false;
			HIVE_LOG_ERROR("Framebuffer is not complete!");
		}
		return IsComplete;
	}
	const std::shared_ptr<CFrameBuffer>& CFrameBuffer::getDefaultFrameBuffer()
	{
		static std::shared_ptr<CFrameBuffer> pDefaultFrameBuffer = std::make_shared<CFrameBuffer>();
		return pDefaultFrameBuffer;
	}

	void CFrameBuffer::initDefaultFrameBuffer(int vWidth, int vHeight, GLuint vFrameBufferID)
	{
		getDefaultFrameBuffer()->m_Width = vWidth;
		getDefaultFrameBuffer()->m_Height = vHeight;
		getDefaultFrameBuffer()->m_FrameBufferID = vFrameBufferID;
	}
}