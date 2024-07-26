#include <glm/glm.hpp>

namespace openGLTask
{
	class CCamera
	{
	private:
		glm::mat4 m_ViewMatrix;
		glm::vec3 m_Position;
		glm::vec3 m_Front;
		glm::vec3 m_Up;

		void __updateViewMatrix();

	public:
		CCamera();

		void setWorldPos(const glm::vec3& vPosition);
		void setFront(const glm::vec3& vFront);
		void setWorldUp(const glm::vec3& vWorldUp);
		const glm::mat4& getViewMatrix() const { return m_ViewMatrix; }
		const glm::vec3& getWorldPos() const { return m_Position; }
		const glm::vec3& getFront() const { return m_Front; }
		const glm::vec3& getUp() const { return m_Up; }
	};
}


