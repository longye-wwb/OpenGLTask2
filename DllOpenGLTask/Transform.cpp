#include "Transform.h"
#include "pch.h"

namespace openGLTask
{
	CTransform::CTransform(const glm::vec3& vPosition, const glm::vec3& vRotation, const glm::vec3& vScale)
		:m_Position(vPosition), m_Rotation(vRotation), m_Scale(vScale), m_ModelMatrix{}
	{
		__updateModelMatrix();
	}

	void CTransform::setLocalPosition(const glm::vec3& vPosition)
	{
		m_Position = vPosition;
		__updateModelMatrix();
	}

	void CTransform::setLocalRotation(const glm::vec3& vRotation)
	{
		m_Rotation = vRotation;
		__updateModelMatrix();
	}

	void CTransform::setLocalScale(const glm::vec3& vScale)
	{
		m_Scale = vScale;
		__updateModelMatrix();
	}

	void CTransform::__updateModelMatrix()
	{
		// T * R * S
		glm::vec3 RotationRadians = glm::radians(m_Rotation);
		m_ModelMatrix = glm::mat4(1.0f);
		m_ModelMatrix = glm::translate(m_ModelMatrix, m_Position);
		
		m_ModelMatrix = glm::rotate(m_ModelMatrix, RotationRadians.z, glm::vec3(0.0f, 0.0f, 1.0f));
		m_ModelMatrix = glm::rotate(m_ModelMatrix, RotationRadians.x, glm::vec3(1.0f, 0.0f, 0.0f));
		m_ModelMatrix = glm::rotate(m_ModelMatrix, RotationRadians.y, glm::vec3(0.0f, 1.0f, 0.0f));

		m_ModelMatrix = glm::scale(m_ModelMatrix, m_Scale);
	}
}