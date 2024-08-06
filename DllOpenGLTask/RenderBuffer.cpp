#include "RenderBuffer.h"

namespace openGLTask 
{
	CRenderBuffer::CRenderBuffer(int vWidth, int vHeight, GLenum vInternalformat)
		:m_RenderBufferID{ 0 }, m_Internalformat(vInternalformat), m_Width(0), m_Height(0)
	{
		glGenRenderbuffers(1, &m_RenderBufferID);
		updateRenderBuffer(vWidth, vHeight);
	}

	CRenderBuffer::~CRenderBuffer()
	{
		glDeleteRenderbuffers(1, &m_RenderBufferID);
	}

	void CRenderBuffer::updateRenderBuffer(int vWidth, int vHeight)
	{
		m_Width = vWidth;
		m_Height = vHeight;
		glBindRenderbuffer(GL_RENDERBUFFER, m_RenderBufferID);
		glRenderbufferStorage(GL_RENDERBUFFER, m_Internalformat, m_Width, m_Height);
	}
}