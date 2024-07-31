#pragma once
#include <stb_image.h>
#include "dllFile.h"
#include <glad/glad.h>
namespace openGLTask {
	class OPENGLTASK_API CTexture2D
	{
	public:
		CTexture2D();
		unsigned int loadTexture(char const* path);
		// Inherited via CTexture
		GLuint getID() const { return m_TextureID; }
		void bind() const ;
		int getWidth() const { return m_Width; }
		int getHeight() const { return m_Height; }
		static void setParameters(GLenum vName, GLint vValue);
		static void setParameters(GLenum vName, const GLfloat* vValue);
	private:
		GLuint m_TextureID;
		int m_Width, m_Height;
	};
}

