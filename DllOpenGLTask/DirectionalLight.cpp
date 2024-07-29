#include "DirectionalLight.h"
#include "pch.h"

namespace openGLTask 
{
	CDirectionalLight::CDirectionalLight()
	:m_LightColor(glm::vec3(1.0f, 1.0f, 1.0f)),m_LightDirection(0.0f, 0.0f, -1.0f),m_LightPosition(0.0f,0.0f,0.0f)
	{
	}

	glm::vec3 CDirectionalLight::rotateDirectionalLight(const float& vSpeed)
	{
		float AngularSpeed = glm::radians(vSpeed);
		float Angle = AngularSpeed * static_cast<float>(glfwGetTime());
		glm::mat4 Rotation = glm::rotate(glm::mat4(1.0f), Angle, glm::vec3(0.0f, 1.0f, 0.0f));
		return glm::vec3(Rotation * glm::vec4(m_LightDirection, 0.0f));
	}
}
