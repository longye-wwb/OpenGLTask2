#include <iostream>
#include <LoggingCore.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <memory>
#include "RenderPipeline.h"
#include "GlfwWindow.h"
#include "Camera.h"
#include "Scene.h"
#include "DefferedGeoPass.h"
#include "DefferedLitPass.h"
#include "FpsController.h"
#include "FrameBuffer.h"
#include "Primitive.h"
#include "GameObject.h"
#include "Light.h"
#include "Mesh.h"
#include "ModelLoader.h"
#include "Shader.h"


std::shared_ptr<openGLTask::CScene> g_Scene = nullptr;
std::shared_ptr<openGLTask::CRenderPipeline> g_RenderPipeline = nullptr;

std::shared_ptr<openGLTask::CGameObject> createGameObject(const std::shared_ptr<openGLTask::CVertexBuffer>& vVAO)
{
	const auto& pNode = std::make_shared<openGLTask::CGameObject>();
	pNode->addMesh(std::make_shared<openGLTask::CMesh>(vVAO));
	return pNode;
}
std::shared_ptr<openGLTask::CFrameBuffer> createColorAndDepthFrameBuffer(int vWidth, int vHeight, const std::vector<int>& vColorBufferChannels)
{
	int NumOfColorBuffers = (int)vColorBufferChannels.size();
	_ASSERTE(vWidth > 0 && vHeight > 0);
	_ASSERTE(NumOfColorBuffers > 0 && NumOfColorBuffers <= 32);

	const auto& pFrameBuffer = std::make_shared<openGLTask::CFrameBuffer>();
	pFrameBuffer->create();
	pFrameBuffer->bind();

	// color buffer
	std::vector<GLenum> DrawAttachments{};
	for (int i = 0; i < NumOfColorBuffers; ++i)
	{
		std::shared_ptr<openGLTask::CTexture2D> pTexture2D = nullptr;
		if (vColorBufferChannels[i] == 3)
		{
			pTexture2D = std::make_shared<openGLTask::CTexture2D>(vWidth, vHeight, GL_RGB16F, GL_RGB, GL_FLOAT);
		}
		else if (vColorBufferChannels[i] == 4)
		{
			pTexture2D = std::make_shared<openGLTask::CTexture2D>(vWidth, vHeight, GL_RGBA16F, GL_RGBA, GL_FLOAT);
		}
		else
		{
			HIVE_LOG_INFO("curr only support channel == 3 or channel == 4");
			continue;
		}
		pTexture2D->setTextureParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		pTexture2D->setTextureParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		pFrameBuffer->setAttachment(GL_COLOR_ATTACHMENT0 + i, pTexture2D, 0);
		DrawAttachments.push_back(GL_COLOR_ATTACHMENT0 + i);
	}
	pFrameBuffer->drawAttachments(DrawAttachments);

	// depth buffer
	const auto& pDepthTex = std::make_shared<openGLTask::CTexture2D>(vWidth, vHeight, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_FLOAT);
	pDepthTex->setTextureParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	pDepthTex->setTextureParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	pFrameBuffer->setAttachment(GL_DEPTH_ATTACHMENT, pDepthTex, 0);

	openGLTask::CFrameBuffer::checkComplete();
	pFrameBuffer->unbind();
	return pFrameBuffer;
}
void setRenderPipeline(int vWidth, int vHeight)
{
	const auto& pDeferredGeoPass = std::make_shared<openGLTask::CDefferedGeoPass>(std::make_shared<openGLTask::CShader>("../shaders/geobuffer.vs",
		"../shaders/geobuffer.fs"));
	const auto& pDeferredLitPass = std::make_shared<openGLTask::CDeferredLitPass>(std::make_shared<openGLTask::CShader>("../shaders/lighting.vs",
		"../shaders/lighting.fs"),0);

	g_RenderPipeline = std::make_shared<openGLTask::CRenderPipeline>();
	g_RenderPipeline->addFrameBuffer(createColorAndDepthFrameBuffer(vWidth, vHeight, std::vector<int>(2,3)));
	g_RenderPipeline->addFrameBuffer(openGLTask::CFrameBuffer::getDefaultFrameBuffer());
	g_RenderPipeline->addRenderPass(pDeferredGeoPass, 0);
	g_RenderPipeline->addRenderPass(pDeferredLitPass, 1);
}
int main()
{
    std::string XMLPath = "../XML/config.xml";
	openGLTask::CGlfwWindow Exe;
	_ASSERT(Exe.init(800, 600));
	openGLTask::CFrameBuffer::initDefaultFrameBuffer(Exe.getWidth(), Exe.getHeight(), 0);
    //TODO:在生成窗口的时候 传入xml文件 顺便检查数据的合理性

	const auto& pPlaneGameObject = createGameObject(openGLTask::CPrimitive::createPlane());
	pPlaneGameObject->setScale(glm::vec3(10.0f, 10.0f, 10.0f));
	pPlaneGameObject->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	const auto& pRootGameObject= std::make_shared<openGLTask::CGameObject>();
	pRootGameObject->addChild(pPlaneGameObject);
	//const auto& pDragonGameObject = openGLTask::CModelLoader::loadGltfFile("../models/dragon.gltf");
	//pRootGameObject->addChild(pDragonGameObject);

	const auto& pCamera = std::make_shared<openGLTask::CCamera>((float)Exe.getWidth() / (float)Exe.getHeight());
	const auto& pCameraController = std::make_shared<openGLTask::CFPSController>();
	pCameraController->control(pCamera);
	Exe.addInputHandler(pCameraController);

	const auto& pDirLight = std::make_shared<openGLTask::SDirectionalLight>();
	pDirLight->_LightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	pDirLight->_LightIntensity = 10.0f;
	pDirLight->_LightPos = glm::vec3(0.0f, 0.0f, 0.0f);
	//pDirLight->_LightDir = glm::vec3(0.0f, 0.0f, 0.0f) - pDirLight->_LightPos;
	pDirLight->_LightDir = glm::vec3(1.0f, 0.0f, 0.0f);

	const auto& pPointLight = std::make_shared<openGLTask::SPointLight>();
	pPointLight->_LightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	pPointLight->_LightIntensity = 1000.0f;
	pPointLight->_LightPos = glm::vec3(0.0f, 0.0f, 5.0f);

	g_Scene = std::make_shared<openGLTask::CScene>();
	g_Scene->setCamera(pCamera);
	g_Scene->setDirectionalLight(pDirLight);
	g_Scene->setPointLight(pPointLight);
	g_Scene->setRootNode(pRootGameObject);

	setRenderPipeline(Exe.getWidth(), Exe.getHeight());

	float LastTime = 0.0f;
	while (!Exe.shouldClose())
	{
		Exe.pollEvents();
		const float CurrTime = static_cast<float>(glfwGetTime());
		const float DeltaTime = CurrTime - LastTime;
		LastTime = CurrTime;
		pCameraController->update(DeltaTime);
		g_RenderPipeline->render(g_Scene);
		Exe.swapBuffers();
	}
	g_Scene.reset();
	g_RenderPipeline.reset();
	return 0;
}   
