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
		glClearColor(0.5f, 0.5f, 0.5f, 0.5f);
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		m_pShaderProgram->use();
		m_pShaderProgram->setMat4("uView", pCamera->getViewMatrix());
		m_pShaderProgram->setMat4("uProjection", pCamera->getProjectionMatrix());
		CGameObject::traverse(vScene->getRootNode(), [this](const std::shared_ptr<CGameObject>& vGameObject) {
			m_pShaderProgram->setMat4("uModel", vGameObject->getModelMatrix());
			for (const auto& pMesh : vGameObject->getMeshes())
			{
				//TODO:针对特定material的基本赋值 如pbR
				pMesh->draw();
			}
			});
	}

}