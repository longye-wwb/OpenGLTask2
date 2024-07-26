#pragma once
#include "Light.h"
namespace openGLTask {
	class CDirectionalLight:public CLight
	{
	public:
		glm::vec3 getLightColor() const override;
		glm::vec3 getPosition() const override;
		glm::vec3 getDirection() const override;
		glm::vec3 setLightColor(const glm::vec3& vColor) const override;
		glm::vec3 setPosition(const glm::vec3& vPosition) const override;
		glm::vec3 setDirection(const glm::vec3& vDirection) const override;
	};
}

