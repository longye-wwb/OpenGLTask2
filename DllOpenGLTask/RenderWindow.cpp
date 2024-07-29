#include "pch.h"
#include "RenderWindow.h"
#include <vector>
#include <optional>
#include <filesystem>
#include <tiny_gltf.h>

#define TINYGLTF_MODE_DEFAULT -1 
#define TINYGLTF_MODE_POINT 0
#define TINYGLTF_MODE_TRIANGLE 4 
#define TINYGLTF_COMPONETTYPE_UNSHORT 5123
#define TINYGLTF_COMPONETTYPE_UNINT   5125

namespace openGLTask 
{
	CRenderWindow::CRenderWindow() : m_MajorVersion(3), m_MinorVersion(3), m_Width(800), m_Height(600), m_WinName("GLFW_Window"),
		m_PosX(10), m_PosY(10), m_UseCoreProfile(false), m_pWindow(nullptr), m_pVertexBuffer(nullptr), m_pShader(nullptr), m_pCamera(nullptr), m_pDirectionalLight(nullptr),
		m_pKeyBoardController(nullptr), m_PixelVertShaderPath("../shaders/vertPerPixelShading.glsl"), m_PixelFragShaderPath("../shaders/fragPerPixelShading.glsl"),
		m_VertexVertShaderPath("../shaders/vertPerVertexShading.glsl"),m_VertexFragShaderPath("../shaders/fragPerVertexShading.glsl"), m_GLTFPath("../models/dragon.gltf"),
		m_ScreenMaxWidth(1920), m_ScreenMaxHeight(1080)
	{
	}

