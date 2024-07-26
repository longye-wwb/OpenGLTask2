#include <glm/gtc/matrix_transform.hpp>
#include "CCamera.h"

namespace openGLTask
{
	CCamera::CCamera() : m_Position(0.0f, 0.0f, -3.0f), m_Front(0.0f, 0.0f, -1.0f), m_Up(0.0f, 1.0f, 0.0f)
	{
	}

	void CCamera::__updateViewMatrix()
	{
		m_ViewMatrix = glm::lookAt(m_Position, m_Position + m_Front, m_Up);
	}

	void CCamera::__setWorldPos(const glm::vec3& vPosition)
	{
		m_Position = vPosition;
	}

	void CCamera::__setFront(const glm::vec3& vFront)
	{
		m_Front = vFront;
	}

	void CCamera::__setWorldUp(const glm::vec3& vWorldUp)
	{
		m_Up = vWorldUp;
	}

}


