#include  "pch.h"
#include "FpsController.h"

namespace  openGLTask
{
	CFPSController::CFPSController() :
		m_Yaw(-90.0f), m_Pitch(0.0f), m_Sensitivity(0.1f),
		m_IsLeftBtnPressed(false), m_IsFirst(true), m_Speed(50.0f)
	{}

	void CFPSController::control(const std::shared_ptr<CCamera>& vCamera)
	{
		CCameraController::control(vCamera);
		__updateCamera();
	}

	void CFPSController::onKeyDown(int vKey)
	{
		m_Keys.insert(vKey);
	}

	void CFPSController::onKeyUp(int vKey)
	{
		m_Keys.erase(vKey);
	}

	void CFPSController::onMouseButtonDown(int vKey)
	{
		if (vKey == HIVE_MOUSE_BUTTON_LEFT) m_IsLeftBtnPressed = true;
	}

	void CFPSController::onMouseButtonUp(int vKey)
	{
		if (vKey == HIVE_MOUSE_BUTTON_LEFT)
		{
			m_IsLeftBtnPressed = false;
			m_IsFirst = true;
		}
	}
	void CFPSController::update(float vDeltaTime)
	{
		float Velocity = m_Speed * vDeltaTime;
		glm::vec3 Position = m_pCamera->getWorldPos();
		if (m_Keys.count(HIVE_KEY_A) == 1)
		{
			Position -= Velocity * m_pCamera->getRight();
			m_pCamera->setWorldPos(Position);
		}
		if (m_Keys.count(HIVE_KEY_D) == 1)
		{
			Position += Velocity * m_pCamera->getRight();
			m_pCamera->setWorldPos(Position);
		}
		if (m_Keys.count(HIVE_KEY_W) == 1)
		{
			Position += Velocity * m_pCamera->getFront();
			m_pCamera->setWorldPos(Position);
		}
		if (m_Keys.count(HIVE_KEY_S) == 1)
		{
			Position -= Velocity * m_pCamera->getFront();
			m_pCamera->setWorldPos(Position);
		}
		if (m_Keys.count(HIVE_KEY_Q) == 1)
		{
			Position += Velocity * m_pCamera->getUp();
			m_pCamera->setWorldPos(Position);
		}
		if (m_Keys.count(HIVE_KEY_E) == 1)
		{
			Position -= Velocity * m_pCamera->getUp();
			m_pCamera->setWorldPos(Position);
		}
	}

	void CFPSController::onMouseMove(float vPosX, float vPosY)
	{
		if (!m_IsLeftBtnPressed) return;
		static float LastPosX, LastPosY;
		if (m_IsFirst)
		{
			LastPosX = vPosX;
			LastPosY = vPosY;
			m_IsFirst = false;
			return;
		}
		float DeltaX = vPosX - LastPosX;
		float DeltaY = vPosY - LastPosY;
		LastPosX = vPosX;
		LastPosY = vPosY;
		m_Yaw += DeltaX * m_Sensitivity;
		m_Pitch -= DeltaY * m_Sensitivity;
		if (m_Pitch > 89.0f) m_Pitch = 89.0f;
		if (m_Pitch < -89.0f) m_Pitch = -89.0f;
		__updateCamera();
	}

	void CFPSController::onMouseScroll(float vOffsetX, float vOffsetY)
	{
			float FovY = 2.0f * glm::atan(m_pCamera->getNearHeight() / (2.0f * m_pCamera->getNear()));
			FovY -= (float)vOffsetY;
			if (FovY < 1.0f)
				FovY = 1.0f;
			if (FovY > 45.0f)
				FovY = 45.0f;
			float NearHeight = 2.0f * m_pCamera->getNear() * glm::tan(FovY / 2.0f);
			m_pCamera->setNearHeight(NearHeight);
	}
	void CFPSController::__updateCamera()
	{
		glm::vec3 Front{};
		Front.x = glm::cos(glm::radians(m_Yaw)) * glm::cos(glm::radians(m_Pitch));
		Front.y = glm::sin(glm::radians(m_Pitch));
		Front.z = glm::sin(glm::radians(m_Yaw)) * glm::cos(glm::radians(m_Pitch));
		Front = glm::normalize(Front);
		m_pCamera->setFront(Front);
	}
}