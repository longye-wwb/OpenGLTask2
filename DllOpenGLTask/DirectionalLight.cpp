#include "DirectionalLight.h"
#include "pch.h"

namespace openGLTask 
{
	CDirectionalLight::CDirectionalLight()
	:m_LightColor(glm::vec3(1.0f, 1.0f, 1.0f)),m_LightDirection(0.0f,0.0f,- 1.0f),m_LightPosition(0.0f,0.0f,0.0f),
		m_DeltaTime(0.0f), m_LastFrame(0.0f), m_RotationAngle(0.0f), m_TimeScale(0.01f)
	{
	}
	glm::vec3 CDirectionalLight::rotateDirectionalLight(const float& vSpeed)
	{
		float AngularSpeed = glm::radians(vSpeed);
		 m_RotationAngle += AngularSpeed * m_TimeScale;
		glm::mat4 Rotation = glm::rotate(glm::mat4(1.0f), m_RotationAngle, glm::vec3(0.0f, 1.0f, 0.0f));
		m_LightDirection = glm::vec3(Rotation * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f));
		return m_LightDirection;
	}
}
