#include "pch.h"
#include "RenderWindow.h"
#include <vector>
#include <optional>
#include <filesystem>
#include <tiny_gltf.h>
#include <stb_image.h>

#define TINYGLTF_MODE_DEFAULT -1 
#define TINYGLTF_MODE_POINT 0
#define TINYGLTF_MODE_TRIANGLE 4 
#define TINYGLTF_COMPONETTYPE_UNSHORT 5123
#define TINYGLTF_COMPONETTYPE_UNINT   5125

namespace openGLTask 
{
	CRenderWindow::CRenderWindow() : m_MajorVersion(3), m_MinorVersion(3), m_UseCoreProfile(false),
		m_pWindowConfig(nullptr), m_pWindow(nullptr), m_pVertexBuffer(nullptr), m_pShader(nullptr), m_pCamera(nullptr), m_pDirectionalLight(nullptr),
		m_pKeyBoardController(nullptr), m_PixelVertShaderPath("../shaders/vertPerPixelShading.glsl"), m_PixelFragShaderPath("../shaders/fragPerPixelShading.glsl"),
		m_VertexVertShaderPath("../shaders/vertPerVertexShading.glsl"),m_VertexFragShaderPath("../shaders/fragPerVertexShading.glsl"), m_GLTFPath("../models/dragon.gltf")
	{
	}

	bool CRenderWindow::__readXML(CRenderConfiguration& vConfig, const std::string& vXMLName)
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

	bool CRenderWindow::__initParametersFromXML()
	{
		CRenderConfiguration Config;
		if (!CRenderWindow::__readXML(Config, "../XML/config.xml")) 
		{
			std::cout << "false" << std::endl;
			return false;
		}
		/*const hiveConfig::CHiveConfig* pSubconfig = Config.getSubconfigAt(0);
		
		std::cout << pSubconfig->getAttribute<int>("Width").value();*/

		m_pWindowConfig = std::make_shared<CWindowConfig>();
		m_pWindowConfig->parseConfig(Config);

		std::optional<int> MajorVersion = Config.getAttribute<int>("MajorVersion");
		std::optional<int> MinorVersion = Config.getAttribute<int>("MinorVersion");
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
		
		__checkAndSetOpenGLVersion(MajorVersion, MinorVersion);
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

		m_pWindow = glfwCreateWindow(m_pWindowConfig->getWidth(), m_pWindowConfig->getHeight(), m_pWindowConfig->getWinName().c_str(), nullptr, nullptr);
		if (m_pWindow == nullptr)
		{
			HIVE_LOG_ERROR("Failed to create GLFW window");
			glfwTerminate();
			return m_pWindow;
		}

		glfwMakeContextCurrent(m_pWindow);
		glfwSetWindowPos(m_pWindow, m_pWindowConfig->getPosX(), m_pWindowConfig->getPosY());
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
		//__setAndBindTextureController();

		glEnable(GL_DEPTH_TEST);
		CShader GBufferShader = CShader("../shaders/gbuffer.vs","../shaders/gbuffer.fs");
		CShader LightShader = CShader("../shaders/light.vs","../shaders/light.fs");

		//// - Colors
		const GLuint NR_LIGHTS = 32;
		std::vector<glm::vec3> lightPositions;
		std::vector<glm::vec3> lightColors;
		
		srand(13);
		for (GLuint i = 0; i < NR_LIGHTS; i++)
		{
			// Calculate slightly random offsets
			GLfloat xPos = ((rand() % 100) / 100.0) * 6.0 - 3.0;
			GLfloat yPos = ((rand() % 100) / 100.0) * 6.0 - 4.0;
			GLfloat zPos = ((rand() % 100) / 100.0) * 6.0 - 3.0;
			lightPositions.push_back(glm::vec3(xPos, yPos, zPos));
			// Also calculate random color
			GLfloat rColor = ((rand() % 100) / 200.0f) + 0.5; // Between 0.5 and 1.0
			GLfloat gColor = ((rand() % 100) / 200.0f) + 0.5; // Between 0.5 and 1.0
			GLfloat bColor = ((rand() % 100) / 200.0f) + 0.5; // Between 0.5 and 1.0
			lightColors.push_back(glm::vec3(rColor, gColor, bColor));
		}
		//// positions all containers
		//glm::vec3 cubePositions[] = {
		//	glm::vec3(0.0f,  0.0f,  0.0f),
		//	glm::vec3(2.0f,  5.0f, -15.0f),
		//	glm::vec3(-1.5f, -2.2f, -2.5f),
		//	glm::vec3(-3.8f, -2.0f, -12.3f),
		//	glm::vec3(2.4f, -0.4f, -3.5f),
		//	glm::vec3(-1.7f,  3.0f, -7.5f),
		//	glm::vec3(1.3f, -2.0f, -2.5f),
		//	glm::vec3(1.5f,  2.0f, -2.5f),
		//	glm::vec3(1.5f,  0.2f, -1.5f),
		//	glm::vec3(-1.3f,  1.0f, -1.5f)
		//};
		GLuint gBuffer;
		glGenFramebuffers(1, &gBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
		GLuint gPosition, gNormal;
		// - Position color buffer
		glGenTextures(1, &gPosition);
		glBindTexture(GL_TEXTURE_2D, gPosition);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, m_pWindowConfig->getWidth(), m_pWindowConfig->getHeight(), 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);
		// - Normal color buffer
		glGenTextures(1, &gNormal);
		glBindTexture(GL_TEXTURE_2D, gNormal);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, m_pWindowConfig->getWidth(), m_pWindowConfig->getHeight(), 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);
		
