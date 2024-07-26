#pragma once
#include <glm/glm.hpp>
namespace openGLTask {
	class CLight
	{
	public:
		virtual glm::vec3 getLightColor() const = 0;
		virtual glm::vec3 getPosition() const = 0;
		virtual glm::vec3 getDirection() const = 0;
		virtual glm::vec3 setLightColor(const glm::vec3& vColor) const = 0;
		virtual glm::vec3 setPosition(const glm::vec3& vPosition) const = 0;
		virtual glm::vec3 setDirection(const glm::vec3& vDirection) const = 0;
	};
}


