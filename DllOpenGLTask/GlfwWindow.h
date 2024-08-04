#pragma once
#include <memory>
#include <string>
#include <memory>
#include <GLFW/glfw3.h>
#include "dllFile.h"
#include "InputController.h"

namespace openGLTask
{
	class OPENGLTASK_API CGlfwWindow
	{
	public:
		CGlfwWindow();
		~CGlfwWindow();

		bool init(int vWidth, int vHeight, const std::string& vAppName = "Hello");
		bool shouldClose() const;
		void pollEvents() const;
		void swapBuffers() const;
		void addInputHandler(const std::shared_ptr<CInputController>& vInputHandler);
		int getWidth() const { return m_Width; }
		int getHeight() const { return m_Height; }

	private:
		void __cleanup();
		static void __mouseButtonCallback(GLFWwindow* vWindow, int vButton, int vAction, int vMods);
		static void __mouseScrollCallback(GLFWwindow* vWindow, double vXoffset, double vYoffset);
		static void __mouseMoveCallback(GLFWwindow* vWindow, double vXpos, double vYpos);
		static void __keyCallback(GLFWwindow* vWindow, int vKey, int vScancode, int vAction, int vMods);
		void __registerCallbacks() const;

		GLFWwindow* m_pWindow;
		int m_Width;
		int m_Height;
		std::vector<std::shared_ptr<CInputController>> m_InputHandlers;
	};
}

