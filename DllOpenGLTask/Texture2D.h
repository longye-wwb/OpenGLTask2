#pragma once
#include <glad/glad.h>
#include  "dllFile.h"

namespace openGLTask 
{
	class OPENGLTASK_API CTexture2D
	{
	public:
		CTexture2D(int vWidth, int vHeight, GLenum vInternalFormat, GLenum vFormat, GLenum vDataType);
		~CTexture2D();

		GLuint getID() const { return m_TextureID; }
		void bind() const ;
		int getWidth() const { return m_Width; }
		int getHeight() const { return m_Height; }
		static void setTextureParameter(GLenum vName, GLint vValue);
		static void setTextureParameter(GLenum vName, const GLfloat* vValue);
		void updateTexture2D(int vWidth, int vHeight, const GLvoid* vPixels);

	private:
		GLuint m_TextureID;
		GLenum m_InternalFormat;
		GLenum m_Format;
		GLenum m_Type;
		int m_Width, m_Height;
	};
}

