#pragma once
#include <string>
#include "RenderConfiguration.h"

namespace openGLTask
{
	class CWindowConfig
	{
	public:
		CWindowConfig();
		~CWindowConfig() = default;

		int getWidth() const { return m_Width; }
		int getHeight() const { return m_Height; }
		int getPosX() const { return m_PosX; }
		int getPosY() const { return m_PosY; }
		int getScreenMaxWidth() const { return m_ScreenMaxWidth; }
		int getScreenMaxHeight() const { return m_ScreenMaxHeight; }
		const std::string& getWinName() const { return m_WinName; }
		void parseConfig(CRenderConfiguration& vConfig);

	private:
		void __setAndGetScreenSize();
		void __checkAndSetWindowSize(std::optional<int> vWidth, std::optional<int> vHeight);
		void __checkAndSetWindowPos(std::optional<int> vPosX, std::optional<int> vPosY);
		void __checkAndSetWinName(const std::optional<std::string>& vWinName);

		int m_Width;
		int m_Height;
		int m_PosX;
		int m_PosY;
		int m_ScreenMaxWidth;
		int m_ScreenMaxHeight;
		std::string m_WinName;
	};
}


