#include "pch.h"
#include <vector>
#include <optional>
#include <filesystem>
#include "RenderWindow.h"
#include "shader_s.h"

namespace openGLTask {

	CRenderWindow::CRenderWindow() : m_MajorVersion(3), m_MinorVersion(3), m_Width(800), m_Height(600),
		m_PosX(10), m_PosY(10), m_UseCoreProfile(false), m_pWindow(nullptr), m_pVertex(nullptr), m_WinName("GLFW_Window"),
		m_VertShaderPath("../shaders/vertPerpixelShading.glsl"), m_FragShaderPath("../shaders/fragPerpixelShading.glsl"),
		m_ScreenMaxWidth(1920), m_ScreenMaxHeight(1080)
	{
	}

	bool CRenderWindow::__init()
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
		
		__setAndGetScreenSize();
		__checkAndSetWindowSize(Width, Height);
		__checkAndSetWindowPos(PosX, PosY);
		__checkAndSetOpenGLVersion(MajorVersion, MinorVersion);
		__checkAndSetWinName(WinName);
		__checkAndSetProfile(UseCoreProfile);
		__checkAndSetShaderGLSL(VertShaderPath, FragShaderPath);
		return true;
	}

	GLFWwindow* CRenderWindow::createWindow() {
		if (!__init()) {
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
		if (m_pShader == nullptr) {
			HIVE_LOG_ERROR("Shader is not initialized!");
			return;
		}
		HIVE_LOG_INFO("GLAD : {}", gladLoadGLLoader((GLADloadproc)glfwGetProcAddress));
		
		__setAndBindVertices();
		VAO = m_pVertex->getVAO();
		VBO = m_pVertex->getVBO();
		EBO = m_pVertex->getEBO();

		glm::vec3 CameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
		glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 Front = glm::vec3(0.0f, 0.0f, -1.0f);

		while (!glfwWindowShouldClose(m_pWindow))
		{
			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			LightingShader.use();
			LightingShader.setVec3("uViewPos", CameraPos);
			LightingShader.setFloat("uShininess", 32.0f);
			LightingShader.setFloat("uAmbientStrength", 0.1f);

			glm::mat4 ProjectionMat = glm::perspective(glm::radians(45.0f), (float)getWidth() / (float)getHeight(), 0.1f, 100.0f);
			glm::mat4 ViewMat = glm::lookAt(CameraPos, CameraPos + Front, Up);
			m_pShader->setUniform("uProjection", ProjectionMat);
			m_pShader->setUniform("uView", ViewMat);
			glm::mat4 model = glm::mat4(1.0f);
			m_pShader->setUniform("uModel", model);

			m_pVertex->draw();

			glfwSwapBuffers(m_pWindow);
			glfwPollEvents();
		}

		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);

		glfwTerminate();
		
		// __renderLoop();
		// __deleteBind(VBO, VAO, EBO);
		return ;
	}

	bool CRenderWindow::__readXML(CRenderConfiguration& vConfig,const std::string& vXMLName)
	{
		CRenderConfiguration Config;
		auto Status = hiveConfig::hiveParseConfig(vXMLName, hiveConfig::EConfigType::XML, &Config);
		if (Status == hiveConfig::EParseResult::SUCCEED) {
			vConfig = Config;
			return true;
		}
		return false;
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
			HIVE_LOG_ERROR("CoreProfileMode is invalid,use default value");
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
				m_pShader = std::make_shared<CShader>(m_VertShaderPath, m_FragShaderPath);
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
			return;
		}
		const GLFWvidmode* mode = glfwGetVideoMode(monitor);
		m_ScreenMaxWidth = mode->width;
		m_ScreenMaxHeight = mode->height;
	}

	void CRenderWindow::__renderLoop()
	{
		HIVE_LOG_INFO("GLAD : {}", gladLoadGLLoader((GLADloadproc)glfwGetProcAddress));
		Shader LightingShader("directionLight.vs", "directionLight.fs");
		float Vertices[] = {
			//Verices              Color             Normal
			 0.5f, 0.5f, 0.0f,  1.0f,0.0f,0.0f,  0.0f,0.0f,1.0f,
			 0.5f,-0.5f, 0.0f,  0.0f,1.0f,0.0f,  0.0f,0.0f,1.0f,
			-0.5f,-0.5f, 0.0f,  0.0f,0.0f,1.0f,  0.0f,0.0f,1.0f,
			-0.5f, 0.5f, 0.0f,  1.0f,1.0f,0.0f,  0.0f,0.0f,1.0f,
		};
		std::cout << std::size(Vertices);
		unsigned int Indices[] = {
			0, 1, 3,
			1, 2, 3
		};
		unsigned int VBO, VAO, EBO;
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glm::vec3 CameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
		glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 Front = glm::vec3(0.0f, 0.0f, -1.0f);

		while (!glfwWindowShouldClose(m_pWindow))
		{
			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			LightingShader.use();
			LightingShader.setVec3("uViewPos", CameraPos);
			LightingShader.setFloat("uShininess", 32.0f);
			LightingShader.setFloat("uAmbientStrength", 0.1f);

			glm::mat4 ProjectionMat = glm::perspective(glm::radians(45.0f), (float)getWidth() / (float)getHeight(), 0.1f, 100.0f);
			glm::mat4 ViewMat = glm::lookAt(CameraPos, CameraPos + Front, Up);
			LightingShader.setMat4("uProjection", ProjectionMat);
			LightingShader.setMat4("uView", ViewMat);

			glm::mat4 model = glm::mat4(1.0f);
			LightingShader.setMat4("uModel", model);

			glBindVertexArray(VAO);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

			glfwSwapBuffers(m_pWindow);
			glfwPollEvents();
		}


		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
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
		std::vector<unsigned int> Indices={
			0, 1, 3,
			1, 2, 3
		};
		
		m_pVertex = std::make_shared<CVertexArrayBuffer>(Vertices, Indices, std::vector<int>{3,3,3});
	}
	
	void CRenderWindow::__deleteBind(unsigned int& vVBO, unsigned int& vVAO, unsigned int& vEBO)
	{
		glDeleteVertexArrays(1, &vVAO);
		glDeleteBuffers(1, &vVBO);
		glDeleteBuffers(1, &vEBO);
	}
}