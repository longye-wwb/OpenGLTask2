#include "VertexBuffer.h"

namespace openGLTask {

	CVertexBuffer::CVertexBuffer(const std::vector<float>& vVertices, GLuint vVerticesCol,GLenum vDrawMode, GLenum vUsage)
	:m_VAO(0), m_VBO(0), m_EBO(0), m_VerticesCol(vVerticesCol), m_DrawMode(vDrawMode),m_VerticesCount(vVerticesCol)
	{
		glGenVertexArrays(1, &m_VAO);
		glGenBuffers(1, &m_VBO);
		glBindVertexArray(m_VAO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vVertices.size(), vVertices.data(), vUsage);
		glBindVertexArray(0);
	}
	CVertexBuffer::CVertexBuffer(const std::vector<float>& vVertices, GLuint vVerticesCol,const std::vector<unsigned int>& vIndices, GLenum vDrawMode, GLenum vUsage)
	:m_VAO(0), m_VBO(0), m_EBO(0), m_VerticesCol(vVerticesCol), m_DrawMode(vDrawMode),m_VerticesCount(vIndices.size())
	{
		glGenVertexArrays(1, &m_VAO);
		glGenBuffers(1, &m_VBO);
		glGenBuffers(1, &m_EBO);
		glBindVertexArray(m_VAO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		//glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vVertices.size(), vVertices.data(), vUsage));
		/*__configVertexLayout(vLayout);
		GL_SAFE_CALL(glBindVertexArray(0));*/
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vVertices.size(), vVertices.data(), vUsage);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * vIndices.size(), vIndices.data(), vUsage);
		glBindVertexArray(0);
	}
	void CVertexBuffer::draw()
	{
		glBindVertexArray(m_VAO);
		if (m_EBO != 0) glDrawElements(m_DrawMode, m_VerticesCount, GL_UNSIGNED_INT, 0);
		else glDrawArrays(m_DrawMode, 0, m_VerticesCol);
	}
}