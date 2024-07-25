#pragma once
#include <vector>

namespace openGLTask {

	class CVertexArrayBuffer
	{
	public:
		CVertexArrayBuffer(
			const std::vector<float>& vVertices,
			const std::vector<int>& vLayout,
			GLenum vDrawMode = GL_TRIANGLES,
			GLenum vUsage = GL_STATIC_DRAW
		);
		CVertexArrayBuffer(
			const std::vector<float>& vVertices,
			const std::vector<unsigned int>& vIndices,
			const std::vector<int>& vLayout,
			GLenum vDrawMode = GL_TRIANGLES,
			GLenum vUsage = GL_STATIC_DRAW
		);
		~CVertexArrayBuffer();
		virtual void draw() const;
		GLuint getVAO(){ return m_VAO; }
		GLuint getVBO(){ return m_VBO; }
		GLuint getEBO(){ return m_EBO; }
	private:
		void __configVertexLayout(const std::vector<int>& vLayout) const;
		GLuint m_VAO;
		GLuint m_VBO;
		GLuint m_EBO;
		GLsizei m_VerticesCount;
		GLenum m_DrawMode;
	};
}