		GLuint attachments[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
		glDrawBuffers(2, attachments);

		GLuint objDepth;
		glGenTextures(1, &objDepth);
		glBindTexture(GL_TEXTURE_2D, objDepth);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_pWindowConfig->getWidth(), m_pWindowConfig->getHeight(), 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, objDepth, 0);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "Framebuffer not complete!" << std::endl;
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		while (!glfwWindowShouldClose(m_pWindow))
		{
			glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
			glEnable(GL_DEPTH_TEST);
			glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glm::mat4 projection = glm::perspective(45.0f, (GLfloat)m_pWindowConfig->getWidth() / (GLfloat)m_pWindowConfig->getHeight(), 0.1f, 100.0f);
			glm::mat4 view = m_pCamera->getViewMatrix();
			glm::mat4 model= glm::mat4(1.0f);
			model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			GBufferShader.use();
			GBufferShader.setMat4("projection",projection);
			GBufferShader.setMat4("view", view);
			GBufferShader.setMat4("model", model);
			m_pVertexBuffer->draw();
			//for (unsigned int i = 0; i < 10; i++)
			//{
			//	// calculate the model matrix for each object and pass it to shader before drawing
			//	glm::mat4 model = glm::mat4(1.0f);
			//	model = glm::translate(model, cubePositions[i]);
			//	float angle = 20.0f * i;
			//	model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			//	GBufferShader.setMat4("model", model);
			//	m_pVertexBuffer->draw();
			//}

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glDisable(GL_DEPTH_TEST);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, gPosition);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, gNormal);
			// Also send light relevant uniforms
			LightShader.use();
			LightShader.setInt("gPosition", 0);
			LightShader.setInt("gNormal", 1);
			for (GLuint i = 0; i < lightPositions.size(); i++)
			{
				LightShader.setVec3(("lights[" + std::to_string(i) + "].Position").c_str(), lightPositions[i][0], lightPositions[i][1], lightPositions[i][2]);
				LightShader.setVec3(("lights[" + std::to_string(i) + "].Color").c_str(), lightColors[i][0], lightColors[i][1], lightColors[i][2]);
				// Update attenuation parameters and calculate radius
				const GLfloat constant = 1.0; // Note that we don't send this to the shader, we assume it is always 1.0 (in our case)
				const GLfloat linear = 0.7;
				const GLfloat quadratic = 1.8;
				LightShader.setFloat(("lights[" + std::to_string(i) + "].Linear").c_str(), linear);
				LightShader.setFloat(("lights[" + std::to_string(i) + "].Quadratic").c_str(), quadratic);
			}
			LightShader.setMat4("viewPos", view);
			__RenderQuad();
			//m_pVertexBuffer->draw();
			glfwSwapBuffers(m_pWindow);
			glfwPollEvents();
		}

		glfwTerminate();
		return ;
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
		//std::vector<float> Vertices= {
		//	// positions          // normals          
		//	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		//	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		//	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		//	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		//	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		//	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

		//	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		//	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		//	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		//	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		//	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		//	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

		//	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		//	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		//	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		//	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		//	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		//	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		//	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		//	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		//	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		//	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		//	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		//	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

		//	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		//	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		//	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		//	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		//	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		//	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

		//	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		//	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		//	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		//	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		//	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		//	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
		//};

		std::vector<unsigned int> Indices;
		__createVerticeAndIndice(GLTFModel, Vertices, Indices);
		m_pVertexBuffer = std::make_shared<CVertexBuffer>(Vertices, Indices ,std::vector<int>{3,3}, GL_TRIANGLES, GL_STATIC_DRAW);
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

	//void CRenderWindow::__setAndBindTextureController()
	//{
	//	m_pTextureController = std::make_shared<CTexture2D>();
	//}

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
	GLuint quadVAO = 0;
	GLuint quadVBO;
	void CRenderWindow::__RenderQuad()
	{
		if (quadVAO == 0)
		{
			GLfloat quadVertices[] = {
				// Positions        // Texture Coords
				-1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
				-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
				1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
				1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
			};
			// Setup plane VAO
			glGenVertexArrays(1, &quadVAO);
			glGenBuffers(1, &quadVBO);
			glBindVertexArray(quadVAO);
			glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		}
		glBindVertexArray(quadVAO);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0);
	}
}