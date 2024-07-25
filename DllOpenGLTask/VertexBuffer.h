#pragma once
#include <glad/glad.h>
#include <vector>
namespace openGLTask {

	class CVertexBuffer
	{
	public:
		CVertexBuffer(
			const std::vector<float>& vVertices,
			GLenum vDrawMode = GL_TRIANGLES
		);
		CVertexBuffer(
			const std::vector<float>& vVertices,
			const std::vector<unsigned int>& vIndices,
			GLenum vDrawMode = GL_TRIANGLES
		);
		~CVertexBuffer();
		void draw() const;
	private:
		GLuint m_VAO;
		GLuint m_VBO;
		GLuint m_EBO;
		GLsizei m_VerticesCount;
		GLenum m_DrawMode;
	};
}


