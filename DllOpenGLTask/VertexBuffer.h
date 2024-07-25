#pragma once
#include <glad/glad.h>
#include <vector>

#define VEC3 3
#define STRIDE 9

namespace openGLTask {

	class CVertexBuffer
	{
	public:
		CVertexBuffer(
			const std::vector<float>& vVertices,
			GLuint vVerticesCol,
			GLenum vDrawMode = GL_TRIANGLES,
			GLenum vUsage = GL_STATIC_DRAW
		);
		CVertexBuffer(
			const std::vector<float>& vVertices,
			GLuint vVerticesCol,
			const std::vector<unsigned int>& vIndices,
			GLenum vDrawMode = GL_TRIANGLES,
			GLenum vUsage = GL_STATIC_DRAW
		);
		~CVertexBuffer()=default;
		void draw();
	
		GLuint getVAO() { return m_VAO; }
	private:
		void __configVertex();
		GLuint m_VAO;
		GLuint m_VBO;
		GLuint m_EBO;
		GLuint m_VerticesCol;
		GLuint m_VerticesCount;
		GLenum m_DrawMode;
	};
}


