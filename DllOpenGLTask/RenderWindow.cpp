#include "pch.h"
#include "RenderWindow.h"
#include "shader_s.h"

namespace openGLTask {

	CRenderWindow::CRenderWindow() : m_MajorVersion(3), m_MinorVersion(3), m_Width(800), m_Height(600),
		m_PosX(0), m_PosY(0), m_UseCoreProfile(true), m_pWindow(nullptr), m_WinName("GLFW_Window"),
		m_ScreenMaxWidth(1920),m_ScreenMaxHeight(1080)
	{
	}

	bool CRenderWindow::__init()
	{
		CRenderConfiguration Config;
		if (!CRenderWindow::__readXML(Config, "./config/config.xml")) {
			HIVE_LOG_ERROR("Can't Read Config,use default value");
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
		__setAndGetScreenSize();
		__checkAndSetWindowSize(Width, Height);
		__checkAndSetWindowPos(PosX, PosY);
		__checkAndSetOpenGLVersion(MajorVersion, MinorVersion);
		__checkAndSetWinName(WinName);
		__checkAndSetProfile(UseCoreProfile);
		return true;
	}

	GLFWwindow* CRenderWindow::createWindow() {
		if (!__init()) {
			HIVE_LOG_ERROR("Can't Init");
		}
		if (m_pWindow!=nullptr)
		{
			HIVE_LOG_ERROR("Only one window can exist.");
			return m_pWindow;
		}
		//init()函数里执行了glfwinit()
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, getMajorVersion());
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, getMinorVersion());
		glfwWindowHint(GLFW_OPENGL_PROFILE, getUseCoreProfile()? GLFW_OPENGL_CORE_PROFILE:GLFW_OPENGL_COMPAT_PROFILE);
		
		m_pWindow = glfwCreateWindow(getWidth(), getHeight(),getWinName().c_str(), nullptr, nullptr);
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

	void CRenderWindow::startRun(std::function<void()> vFuncTickOnceLoop)
	{
		GLFWwindow* m_pWindow = createWindow();
		
		if (m_pWindow == nullptr) {
			HIVE_LOG_ERROR("Window is not initialized!");
			return;
		}

		HIVE_LOG_INFO("GLAD : {}", gladLoadGLLoader((GLADloadproc)glfwGetProcAddress));
		unsigned int VBO, VAO, EBO;
		__setAndBindVertices(VBO, VAO, EBO);
		__renderLoop(vFuncTickOnceLoop, VAO);
		__deleteBind(VBO, VAO, EBO);
		glfwTerminate();
		return ;
	}

	bool CRenderWindow::__readXML(CRenderConfiguration& vConfig,const std::string& vXMLName)
	{
		//hiveUtility::hiveAddFileSearchPath("./");
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

	void CRenderWindow::__renderLoop(std::function<void()> vFuncTickOnceLoop, unsigned int& vVAO)
	{
		
		while (!glfwWindowShouldClose(m_pWindow))
		{
			if (vFuncTickOnceLoop != nullptr) {
				vFuncTickOnceLoop();
				glBindVertexArray(vVAO);
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			};
			glfwSwapBuffers(m_pWindow);
			glfwPollEvents();
		}
	}

	void CRenderWindow::__setAndBindVertices(unsigned int& vVBO, unsigned int& vVAO,unsigned int& vEBO)
	{
		float Vertices[] = {
			//Verices              Color             Normal
			 0.5f, 0.5f, 0.0f,  1.0f,0.0f,0.0f,  0.0f,0.0f,1.0f,
			 0.5f,-0.5f, 0.0f,  0.0f,1.0f,0.0f,  0.0f,0.0f,1.0f,
			-0.5f,-0.5f, 0.0f,  0.0f,0.0f,1.0f,  0.0f,0.0f,1.0f,
			-0.5f, 0.5f, 0.0f,  1.0f,1.0f,0.0f,  0.0f,0.0f,1.0f,
		};

		unsigned int Indices[] = {
			0, 1, 3,
			1, 2, 3
		};
		
		glGenVertexArrays(1, &vVAO);
		glGenBuffers(1, &vVBO);
		glGenBuffers(1, &vEBO);

		glBindVertexArray(vVAO);

		glBindBuffer(GL_ARRAY_BUFFER, vVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);

		glVertexAttribPointer(LAYOUT, VEC3SIZE, GL_FLOAT, GL_FALSE, STRIDE*sizeof(float), (void*)OFFSET);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(LAYOUT+1, VEC3SIZE, GL_FLOAT, GL_FALSE, STRIDE*sizeof(float), (void*)(OFFSET+3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(LAYOUT+2, VEC3SIZE, GL_FLOAT, GL_FALSE, STRIDE*sizeof(float), (void*)(OFFSET+6 * sizeof(float)));
		glEnableVertexAttribArray(2);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	
	void CRenderWindow::__deleteBind(unsigned int& vVBO, unsigned int& vVAO, unsigned int& vEBO)
	{
		glDeleteVertexArrays(1, &vVAO);
		glDeleteBuffers(1, &vVBO);
		glDeleteBuffers(1, &vEBO);
	}
}