#include "VertexBuffer.h"
#include "pch.h"
namespace openGLTask {

	CVertexBuffer::CVertexBuffer(const std::vector<float>& vVertices, GLuint vVerticesCol,GLenum vDrawMode, GLenum vUsage)
	:m_VAO(0), m_VBO(0), m_EBO(0), m_VerticesCol(vVerticesCol), m_DrawMode(vDrawMode),m_VerticesCount(vVerticesCol)
	{
		glGenVertexArrays(1, &m_VAO);
		glGenBuffers(1, &m_VBO);
		glBindVertexArray(m_VAO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vVertices.size(), vVertices.data(), vUsage);
		__configVertex();
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	CVertexBuffer::CVertexBuffer(const std::vector<float>& vVertices, GLuint vVerticesCol,const std::vector<unsigned int>& vIndices, GLenum vDrawMode, GLenum vUsage)
	:m_VAO(0), m_VBO(0), m_EBO(0), m_VerticesCol(vVerticesCol), m_DrawMode(vDrawMode),m_VerticesCount(vIndices.size())
	{
		glGenVertexArrays(1, &m_VAO);
		glGenBuffers(1, &m_VBO);
		glGenBuffers(1, &m_EBO);
		glBindVertexArray(m_VAO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vVertices.size(), vVertices.data(), vUsage);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * vIndices.size(), vIndices.data(), vUsage);
		__configVertex();
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void CVertexBuffer::draw()
	{
		glBindVertexArray(m_VAO);
		if (m_EBO != 0) glDrawElements(m_DrawMode, m_VerticesCount, GL_UNSIGNED_INT, 0);
		else glDrawArrays(m_DrawMode, 0, m_VerticesCol);
	}
	void CVertexBuffer::__configVertex()
	{
		for (int i = 0; i <3; ++i)
		{
			glVertexAttribPointer(i, 3, GL_FLOAT, GL_FALSE, STRIDE * sizeof(float), (void*)(i*VEC3 * sizeof(float)));
			glEnableVertexAttribArray(i);
		}
	}
}