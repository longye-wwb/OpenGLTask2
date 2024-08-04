#include "pch.h"
#include "DefferedGeoPass.h"
#include "Primitive.h"
#include "GameObject.h"
#include "Scene.h"
#include "Framebuffer.h"
#include "Shader.h"
#include "Camera.h"
#include "Mesh.h"

namespace openGLTask
{
	void CDefferedGeoPass::renderV(const std::shared_ptr<CScene>& vScene, const std::vector<std::shared_ptr<CFrameBuffer>>& vFrameBuffers, const std::vector<size_t> vOutputIndices, size_t vIdxOfPasses)
	{ 
		CRenderPass::renderV(vScene, vFrameBuffers, vOutputIndices, vIdxOfPasses);
		const auto& pCamera = vScene->getCamera();
		const auto& pLight = vScene->getDirectionalLight();
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		m_pShaderProgram->use();
		m_pShaderProgram->setMat4("uView", vScene->getCamera()->getViewMatrix());
		//m_pShaderProgram->setMat4("uView", glm::lookAt(glm::vec3(0.0f,10.0f,10.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
		m_pShaderProgram->setMat4("uProjection", vScene->getCamera()->getProjectionMatrix());
		//m_pShaderProgram->setMat4("uProjection", glm::perspective(glm::radians(45.0f), 4.0f/3.0f,0.1f, 100.0f));
		CGameObject::traverse(vScene->getRootNode(), [this](const std::shared_ptr<CGameObject>& vGameObject) {
			m_pShaderProgram->setMat4("uModel", vGameObject->getModelMatrix());
			for (const auto& pMesh : vGameObject->getMeshes())
			{
				//TODO:针对特定material的基本赋值
				pMesh->draw();
			}
			});
	}

}