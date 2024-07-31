#pragma once
#include "UtilityInterface.h"
#include "ConfigInterface.h"
#include "RenderConfiguration.h"
#include "DirectionalLight.h"
#include "WindowConfig.h"
#include "Shader.h"
#include "Camera.h"
#include "VertexBuffer.h"
#include "keyBoardInput.h"
#include "Texture2D.h"
#include <tiny_gltf.h>

namespace openGLTask 
{
	class OPENGLTASK_API CRenderWindow
	{	
	public:
		CRenderWindow();

		bool m_ShaderState = false;
		void startRun(std::function<glm::vec3(std::shared_ptr<openGLTask::CDirectionalLight>)> vFunCallback);
		int getMajorVersion() { return m_MajorVersion; }
		int getMinorVersion() { return m_MinorVersion; }
		bool getUseCoreProfile() { return m_UseCoreProfile; }
		const std::shared_ptr<CkeyBoardInput>& getKeyBoardInput() const { return m_pKeyBoardController; }

	private:
		GLFWwindow* __createWindow();
		bool __initParametersFromXML();
		bool __readXML(CRenderConfiguration& vConfig, const std::string& vXMLName);
		bool __loadGLTF(const std::string& vFilename, tinygltf::Model& vModelGLTF);
		void __createVerticeAndIndice(tinygltf::Model& vGLTFModel, std::vector<float>& vioVertices, std::vector<unsigned int>& vioIndices);
		void __createVertexBufferData(std::vector<float>& vVertices, const tinygltf::Buffer& vBuffer, const int vIndex);
		void __createIndiceBufferData(std::vector<unsigned int>& vIndices, const tinygltf::BufferView& vBufferView, const tinygltf::Buffer& vBuffer, const int& vComponentType);
		void __setAndBindVertices();
		void __setAndBindShader();
		void __setAndBindKeyInputController();
		void __setAndBindTextureController();
		void __checkAndBindCamera(std::optional<std::tuple<double, double, double>> vCameraPos, std::optional<std::tuple<double, double, double>> vCameraFront, std::optional<std::tuple<double, double, double>> vCameraUp);
		void __checkAndSetLightDirection(std::optional<std::tuple<double, double, double>> vLightDirection);
		void __checkAndSetOpenGLVersion(std::optional<int> vMajorVersion, std::optional<int> vMinorVersion);
		void __checkAndSetProfile(std::optional<bool> vUseCoreProfile);
		void __checkAndSetShaderGLSL(std::optional<std::string> vVertShaderPath, std::optional<std::string> vFragShaderPath);
		void __checkAndSetGLTFPath(std::optional<std::string> vGLTFPath);
		bool __isOpenGLVersionValid(std::optional<int> vMajorVersion, std::optional<int> vMinorVersion);
		void __RenderQuad();

		int m_MajorVersion;
		int m_MinorVersion;
		bool m_UseCoreProfile;
		std::string m_PixelVertShaderPath;
		std::string m_PixelFragShaderPath;
		std::string m_VertexVertShaderPath;
		std::string m_VertexFragShaderPath;
		std::string m_GLTFPath;
		
		GLFWwindow* m_pWindow = nullptr;
		std::shared_ptr<CWindowConfig> m_pWindowConfig = nullptr;
		std::shared_ptr<CVertexBuffer> m_pVertexBuffer = nullptr;
		std::shared_ptr<CShader> m_pShader = nullptr;
		std::shared_ptr<CCamera> m_pCamera = nullptr;
		std::shared_ptr<CDirectionalLight> m_pDirectionalLight = nullptr;
		std::shared_ptr<CkeyBoardInput> m_pKeyBoardController = nullptr;
		std::shared_ptr<CTexture2D> m_pTextureController = nullptr;
	};
}
