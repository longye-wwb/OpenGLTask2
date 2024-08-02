#include "Texture2D.h"
#include "pch.h"

namespace openGLTask 
{
	CTexture2D::CTexture2D(int vWidth, int vHeight, GLenum vInternalFormat, GLenum vFormat, GLenum vDataType)
		:m_TextureID(0), m_InternalFormat(vInternalFormat), m_Format(vFormat), m_Type(vDataType), m_Width(0), m_Height(0)
	{
		glGenTextures(1, &m_TextureID);
		updateTexture2D(vWidth, vHeight, nullptr);
	}
	CTexture2D::~CTexture2D()
	{
		glDeleteTextures(1, &m_TextureID);
	}
	void CTexture2D::bind() const
	{
		glBindTexture(GL_TEXTURE_2D, m_TextureID);
	}
	void CTexture2D::setTextureParameter(GLenum vName, GLint vValue)
	{
		glTexParameteri(GL_TEXTURE_2D, vName, vValue);
	}

	void CTexture2D::setTextureParameter(GLenum vName, const GLfloat* vValue)
	{
		glTexParameterfv(GL_TEXTURE_2D, vName, vValue);
	}
	void CTexture2D::updateTexture2D(int vWidth, int vHeight, const GLvoid* vPixels)
	{
		m_Width = vWidth;
		m_Height = vHeight;
		bind();
		glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, vWidth, vHeight, 0, m_Format, m_Type, vPixels);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
}