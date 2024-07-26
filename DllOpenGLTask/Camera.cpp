#include <glm/gtc/matrix_transform.hpp>
#include "Camera.h"

namespace openGLTask
{
	CCamera::CCamera() : m_Position(0.0f, 0.0f, -3.0f), m_Front(0.0f, 0.0f, -1.0f), m_Up(0.0f, 1.0f, 0.0f)
	{
		__updateViewMatrix();
	}

	void CCamera::__updateViewMatrix()
	{
		m_ViewMatrix = glm::lookAt(m_Position, m_Position + m_Front, m_Up);
	}

	void CCamera::setWorldPos(const glm::vec3& vPosition)
	{
		m_Position = vPosition;
		__updateViewMatrix();
	}

	void CCamera::setFront(const glm::vec3& vFront)
	{
		m_Front = vFront;
		__updateViewMatrix();
	}

	void CCamera::setWorldUp(const glm::vec3& vWorldUp)
	{
		m_Up = vWorldUp;
		__updateViewMatrix();
	}

}


