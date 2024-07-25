#pragma once
#include "dllFile.h"
#include "UtilityInterface.h"
#include "ConfigInterface.h"
#include "RenderConfiguration.h"
#include <optional>
#include "VertexBuffer.h"
#define LAYOUT 0
#define VEC3SIZE 3
#define STRIDE 9
#define OFFSET 0

namespace openGLTask {
	class OPENGLTASK_API CRenderWindow
	{
		public:
			CRenderWindow();

			GLFWwindow* createWindow();
			void startRun();
			int getWidth() { return m_Width; }
			int getHeight() { return m_Height; }
			int getPosX() { return m_PosX; }
			int getPosY() { return m_PosY; }
			int getMajorVersion() { return m_MajorVersion; }
			int getMinorVersion() { return m_MinorVersion; }
			bool getUseCoreProfile() { return m_UseCoreProfile; }
			const std::string& getWinName() { return m_WinName; }		
		private:
			bool __init();
			bool __readXML(CRenderConfiguration& vConfig, const std::string& vXMLName);
			void __setAndBindVertices(unsigned int& vVBO, unsigned int& vVAO, unsigned int& vEBO);
			void __deleteBind(unsigned int& vVBO, unsigned int& vVAO, unsigned int& vEBO);
			void __checkAndSetWindowSize(std::optional<int> vWidth, std::optional<int> vHeight);
			void __checkAndSetWindowPos(std::optional<int> vPosX, std::optional<int> vPosY);
			void __checkAndSetOpenGLVersion(std::optional<int> vMajorVersion, std::optional<int> vMinorVersion);
			void __checkAndSetWinName(const std::optional<std::string>& vWinName);
			void __checkAndSetProfile(std::optional<bool> vUseCoreProfile);
			bool __isOpenGLVersionValid(std::optional<int> vMajorVersion, std::optional<int> vMinorVersion);
			void __setWidth(int vWidth) { m_Width = vWidth; }
			void __setHeight(int vHeight) { m_Height = vHeight; }
			void __setAndGetScreenSize();
		
			void __renderLoop();
			int m_MajorVersion;
			int m_MinorVersion;
			int m_Width;
			int m_Height;
			int m_PosX;
			int m_PosY;
			std::string m_WinName;
			bool m_UseCoreProfile;
			int m_ScreenMaxWidth;
			int m_ScreenMaxHeight;
			GLFWwindow* m_pWindow=nullptr;
			std::shared_ptr<CVertexBuffer> m_pVertex = nullptr;
	};

}
