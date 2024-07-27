#pragma once
#include <glad/glad.h>
#include <vector>

namespace openGLTask {

	class CVertexBuffer
	{
	private:
		GLuint m_VAO;
		GLuint m_VBO;
		GLuint m_EBO;
		GLuint m_VerticesCount;
		GLenum m_DrawMode;

		void __configVertex(const std::vector<int>& vVertexIndexLayout);

	public:
		CVertexBuffer(
			const std::vector<float>& vVertices,
			const std::vector<int>& vVertexIndexLayout,
			GLenum vDrawMode = GL_TRIANGLES,
			GLenum vUsage = GL_STATIC_DRAW
		);
		CVertexBuffer(
			const std::vector<float>& vVertices,
			const std::vector<unsigned int>& vIndices,
			const std::vector<int>& vVertexIndexLayout,
			GLenum vDrawMode = GL_TRIANGLES,
			GLenum vUsage = GL_STATIC_DRAW
		);
		~CVertexBuffer();

		void draw();
	};
}


