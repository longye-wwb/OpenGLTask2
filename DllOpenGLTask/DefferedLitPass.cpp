#include "pch.h"
#include "DefferedLitPass.h"
#include "Texture2D.h"
#include "Shader.h"
#include "Framebuffer.h"
#include "VertexBuffer.h"
#include "Camera.h"
#include "Scene.h"
#include "Light.h"
#include "Primitive.h"
#include "Scene.h"

namespace openGLTask 
{
	CDeferredLitPass::CDeferredLitPass(
		const std::shared_ptr<CShader>& vShaderProgram,
		size_t vIdxOfDeferredGeoFB//, size_t vIdxOfDirShadowMapFB
		//,const std::shared_ptr<CDirShadowMapPass>& vDirShadowMapPass
	)
		:CRenderPass(vShaderProgram), m_pQuadVAO(CPrimitive::createQuad()),
		m_IdxOfDeferredGeoFB(vIdxOfDeferredGeoFB)//, m_IdxOfDirShadowMapFB(vIdxOfDirShadowMapFB)
		//, m_pDirShadowMapPass(vDirShadowMapPass)
	{
	}

	CDeferredLitPass::~CDeferredLitPass()
	{
		m_pQuadVAO.reset();
		//m_pDirShadowMapPass.reset();
	}

	void CDeferredLitPass::renderV(const std::shared_ptr<CScene>& vScene, const std::vector<std::shared_ptr<CFrameBuffer>>& vFrameBuffers, const std::vector<size_t> vOutputIndices, size_t vIdxOfPasses)
	{
		CRenderPass::renderV(vScene, vFrameBuffers, vOutputIndices, vIdxOfPasses);
		const auto& pCamera = vScene->getCamera();
		const auto& pDirLight = vScene->getDirectionalLight();
		glm::vec4 SolidColor = pCamera->getSolidColor();
		glDisable(GL_DEPTH_TEST);
		glClearColor(SolidColor.x, SolidColor.y, SolidColor.z, SolidColor.w);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		const auto& pDeferredGeoFB = vFrameBuffers[m_IdxOfDeferredGeoFB];

		glActiveTexture(GL_TEXTURE0);
		pDeferredGeoFB->getAttachment(GL_COLOR_ATTACHMENT0)->bind();
		glActiveTexture(GL_TEXTURE1);
		pDeferredGeoFB->getAttachment(GL_COLOR_ATTACHMENT1)->bind();
		glActiveTexture(GL_TEXTURE2);
		pDeferredGeoFB->getAttachment(GL_DEPTH_ATTACHMENT)->bind();

		m_pShaderProgram->use();
		m_pShaderProgram->setInt("gPosition", 0);
		m_pShaderProgram->setInt("gNormal", 1);
		m_pShaderProgram->setInt("gDepthTex", 2);

		m_pShaderProgram->setVec3("uViewPos", pCamera->getWorldPos());
		m_pShaderProgram->setVec3("uLightDir", pDirLight->_LightDir);
		m_pShaderProgram->setVec3("uLightColor", pDirLight->_LightColor);
		m_pShaderProgram->setFloat("uGlossy", 16.0f);
		m_pQuadVAO->draw();
	}


}