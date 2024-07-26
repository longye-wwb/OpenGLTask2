#include "pch.h"
#include <vector>
#include <optional>
#include <filesystem>
#include "RenderWindow.h"

namespace openGLTask {

	CRenderWindow::CRenderWindow() : m_MajorVersion(3), m_MinorVersion(3), m_Width(800), m_Height(600), m_WinName("GLFW_Window"), m_LightDirection(glm::vec3(0.0f, 0.0f, 1.0f)),
		m_PosX(10), m_PosY(10), m_UseCoreProfile(false), m_pWindow(nullptr), m_pVertexBuffer(nullptr), m_pShader(nullptr), m_pCamera(nullptr),
		m_VertShaderPath("../shaders/vertPerpixelShading.glsl"), m_FragShaderPath("../shaders/fragPerpixelShading.glsl"),
		m_ScreenMaxWidth(1920), m_ScreenMaxHeight(1080)
	{
	}

	bool CRenderWindow::__initParametersFromXML()
	{
		CRenderConfiguration Config;
		if (!CRenderWindow::__readXML(Config, "./config/config.xml")) {
			return false;
		}

		std::optional<int> Width = Config.getAttribute<int>("Width");
		std::optional<int> Height = Config.getAttribute<int>("Height");
		std::optional<int> PosX = Config.getAttribute<int>("PosX");
		std::optional<int> PosY = Config.getAttribute<int>("PosY");
		std::optional<int> MajorVersion = Config.getAttribute<int>("MajorVersion");
		std::optional<int> MinorVersion = Config.getAttribute<int>("MinorVersion");
		std::optional<std::string> WinName = Config.getAttribute<std::string>("WinName");
		std::optional<bool> UseCoreProfile = Config.getAttribute<bool>("UseCoreProfile");
		std::optional<std::string> VertShaderPath = Config.getAttribute<std::string>("shader_perpixel_shading_vs|SHADER_SOURCE_FILE").value();
		std::optional<std::string> FragShaderPath = Config.getAttribute<std::string>("shader_perpixel_shading_fs|SHADER_SOURCE_FILE").value();
		
		std::optional<std::tuple<double, double, double>> CamPos = Config.getAttribute<std::tuple<double, double, double>>("CameraPos");
		std::optional<std::tuple<double, double, double>> CameraFront = Config.getAttribute<std::tuple<double, double, double>>("CameraFront");
		std::optional<std::tuple<double, double, double>> CameraUp = Config.getAttribute<std::tuple<double, double, double>>("CameraUp");
		std::optional<std::tuple<double, double, double>> LightDirection = Config.getAttribute<std::tuple<double, double, double>>("LightDirection");
		
		__setAndGetScreenSize();
		__checkAndSetWindowSize(Width, Height);
		__checkAndSetWindowPos(PosX, PosY);
		__checkAndSetOpenGLVersion(MajorVersion, MinorVersion);
		__checkAndSetWinName(WinName);
		__checkAndSetProfile(UseCoreProfile);
		__checkAndSetShaderGLSL(VertShaderPath, FragShaderPath);
		__checkAndBindCamera(CamPos, CameraFront, CameraUp);
		__checkAndSetLightDirection(LightDirection);
		return true;
	}

