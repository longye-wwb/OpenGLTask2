#pragma once
#include "Light.h"
#include "dllFile.h"
namespace openGLTask 
{
	class OPENGLTASK_API CDirectionalLight:public CLight
	{
	private:
		glm::vec3 m_LightColor;
		glm::vec3 m_LightPosition;
		glm::vec3 m_LightDirection;

	public:
		CDirectionalLight();

		glm::vec3 getLightColor() const override { return m_LightColor; }
		glm::vec3 getPosition() const override { return m_LightPosition; }
		glm::vec3 getDirection() const override { return m_LightDirection; }
		void setLightColor(const glm::vec3& vColor) override { m_LightColor = vColor; }
		void setPosition(const glm::vec3& vPosition) override { m_LightPosition = vPosition; }
		void setDirection(const glm::vec3& vDirection)  override { m_LightDirection = vDirection; }
		glm::vec3 rotateDirectionalLight(const float& vSpeed);
	};
}

