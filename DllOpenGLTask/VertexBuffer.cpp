#include "VertexBuffer.h"
namespace openGLTask {
	CVertexBuffer::CVertexBuffer(const std::vector<float>& vVertices, GLenum vDrawMode)
	:m_VAO(0), m_VBO(0), m_EBO(0), m_VerticesCount((GLsizei)vVertices.size()), m_DrawMode(vDrawMode)
	{
		glGenVertexArrays(1, &m_VAO);
		glGenBuffers(1, &m_VBO);
		glBindVertexArray(m_VAO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		//glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vVertices.size(), vVertices.data(), vUsage));
		/*__configVertexLayout(vLayout);
		GL_SAFE_CALL(glBindVertexArray(0));*/
	}
	CVertexBuffer::CVertexBuffer(const std::vector<float>& vVertices, const std::vector<unsigned int>& vIndices, GLenum vDrawMode)
	:m_VAO(0), m_VBO(0), m_EBO(0), m_VerticesCount((GLsizei)vVertices.size()), m_DrawMode(vDrawMode)
	{

	}

}