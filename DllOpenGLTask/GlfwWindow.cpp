#include "pch.h"
#include "GlfwWindow.h"

namespace openGLTask
{
	CGlfwWindow::CGlfwWindow() :m_pWindow(nullptr), m_Width(0), m_Height(0)
	{
	}

	CGlfwWindow::~CGlfwWindow()
	{
		__cleanup();
	}

	bool CGlfwWindow::init(int vWidth, int vHeight, const std::string& vAppName)
	{
		if (m_pWindow != nullptr)
		{
			HIVE_LOG_ERROR("window has been initialized");
			return false;
		}
		if (vWidth <= 0 || vHeight <= 0)
		{
			HIVE_LOG_ERROR("window size must be greater than 0");
			return false;
		}
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		m_pWindow = glfwCreateWindow(vWidth, vHeight, vAppName.c_str(), nullptr, nullptr);
		if (m_pWindow == nullptr)
		{
			glfwTerminate();
			HIVE_LOG_ERROR("failed to create GLFW window");
			return false;
		}
		glfwGetFramebufferSize(m_pWindow, &m_Width, &m_Height);
		glfwSetWindowUserPointer(m_pWindow, this);
		__registerCallbacks();
		glfwMakeContextCurrent(m_pWindow);
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			__cleanup();
			HIVE_LOG_ERROR("failed to initialize GLAD");
			return false;
		}
		return true;
	}

	bool CGlfwWindow::shouldClose() const
	{
		_ASSERTE(m_pWindow != nullptr);
		return glfwWindowShouldClose(m_pWindow);
	}

	void CGlfwWindow::pollEvents() const
	{
		glfwPollEvents();
	}

	void CGlfwWindow::swapBuffers() const
	{
		_ASSERTE(m_pWindow != nullptr);
		glfwSwapBuffers(m_pWindow);
	}
	void CGlfwWindow::addInputHandler(const std::shared_ptr<CInputController>& vInputHandler)
	{
		if (vInputHandler == nullptr) return;
		m_InputHandlers.push_back(vInputHandler);
	}

	void CGlfwWindow::__cleanup()
	{
		if (m_pWindow != nullptr)
		{
			glfwDestroyWindow(m_pWindow);
			m_pWindow = nullptr;
		}
		glfwTerminate();
	}

	void CGlfwWindow::__mouseButtonCallback(GLFWwindow* vWindow, int vButton, int vAction, int vMods)
	{
		CGlfwWindow* pApp = (CGlfwWindow*)glfwGetWindowUserPointer(vWindow);
		for (const auto& pInputHandler : pApp->m_InputHandlers)
		{
			if (pInputHandler == nullptr) continue;
			if (vAction == GLFW_PRESS) pInputHandler->onMouseButtonDown(vButton);
			else if (vAction == GLFW_RELEASE) pInputHandler->onMouseButtonUp(vButton);
		}
	}

	void CGlfwWindow::__mouseScrollCallback(GLFWwindow* vWindow, double vXoffset, double vYoffset)
	{
		CGlfwWindow* pApp = (CGlfwWindow*)glfwGetWindowUserPointer(vWindow);
		for (const auto& pInputHandler : pApp->m_InputHandlers)
		{
			if (pInputHandler == nullptr) continue;
			pInputHandler->onMouseScroll((float)vXoffset, (float)vYoffset);
		}
	}

	void CGlfwWindow::__mouseMoveCallback(GLFWwindow* vWindow, double vXpos, double vYpos)
	{
		CGlfwWindow* pApp = (CGlfwWindow*)glfwGetWindowUserPointer(vWindow);
		for (const auto& pInputHandler : pApp->m_InputHandlers)
		{
			if (pInputHandler == nullptr) continue;
			pInputHandler->onMouseMove((float)vXpos, (float)vYpos);
		}
	}

	void CGlfwWindow::__keyCallback(GLFWwindow* vWindow, int vKey, int vScancode, int vAction, int vMods)
	{
		CGlfwWindow* pApp = (CGlfwWindow*)glfwGetWindowUserPointer(vWindow);
		for (const auto& pInputHandler : pApp->m_InputHandlers)
		{
			if (pInputHandler == nullptr) continue;
			if (vAction == GLFW_PRESS) pInputHandler->onKeyDown(vKey);
			else if (vAction == GLFW_RELEASE) pInputHandler->onKeyUp(vKey);
		}
	}
	void CGlfwWindow::__registerCallbacks() const
	{
		glfwSetMouseButtonCallback(m_pWindow, __mouseButtonCallback);
		glfwSetScrollCallback(m_pWindow, __mouseScrollCallback);
		glfwSetCursorPosCallback(m_pWindow, __mouseMoveCallback);
		glfwSetKeyCallback(m_pWindow, __keyCallback);
	}

}
