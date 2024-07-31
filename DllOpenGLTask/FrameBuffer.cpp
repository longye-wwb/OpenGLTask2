#include "FrameBuffer.h"
namespace openGLTask {
	CFrameBuffer::CFrameBuffer()
		:m_FrameBufferID(0)
	{
		glGenFramebuffers(1, &m_FrameBufferID);
	}

	CFrameBuffer::~CFrameBuffer()
	{
		glDeleteFramebuffers(1, &m_FrameBufferID);
	}
	void CFrameBuffer::bind(GLenum vTarget) const
	{
		glBindFramebuffer(vTarget, m_FrameBufferID);
	}

	void CFrameBuffer::unbind(GLenum vTarget) const
	{
		glBindFramebuffer(vTarget, 0);
	}
	void CFrameBuffer::bindTexture(GLuint vBufferTarget)
	{
		glGenTextures(1, &vBufferTarget);
		glBindTexture(GL_TEXTURE_2D, vBufferTarget);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, m_Width, m_Height, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, vBufferTarget, 0);
	}
	void CFrameBuffer::bindDepthTexture(GLuint vBufferTarget)
	{
		glGenTextures(1, &vBufferTarget);
		glBindTexture(GL_TEXTURE_2D, vBufferTarget);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_Width, m_Height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, vBufferTarget, 0);
	}
	void CFrameBuffer::setAttachment(GLenum vAttachmentType, const std::shared_ptr<CTexture2D>& vTexture2D, GLint vTextureLevel)
	{
		glFramebufferTexture2D(GL_FRAMEBUFFER, vAttachmentType, GL_TEXTURE_2D, vTexture2D->getID(), vTextureLevel);
		m_TexturesMap[vAttachmentType] = vTexture2D;
		m_Width = vTexture2D->getWidth();
		m_Height = vTexture2D->getHeight();
	}
	const std::shared_ptr<CTexture2D>& CFrameBuffer::getAttachment(GLenum vAttachmentType)
	{
		return m_TexturesMap[vAttachmentType];
	}
}