	bool CRenderWindow::__initParametersFromXML()
	{
		CRenderConfiguration Config;
		if (!CRenderWindow::__readXML(Config, "../XML/config.xml")) 
		{
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
		std::optional<std::string> PixelVertShaderPath = Config.getAttribute<std::string>("shader_perpixel_shading_vs|SHADER_SOURCE_FILE").value();
		std::optional<std::string> PixelFragShaderPath = Config.getAttribute<std::string>("shader_perpixel_shading_fs|SHADER_SOURCE_FILE").value();
		std::optional<std::string> VtexVertShaderPath = Config.getAttribute<std::string>("shader_pervertex_shading_vs|SHADER_SOURCE_FILE").value();
		std::optional<std::string> VtexFragShaderPath = Config.getAttribute<std::string>("shader_pervertex_shading_fs|SHADER_SOURCE_FILE").value();
		std::optional<std::string> GLTFPath = Config.getAttribute<std::string>("GLTFPath").value();

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
		__checkAndSetShaderGLSL(PixelVertShaderPath, PixelFragShaderPath);
		__checkAndSetGLTFPath(GLTFPath);
		__checkAndBindCamera(CamPos, CameraFront, CameraUp);
		__checkAndSetLightDirection(LightDirection);
		return true;
	}

	GLFWwindow* CRenderWindow::__createWindow() 
	{
		if (!__initParametersFromXML()) 
		{
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
		glfwSetWindowUserPointer(m_pWindow, this);
		glfwSetKeyCallback(m_pWindow, [](GLFWwindow* vWindow, int key, int scancode, int action, int mods)
			{
				if (action == GLFW_PRESS)
				{
					auto pRenderWindow = (CRenderWindow*)glfwGetWindowUserPointer(vWindow);
					pRenderWindow->getKeyBoardInput()->onKeyDown(key);
				}
			});
		return m_pWindow;
	}

	void CRenderWindow::startRun(std::function<glm::vec3(std::shared_ptr<openGLTask::CDirectionalLight>)> vFunCallback)
	{
		GLFWwindow* m_pWindow = __createWindow();
		if (m_pWindow == nullptr) 
		{
			HIVE_LOG_ERROR("Window is not initialized!");
			return;
		}
		HIVE_LOG_INFO("GLAD : {}", gladLoadGLLoader((GLADloadproc)glfwGetProcAddress));

		__setAndBindVertices();
		__setAndBindKeyInputController();

		glEnable(GL_DEPTH_TEST);
		while (!glfwWindowShouldClose(m_pWindow))
		{
			glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			__setAndBindShader();
			m_pShader->use();
			m_pShader->setVec3("uViewPos", m_pCamera->getWorldPos());
			m_pShader->setFloat("uShininess", 32.0f);
			m_pShader->setFloat("uAmbientStrength", 0.1f);
			if (!m_pKeyBoardController->getEState())
			{
				m_pShader->setVec3("uDirection", vFunCallback(m_pDirectionalLight));
			}
			else
			{
				m_pShader->setVec3("uDirection", m_pDirectionalLight->getDirection());
			}
			glm::mat4 ProjectionMat = glm::perspective(glm::radians(45.0f), (float)getWidth() / (float)getHeight(), 0.1f, 100.0f);
			glm::mat4 ViewMat = m_pCamera->getViewMatrix();
			m_pShader->setMat4("uProjection", ProjectionMat);
			m_pShader->setMat4("uView", ViewMat);
			glm::mat4 Model = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0, 0.0, 0.0));
			m_pShader->setMat4("uModel", Model);

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
	
	bool CRenderWindow::__loadGLTF(const std::string& vFilename, tinygltf::Model& vModelGLTF)
	{
		tinygltf::TinyGLTF Loader;
		std::string Err;
		std::string Warn;

		bool res = Loader.LoadASCIIFromFile(&vModelGLTF, &Err, &Warn, vFilename);

		if (!Warn.empty()) {
			HIVE_LOG_WARNING("WARN: {}", Warn);
		}

		if (!Err.empty()) {
			HIVE_LOG_ERROR("ERR: {}", Err);
		}

		if (!res) {
			HIVE_LOG_ERROR("Failed to load glTF: {}", vFilename);
		}
		else {
			HIVE_LOG_INFO("Loaded glTF: {}", vFilename);
		}

		return res;
	}

	void CRenderWindow::__createIndiceBufferData(std::vector<unsigned int>& vIndices, const tinygltf::BufferView& vBufferView, const tinygltf::Buffer& vBuffer, const int& vComponentType)
	{
		unsigned short tempUShortIndice;
		unsigned int   tempUIntIndice;
		const int UnShortByte = 2;
		const int UnIntByte = 4;
		if (vComponentType == TINYGLTF_COMPONETTYPE_UNSHORT)
		{
			for (size_t i = vBufferView.byteOffset; i < vBufferView.byteOffset + vBufferView.byteLength; i += UnShortByte) {
				std::memcpy(&tempUShortIndice, &vBuffer.data.at(i), sizeof(unsigned short));
				vIndices.push_back(tempUShortIndice);
			}
		}
		else if (vComponentType == TINYGLTF_COMPONETTYPE_UNINT) 
		{
			for (size_t i = vBufferView.byteOffset; i < vBufferView.byteOffset + vBufferView.byteLength; i += UnIntByte) {
				std::memcpy(&tempUIntIndice, &vBuffer.data.at(i), sizeof(unsigned int));
				vIndices.push_back(tempUIntIndice);
			}
		}
	}

	void CRenderWindow::__createVertexBufferData(std::vector<float>& vVertices, const tinygltf::Buffer& vBuffer, const int vIndex) {
		float tempVertice;
		const int FloatByte = 4;
		const int FloatNum = 3;
		for (auto i = vIndex; i < vIndex + FloatNum * FloatByte; i += FloatByte) 
		{
			std::memcpy(&tempVertice, &vBuffer.data.at(i), sizeof(float));
			vVertices.push_back(tempVertice);
		}
	}

	void CRenderWindow::__createVerticeAndIndice(tinygltf::Model& vGLTFModel, std::vector<float>& vioVertices, std::vector<unsigned int>& vioIndices)
	{
		for (auto& node : vGLTFModel.nodes) 
		{
			if (node.mesh == -1) continue;
			const auto& Mesh = vGLTFModel.meshes[node.mesh];
			std::string MeshName = Mesh.name;
			HIVE_LOG_INFO("MeshName : {}", MeshName);

			for (auto& primitive : Mesh.primitives)
			{
				vioVertices.clear();
				if (primitive.mode == TINYGLTF_MODE_POINT) 
				{
					const tinygltf::Accessor& AccessorPos = vGLTFModel.accessors[primitive.attributes.at("POSITION")];
					const tinygltf::BufferView& BufferViewPos = vGLTFModel.bufferViews[AccessorPos.bufferView];
					const tinygltf::Buffer& BufferPos = vGLTFModel.buffers[BufferViewPos.buffer];
					const tinygltf::Accessor& AccessorColor = vGLTFModel.accessors[primitive.attributes.at("COLOR_0")];
					const tinygltf::BufferView& BufferViewColor = vGLTFModel.bufferViews[AccessorColor.bufferView];
					const tinygltf::Buffer& BufferColor = vGLTFModel.buffers[BufferViewColor.buffer];
					glm::vec3 MinPos(AccessorPos.minValues[0], AccessorPos.minValues[1], AccessorPos.minValues[2]);
					glm::vec3 MaxPos(AccessorPos.maxValues[0], AccessorPos.maxValues[1], AccessorPos.maxValues[2]);

					const int Vec3Byte = 12;
					for (size_t i = BufferViewPos.byteOffset, k = BufferViewColor.byteOffset;
						(i < BufferViewPos.byteOffset + BufferViewPos.byteLength && k < BufferViewColor.byteOffset + BufferViewColor.byteLength);
						i += Vec3Byte, k += Vec3Byte) 
					{
						__createVertexBufferData(vioVertices, BufferPos, (int)i);
						__createVertexBufferData(vioVertices, BufferColor, (int)k);
					}

					HIVE_LOG_INFO("Vertices.size : {}", vioVertices.size());
					assert(vioVertices.size() == vGLTFModel.accessors[primitive.attributes.at("POSITION")].count * 3 * 2);
				}
				else if (primitive.mode == TINYGLTF_MODE_TRIANGLE || primitive.mode == TINYGLTF_MODE_DEFAULT) 
				{
					vioVertices.clear();
					vioIndices.clear();
					const tinygltf::BufferView& BufferViewIndice = vGLTFModel.bufferViews[vGLTFModel.accessors[primitive.indices].bufferView];
					const tinygltf::Buffer& BufferIndice = vGLTFModel.buffers[BufferViewIndice.buffer];
					const int IndiceComponentType = vGLTFModel.accessors[primitive.indices].componentType;

					__createIndiceBufferData(vioIndices, BufferViewIndice, BufferIndice, IndiceComponentType);
					HIVE_LOG_INFO("indice.size : {}", vioIndices.size());
					assert(vioIndices.size() == vGLTFModel.accessors[primitive.indices].count);

					const tinygltf::BufferView& BufferViewPos = vGLTFModel.bufferViews[vGLTFModel.accessors[primitive.attributes.at("POSITION")].bufferView];
					const tinygltf::Buffer& BufferPos = vGLTFModel.buffers[BufferViewPos.buffer];
					const tinygltf::BufferView& BufferViewNor = vGLTFModel.bufferViews[vGLTFModel.accessors[primitive.attributes.at("NORMAL")].bufferView];
					const tinygltf::Buffer& BufferNor = vGLTFModel.buffers[BufferViewNor.buffer];

					assert(BufferViewPos.byteLength == BufferViewNor.byteLength);

					const int Vec3Byte = 12;
					for (std::size_t i = BufferViewPos.byteOffset, k = BufferViewNor.byteOffset;
						(i < BufferViewPos.byteOffset + BufferViewPos.byteLength && k < BufferViewNor.byteOffset + BufferViewNor.byteLength);
						i += Vec3Byte, k += Vec3Byte) 
					{
						__createVertexBufferData(vioVertices, BufferPos, (int)i);
						__createVertexBufferData(vioVertices, BufferNor, (int)k);
					}
					HIVE_LOG_INFO("Vertices.size : {}", vioVertices.size());
					assert(vioVertices.size() == vGLTFModel.accessors[primitive.attributes.at("POSITION")].count * 6);
				}
			}
		}
		return;
	}

	void CRenderWindow::__setAndBindVertices()
	{
		tinygltf::Model GLTFModel;
		__loadGLTF(m_GLTFPath, GLTFModel);
		
		std::vector<float> Vertices;
		std::vector<unsigned int> Indices;
		__createVerticeAndIndice(GLTFModel, Vertices, Indices);
		m_pVertexBuffer = std::make_shared<CVertexBuffer>(Vertices, Indices, std::vector<int>{3, 3}, GL_TRIANGLES, GL_STATIC_DRAW);
	}

	void CRenderWindow::__setAndBindShader()
	{
		if (m_pKeyBoardController->getQState())
		{
			m_pShader = std::make_shared<CShader>(m_PixelVertShaderPath.c_str(), m_PixelFragShaderPath.c_str());
		}
		else
		{
			m_pShader = std::make_shared<CShader>(m_VertexVertShaderPath.c_str(), m_VertexFragShaderPath.c_str());
		}
	}

	void CRenderWindow::__setAndBindKeyInputController()
	{
		m_pKeyBoardController = std::make_shared<CkeyBoardInput>();
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
		m_pDirectionalLight = std::make_shared<CDirectionalLight>();
		//TODO:
		if (vLightDirection.has_value())
		{
			m_pDirectionalLight->setDirection(glm::vec3(std::get<0>(vLightDirection.value()), std::get<1>(vLightDirection.value()), std::get<2>(vLightDirection.value())));
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

	void CRenderWindow::__checkAndSetShaderGLSL(std::optional<std::string> vPixelVertShaderPath, std::optional<std::string> vPixelFragShaderPath)
	{
		if (vPixelVertShaderPath.has_value() && vPixelFragShaderPath.has_value() && std::filesystem::exists(vPixelVertShaderPath.value()) && std::filesystem::exists(vPixelFragShaderPath.value()))
		{
			m_PixelVertShaderPath = vPixelVertShaderPath.value();
			m_PixelFragShaderPath = vPixelFragShaderPath.value();
		}
		else
		{
			HIVE_LOG_ERROR("ShaderGLSL path is invalid, use default path.");
		}
		return;
	}

	void CRenderWindow::__checkAndSetGLTFPath(std::optional<std::string> vGLTFPath)
	{
		if (vGLTFPath.has_value() && std::filesystem::exists(vGLTFPath.value()) )
		{
			m_GLTFPath = vGLTFPath.value();
		}
		else
		{
			HIVE_LOG_ERROR("GLTF path is invalid, use default path.");
		}
		return;
	}
	
	bool CRenderWindow::__isOpenGLVersionValid(std::optional<int> vMajorVersion, std::optional<int> vMinorVersion) 
	{
		if (!vMajorVersion.has_value() || !vMinorVersion.has_value()) 
		{
			return false;
		}

		int Major = vMajorVersion.value();
		int Minor = vMinorVersion.value();

		switch (Major) 
		{
		case 1:
			switch (Minor) 
			{
			case 0:
			case 1:
				return true;
			default:
				return false;
			}
		case 2:
			switch (Minor) 
			{
			case 0:
			case 1:
				return true;
			default:
				return false;
			}
		case 3:
			switch (Minor) 
			{
			case 0:
			case 1:
			case 2:
			case 3:
				return true;
			default:
				return false;
			}
		case 4:
			switch (Minor) 
			{
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

	void CRenderWindow::__setAndGetScreenSize() 
	{
		glfwInit();
		GLFWmonitor* pMonitor = glfwGetPrimaryMonitor();
		if (pMonitor == nullptr) 
		{
			HIVE_LOG_WARNING("Can't Get Monitor");
			return;
		}
		const GLFWvidmode* pMode = glfwGetVideoMode(pMonitor);
		m_ScreenMaxWidth = pMode->width;
		m_ScreenMaxHeight = pMode->height;
	}
}