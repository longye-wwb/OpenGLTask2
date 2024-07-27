#pragma once
#include <glm/glm.hpp>

namespace openGLTask 
{
	class CLight
	{
	public:
		virtual glm::vec3 getLightColor() const = 0;
		virtual glm::vec3 getPosition() const = 0;
		virtual glm::vec3 getDirection() const = 0;
		virtual void setLightColor(const glm::vec3& vColor) = 0;
		virtual void setPosition(const glm::vec3& vPosition) = 0;
		virtual void setDirection(const glm::vec3& vDirection) = 0;
	};
}


