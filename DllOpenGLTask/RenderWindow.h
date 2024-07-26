#pragma once
#include "UtilityInterface.h"
#include "ConfigInterface.h"
#include "RenderConfiguration.h"
#include "Shader.h"
#include "Camera.h"
#include "VertexBuffer.h"

namespace openGLTask 
{
	class OPENGLTASK_API CRenderWindow
	{	
	private:
		int m_MajorVersion;
		int m_MinorVersion;
		int m_Width;
		int m_Height;
		int m_PosX;
		int m_PosY;
		std::string m_WinName;
		std::string m_VertShaderPath;
		std::string m_FragShaderPath;
		bool m_UseCoreProfile;
		int m_ScreenMaxWidth;
		int m_ScreenMaxHeight;
		glm::vec3 m_LightDirection;
		GLFWwindow* m_pWindow = nullptr;
		std::shared_ptr<CVertexBuffer> m_pVertexBuffer = nullptr;
		std::shared_ptr<CShader> m_pShader = nullptr;
		std::shared_ptr<CCamera> m_pCamera = nullptr;

		bool __initParametersFromXML();
		bool __readXML(CRenderConfiguration& vConfig, const std::string& vXMLName);
		void __setAndBindVertices();
		void __setAndBindShader();
		void __checkAndBindCamera(std::optional<std::tuple<double, double, double>> vCameraPos, std::optional<std::tuple<double, double, double>> vCameraFront, std::optional<std::tuple<double, double, double>> vCameraUp);
		void __checkAndSetLightDirection(std::optional<std::tuple<double, double, double>> vLightDirection);
		void __checkAndSetWindowSize(std::optional<int> vWidth, std::optional<int> vHeight);
		void __checkAndSetWindowPos(std::optional<int> vPosX, std::optional<int> vPosY);
		void __checkAndSetOpenGLVersion(std::optional<int> vMajorVersion, std::optional<int> vMinorVersion);
		void __checkAndSetWinName(const std::optional<std::string>& vWinName);
		void __checkAndSetProfile(std::optional<bool> vUseCoreProfile);
		void __checkAndSetShaderGLSL(std::optional<std::string> vVertShaderPath, std::optional<std::string> vFragShaderPath);
		bool __isOpenGLVersionValid(std::optional<int> vMajorVersion, std::optional<int> vMinorVersion);
		void __setWidth(int vWidth) { m_Width = vWidth; }
		void __setHeight(int vHeight) { m_Height = vHeight; }
		void __setAndGetScreenSize();

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
	};
}
