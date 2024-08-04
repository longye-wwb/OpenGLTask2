#include "pch.h"
#include "Camera.h"

namespace openGLTask
{
	CCamera::CCamera(float vAspect, float vNearHeight, float vNear, float vFar) :m_Position(0.0f, 1.0f, 10.0f),
		m_Front(0.0f, 0.0f, -1.0f), m_Up(0.0f, 1.0f, 0.0f), m_Right(1.0f, 0.0f, 0.0f), m_WorldUp(0.0f, 1.0f, 0.0f),
		m_ViewMatrix{}, m_ProjectionMatrix{}, m_SolidColor(0.2f, 0.3f, 0.3f, 1.0f), m_Aspect(vAspect), m_Near(vNear), m_Far(vFar)
	{
		m_NearHeight = m_Near * glm::tan(glm::radians(45.0f) * 0.5f);
		__updateCameraVectors();
		__updateViewMatrix();
		__updateProjectionMatrix();
	}

	void CCamera::setAspect(float vAspect)
	{
		m_Aspect = vAspect;
		__updateProjectionMatrix();
	}


	void CCamera::setNear(float vNear)
	{
		m_Near = vNear;
		__updateProjectionMatrix();
	}

	void CCamera::setFar(float vFar)
	{
		m_Far = vFar;
		__updateProjectionMatrix();
	}

	void CCamera::setNearHeight(float vNearHeight)
	{
		m_NearHeight = vNearHeight;
		__updateProjectionMatrix();
	}

	void CCamera::setFront(const glm::vec3& vFront)
	{
		m_Front = vFront;
		__updateCameraVectors();
		__updateViewMatrix();
	}

	void CCamera::setWorldPos(const glm::vec3& vPosition)
	{
		m_Position = vPosition;
		__updateViewMatrix();
	}

	void CCamera::setWorldUp(const glm::vec3& vWorldUp)
	{
		m_WorldUp = vWorldUp;
		__updateCameraVectors();
		__updateViewMatrix();
	}

	void CCamera::setViewport(int vWidth, int vHeight)
	{
		setAspect((float)vWidth / (float)vHeight);
	}

	void CCamera::__updateViewMatrix()
	{
		m_ViewMatrix = glm::lookAt(m_Position, m_Position + m_Front, m_Up);
	}

	void CCamera::__updateProjectionMatrix()
	{
			float FovY = 2.0f * glm::atan(getNearHeight() / (2.0f * getNear()));
			m_ProjectionMatrix = glm::perspective(FovY, getAspect(), getNear(), getFar());
	}

	void CCamera::__updateCameraVectors()
	{
		m_Front = glm::normalize(m_Front);
		m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));
		m_Up = glm::normalize(glm::cross(m_Right, m_Front));
	}
}


