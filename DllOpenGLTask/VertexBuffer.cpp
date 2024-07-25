#include "VertexBuffer.h"
#include <numeric>
#define VEC3 3
#define STRIDE 9

namespace openGLTask {

	CVertexBuffer::CVertexBuffer(const std::vector<float>& vVertices, const std::vector<int>& vVertexIndex, GLenum vDrawMode, GLenum vUsage)
	:m_VAO(0), m_VBO(0), m_EBO(0), m_DrawMode(vDrawMode),m_VerticesCount(vVertices.size())
	{
		int VertexCount = std::accumulate(vVertexIndex.begin(), vVertexIndex.end(), 0);
		m_VerticesCount = vVertices.size() / VertexCount;
		glGenVertexArrays(1, &m_VAO);
		glGenBuffers(1, &m_VBO);
		glBindVertexArray(m_VAO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vVertices.size(), vVertices.data(), vUsage);
		__configVertex(vVertexIndex);
		glBindVertexArray(0);
	}

	CVertexBuffer::CVertexBuffer(const std::vector<float>& vVertices,const std::vector<unsigned int>& vIndices, const std::vector<int>& vVertexIndex, GLenum vDrawMode, GLenum vUsage)
	:m_VAO(0), m_VBO(0), m_EBO(0), m_DrawMode(vDrawMode),m_VerticesCount(vVertices.size())
	{
		glGenVertexArrays(1, &m_VAO);
		glGenBuffers(1, &m_VBO);
		glGenBuffers(1, &m_EBO);
		glBindVertexArray(m_VAO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vVertices.size(), vVertices.data(), vUsage);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * vIndices.size(), vIndices.data(), vUsage);
		__configVertex(vVertexIndex);
		glBindVertexArray(0);
	}

	CVertexBuffer::~CVertexBuffer()
	{
		glDeleteVertexArrays(1, &m_VAO);
		glDeleteBuffers(1, &m_VBO);
		if (m_EBO!=0) glDeleteBuffers(1, &m_EBO);
	}

	void CVertexBuffer::draw()
	{
		glBindVertexArray(m_VAO);
		if (m_EBO != 0) glDrawElements(m_DrawMode, m_VerticesCount, GL_UNSIGNED_INT, 0);
		else glDrawArrays(m_DrawMode, 0, m_VerticesCount);
	}

	void CVertexBuffer::__configVertex(const std::vector<int>& vVertexIndex)
	{
		int VertexCount= std::accumulate(vVertexIndex.begin(), vVertexIndex.end(), 0);
		int TempCount = 0;
		for (int i = 0; i <3; ++i)
		{
			glVertexAttribPointer(i, vVertexIndex[i], GL_FLOAT, GL_FALSE, VertexCount * sizeof(float), (void*)(TempCount* sizeof(float)));
			glEnableVertexAttribArray(i);
			TempCount += vVertexIndex[i];
		}
	}
}