	GLFWwindow* CRenderWindow::createWindow() {
		if (!__initParametersFromXML()) {
			HIVE_LOG_ERROR("Can't read config file, use default settings.");
		}
		if (m_pWindow != nullptr)
		{
			HIVE_LOG_ERROR("Only one window can exist.");
			return m_pWindow;
		}

		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, getMajorVersion());
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, getMinorVersion());
		glfwWindowHint(GLFW_OPENGL_PROFILE, getUseCoreProfile() ? GLFW_OPENGL_CORE_PROFILE : GLFW_OPENGL_COMPAT_PROFILE);
		
		m_pWindow = glfwCreateWindow(getWidth(), getHeight(), getWinName().c_str(), nullptr, nullptr);
		if (m_pWindow == nullptr)
		{
			HIVE_LOG_ERROR("Failed to create GLFW window");
			glfwTerminate();
			return m_pWindow;
		}
		glfwMakeContextCurrent(m_pWindow);
		glfwSetWindowPos(m_pWindow, getPosX(), getPosY());
		glfwSetFramebufferSizeCallback(m_pWindow, [](GLFWwindow* window, int width, int height) {
			glViewport(0, 0, width, height); });
		return m_pWindow;
	}

	void CRenderWindow::startRun()
	{
		GLFWwindow* m_pWindow = createWindow();
		if (m_pWindow == nullptr) {
			HIVE_LOG_ERROR("Window is not initialized!");
			return;
		}
		HIVE_LOG_INFO("GLAD : {}", gladLoadGLLoader((GLADloadproc)glfwGetProcAddress));

		__setAndBindVertices();
		__setAndBindShader();

		glm::vec3 CameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
		glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 Front = glm::vec3(0.0f, 0.0f, -1.0f);

		while (!glfwWindowShouldClose(m_pWindow))
		{
			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			m_pShader->use();
			m_pShader->setVec3("uViewPos", m_pCamera->getWorldPos());
			m_pShader->setFloat("uShininess", 32.0f);
			m_pShader->setFloat("uAmbientStrength", 0.1f);
			float angularSpeed = glm::radians(180.0f);
			float angle = angularSpeed * static_cast<float>(glfwGetTime());
			glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f));
			m_pShader->setVec3("uDirection", glm::vec3(rotation * glm::vec4(m_LightDirection, 0.0f)));
			glm::mat4 ProjectionMat = glm::perspective(glm::radians(45.0f), (float)getWidth() / (float)getHeight(), 0.1f, 100.0f);
			glm::mat4 ViewMat = m_pCamera->getViewMatrix();
			m_pShader->setMat4("uProjection", ProjectionMat);
			m_pShader->setMat4("uView", ViewMat);
			glm::mat4 model = glm::mat4(1.0f);
			m_pShader->setMat4("uModel", model);

			m_pVertexBuffer->draw();
			glfwSwapBuffers(m_pWindow);
			glfwPollEvents();
		}

		glfwTerminate();
		return ;
	}

	bool CRenderWindow::__readXML(CRenderConfiguration& vConfig,const std::string& vXMLName)
	{
		CRenderConfiguration Config;
		auto Status = hiveConfig::hiveParseConfig(vXMLName, hiveConfig::EConfigType::XML, &Config);
		if (Status == hiveConfig::EParseResult::SUCCEED) 
		{
			vConfig = Config;
			return true;
		}
		else if (Status == hiveConfig::EParseResult::SKIP_SOME_ITEMS)
		{
			vConfig = Config;
			HIVE_LOG_WARNING("Warning : Some items in XML are not set correctly!");
			return true;
		}
		else if (Status == hiveConfig::EParseResult::FAIL)
		{
			return false;
		}
	}
	
	void CRenderWindow::__setAndBindVertices()
	{
		std::vector<float>Vertices = {
			//Verices              Color             Normal
			 0.5f, 0.5f, 0.0f,  1.0f,0.0f,0.0f,  0.0f,0.0f,1.0f,
			 0.5f,-0.5f, 0.0f,  0.0f,1.0f,0.0f,  0.0f,0.0f,1.0f,
			-0.5f,-0.5f, 0.0f,  0.0f,0.0f,1.0f,  0.0f,0.0f,1.0f,
			-0.5f, 0.5f, 0.0f,  1.0f,1.0f,0.0f,  0.0f,0.0f,1.0f,
		};
		std::vector<unsigned int> Indices = {
			0, 1, 3,
			1, 2, 3
		};

		m_pVertexBuffer = std::make_shared<CVertexBuffer>(Vertices, Indices, std::vector<int>{3, 3, 3}, GL_TRIANGLES, GL_STATIC_DRAW);
	}

	void CRenderWindow::__setAndBindShader()
	{
		m_pShader = std::make_shared<CShader>(m_VertShaderPath.c_str(), m_FragShaderPath.c_str());
	}

	void CRenderWindow::__checkAndBindCamera(std::optional<std::tuple<double, double, double>> vCameraPos, std::optional<std::tuple<double, double, double>> vCameraFront, std::optional<std::tuple<double, double, double>> vCameraUp)
	{
		m_pCamera = std::make_shared<CCamera>();
		if (vCameraPos.has_value())
		{
			glm::vec3 CameraPos = glm::vec3((float)std::get<0>(vCameraPos.value()), (float)std::get<1>(vCameraPos.value()), (float)std::get<2>(vCameraPos.value()));
			m_pCamera->setWorldPos(CameraPos);
		}
		if (vCameraFront.has_value())
		{
			glm::vec3 CameraFront = glm::vec3((float)std::get<0>(vCameraFront.value()), (float)std::get<1>(vCameraFront.value()), (float)std::get<2>(vCameraFront.value()));
			m_pCamera->setFront(CameraFront);
		}
		if (vCameraUp.has_value())
		{
			glm::vec3 CameraUp = glm::vec3(std::get<0>(vCameraUp.value()), std::get<1>(vCameraUp.value()), std::get<2>(vCameraUp.value()));
			m_pCamera->setWorldUp(CameraUp);
		}
	}

	void CRenderWindow::__checkAndSetLightDirection(std::optional<std::tuple<double, double, double>> vLightDirection)
	{
		if (vLightDirection.has_value())
		{
			m_LightDirection = glm::vec3(std::get<0>(vLightDirection.value()), std::get<1>(vLightDirection.value()), std::get<2>(vLightDirection.value()));
		}
	}

	void CRenderWindow::__checkAndSetWindowSize(std::optional<int> vWidth, std::optional<int> vHeight)
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
				HIVE_LOG_ERROR("Width is too big,use max value");
			}
			if ( vHeight.value() > m_ScreenMaxHeight)
			{
				m_Height = m_ScreenMaxHeight;
				HIVE_LOG_ERROR("Height is too big,use max value");
			}
		}
		else
		{
			HIVE_LOG_ERROR("Width or Height is invalid,use default value");
		}
		return;
	}

	void CRenderWindow::__checkAndSetWindowPos(std::optional<int> vPosX, std::optional<int> vPosY)
	{

		if ( vPosX.value() >= 0 && vPosY.value() >= 0 
			&& vPosX.value() <= m_ScreenMaxWidth && vPosY.value() <= m_ScreenMaxHeight)
		{
			m_PosX = vPosX.value();
			m_PosY = vPosY.value();
		}
		else
		{
			HIVE_LOG_ERROR("PosX or PosY is invalid,use default value");
		}
		return;
	}

	void CRenderWindow::__checkAndSetOpenGLVersion(std::optional<int> vMajorVersion, std::optional<int> vMinorVersion)
	{
		if (__isOpenGLVersionValid(vMajorVersion, vMinorVersion))
		{
			m_MajorVersion = vMajorVersion.value();
			m_MinorVersion = vMinorVersion.value();
		}
		else
		{
			HIVE_LOG_ERROR("OpenGLVersion is invalid,use default value");
		}
		
	}

	void CRenderWindow::__checkAndSetWinName(const std::optional<std::string>& vWinName)
	{
		if (vWinName.has_value())
		{
			m_WinName = vWinName.value();
		}
		else
		{
			HIVE_LOG_ERROR("WindowName is invalid,use default value");
		}
		return;
	}

	void CRenderWindow::__checkAndSetProfile(std::optional<bool> vUseCoreProfile)
	{
		if (vUseCoreProfile.has_value())
		{
			m_UseCoreProfile = vUseCoreProfile.value();
		}
		else
		{
			HIVE_LOG_ERROR("CoreProfileMode is invalid, use default value");
		}
		return;
	}

	void CRenderWindow::__checkAndSetShaderGLSL(std::optional<std::string> vVertShaderPath, std::optional<std::string> vFragShaderPath)
	{
		if (vVertShaderPath.has_value() && vFragShaderPath.has_value())
		{
			if (std::filesystem::exists(vVertShaderPath.value()) && std::filesystem::exists(vFragShaderPath.value()))
			{
				m_VertShaderPath = vVertShaderPath.value();
				m_FragShaderPath = vFragShaderPath.value();
			}
		}
		else
		{
			HIVE_LOG_ERROR("ShaderGLSL paht is invalid, use default path.");
		}
		return;
	}
	
	bool CRenderWindow::__isOpenGLVersionValid(std::optional<int> vMajorVersion, std::optional<int> vMinorVersion) {
		if (!vMajorVersion.has_value() || !vMinorVersion.has_value()) {
			return false;
		}

		int major = vMajorVersion.value();
		int minor = vMinorVersion.value();

		switch (major) {
		case 1:
			switch (minor) {
			case 0:
			case 1:
				return true;
			default:
				return false;
			}
		case 2:
			switch (minor) {
			case 0:
			case 1:
				return true;
			default:
				return false;
			}
		case 3:
			switch (minor) {
			case 0:
			case 1:
			case 2:
			case 3:
				return true;
			default:
				return false;
			}
		case 4:
			switch (minor) {
			case 0:
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
				return true;
			default:
				return false;
			}
		default:
			return false;
		}
	}

	void CRenderWindow::__setAndGetScreenSize() {
		glfwInit();
		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		if (monitor == nullptr) {
			HIVE_LOG_WARNING("Can't Get Monitor");
			return;
		}
		const GLFWvidmode* mode = glfwGetVideoMode(monitor);
		m_ScreenMaxWidth = mode->width;
		m_ScreenMaxHeight = mode->height;
	}

}