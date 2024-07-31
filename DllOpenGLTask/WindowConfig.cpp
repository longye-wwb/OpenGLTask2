#include "WindowConfig.h"
#include <glfw/glfw3.h>
#include "HiveLogger.h"

namespace openGLTask
{
	CWindowConfig::CWindowConfig() : m_Width(800), m_Height(600), m_WinName("GLFW_Window"), m_PosX(10), m_PosY(10), m_ScreenMaxWidth(1920), m_ScreenMaxHeight(1080)
	{
	}

	void CWindowConfig::parseConfig(CRenderConfiguration& vConfig)
	{
		__setAndGetScreenSize();

		std::optional<int> Width = vConfig.getAttribute<int>("Width");
		std::optional<int> Height = vConfig.getAttribute<int>("Height");
		__checkAndSetWindowSize(Width, Height);

		std::optional<int> PosX = vConfig.getAttribute<int>("PosX");
		std::optional<int> PosY = vConfig.getAttribute<int>("PosY");
		__checkAndSetWindowPos(PosX, PosY);

		std::optional<std::string> WinName = vConfig.getAttribute<std::string>("WinName");
		__checkAndSetWinName(WinName);
	}

	void CWindowConfig::__setAndGetScreenSize()
	{
		glfwInit();
		GLFWmonitor* pMonitor = glfwGetPrimaryMonitor();
		if (pMonitor == nullptr)
		{
			HIVE_LOG_WARNING("Can't Get Max Monitor. Set default Max Screen Size <1920*1080>");
			return;
		}
		const GLFWvidmode* pMode = glfwGetVideoMode(pMonitor);
		m_ScreenMaxWidth = pMode->width;
		m_ScreenMaxHeight = pMode->height;
	}

	void CWindowConfig::__checkAndSetWindowSize(std::optional<int> vWidth, std::optional<int> vHeight)
	{

		if (vWidth.value() >= 0 && vHeight.value() >= 0 &&
			vWidth.value() <= m_ScreenMaxWidth && vHeight.value() <= m_ScreenMaxHeight)
		{
			m_Width = vWidth.value();
			m_Height = vHeight.value();
		}
		else if (vWidth.value() > m_ScreenMaxWidth || vHeight.value() > m_ScreenMaxHeight)
		{
			if (vWidth.value() > m_ScreenMaxWidth)
			{
				m_Width = m_ScreenMaxWidth;
				HIVE_LOG_ERROR("Width is too big, use default width value.");
			}
			if (vHeight.value() > m_ScreenMaxHeight)
			{
				m_Height = m_ScreenMaxHeight;
				HIVE_LOG_ERROR("Height is too big, use default height value.");
			}
		}
		else
		{
			HIVE_LOG_ERROR("Width or Height is invalid,use default value");
		}
		return;
	}

	void CWindowConfig::__checkAndSetWindowPos(std::optional<int> vPosX, std::optional<int> vPosY)
	{

		if (vPosX.value() >= 0 && vPosY.value() >= 0
			&& vPosX.value() <= m_ScreenMaxWidth && vPosY.value() <= m_ScreenMaxHeight)
		{
			m_PosX = vPosX.value();
			m_PosY = vPosY.value();
		}
		else
		{
			HIVE_LOG_ERROR("PosX or PosY is invalid, use default value <10,10>");
		}
		return;
	}

	void CWindowConfig::__checkAndSetWinName(const std::optional<std::string>& vWinName)
	{
		if (vWinName.has_value())
		{
			m_WinName = vWinName.value();
		}
		else
		{
			HIVE_LOG_ERROR("WindowName is invalid, use default name : GLFW_Window");
		}
		return;
	}
}

