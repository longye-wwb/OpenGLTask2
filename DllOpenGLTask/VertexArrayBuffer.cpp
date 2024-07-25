#include <numeric>
#include <glad/glad.h>
#include "safe.h"
#include "VertexArrayBuffer.h"

namespace openGLTask {

	CVertexArrayBuffer::CVertexArrayBuffer(const std::vector<float>& vVertices, const std::vector<int>& vLayout, GLenum vDrawMode, GLenum vUsage)
		:m_VAO(0), m_VBO(0), m_EBO(0), m_VerticesCount((GLsizei)vVertices.size()), m_DrawMode(vDrawMode)
	{
		GL_SAFE_CALL(glGenVertexArrays(1, &m_VAO));
		GL_SAFE_CALL(glGenBuffers(1, &m_VBO));
		GL_SAFE_CALL(glBindVertexArray(m_VAO));
		GL_SAFE_CALL(glBindBuffer(GL_ARRAY_BUFFER, m_VBO));
		GL_SAFE_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vVertices.size(), vVertices.data(), vUsage));
		__configVertexLayout(vLayout);
		GL_SAFE_CALL(glBindVertexArray(0));
	}

	CVertexArrayBuffer::CVertexArrayBuffer(const std::vector<float>& vVertices, const std::vector<unsigned int>& vIndices, const std::vector<int>& vLayout, GLenum vDrawMode, GLenum vUsage)
		:m_VAO(0), m_VBO(0), m_EBO(0), m_VerticesCount((GLsizei)vVertices.size()), m_DrawMode(vDrawMode)
	{
		GL_SAFE_CALL(glGenVertexArrays(1, &m_VAO));
		GL_SAFE_CALL(glGenBuffers(1, &m_VBO));
		GL_SAFE_CALL(glGenBuffers(1, &m_EBO));
		GL_SAFE_CALL(glBindVertexArray(m_VAO));
		GL_SAFE_CALL(glBindBuffer(GL_ARRAY_BUFFER, m_VBO));
		GL_SAFE_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vVertices.size(), vVertices.data(), vUsage));
		GL_SAFE_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO));
		GL_SAFE_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * vIndices.size(), vIndices.data(), vUsage));
		__configVertexLayout(vLayout);
		GL_SAFE_CALL(glBindVertexArray(0));
	}

	CVertexArrayBuffer::~CVertexArrayBuffer()
	{
		GL_SAFE_CALL(glBindVertexArray(m_VAO));
		GL_SAFE_CALL(glDeleteVertexArrays(1, &m_VAO));
		GL_SAFE_CALL(glDeleteBuffers(1, &m_VBO));
		if (m_EBO != 0) GL_SAFE_CALL(glDeleteBuffers(1, &m_EBO));
	}

	void CVertexArrayBuffer::draw() const
	{
		GL_SAFE_CALL(glBindVertexArray(m_VAO));
		if (m_EBO != 0) GL_SAFE_CALL(glDrawElements(m_DrawMode, m_VerticesCount, GL_UNSIGNED_INT, 0));
		else GL_SAFE_CALL(glDrawArrays(m_DrawMode, 0, m_VerticesCount));
	}

	void CVertexArrayBuffer::__configVertexLayout(const std::vector<int>& vLayout) const
	{
		unsigned int TotalCount = std::accumulate(vLayout.begin(), vLayout.end(), 0);
		unsigned int CurrAccumCount = 0;
		for (int i = 0; i < vLayout.size(); ++i)
		{
			GL_SAFE_CALL(glVertexAttribPointer(i, vLayout[i], GL_FLOAT, GL_FALSE, TotalCount * sizeof(float), (void*)(CurrAccumCount * sizeof(float))));
			GL_SAFE_CALL(glEnableVertexAttribArray(i));
			CurrAccumCount += vLayout[i];
		}
	}
}