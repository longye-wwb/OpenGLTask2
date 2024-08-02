#include "DeferredPipeline.h"
#include "Primitive.h"
#include "pch.h"
#include "GameObject.h"

namespace openGLTask
{
	CDeferredPipeline::CDeferredPipeline(int vWidth, int vHeight)
		:m_Width(vWidth), m_Height(vHeight),
		m_pFBO(std::make_shared<CFrameBuffer>()), m_pQuadVAO(CPrimitive::createQuad()),
		m_pGeoPassShader(std::make_shared<CShader>("../shaders/gbuffer.vs", "../shaders/gbuffer.fs"))
	{
		__registerFrameBuffer(vWidth, vHeight);
	}

	void CDeferredPipeline::renderV(const std::shared_ptr<CRenderWindow>& vWindow)
	{
		//GeoPass
		m_pFBO->bind();
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		m_pGeoPassShader->use();
		m_pGeoPassShader->setMat4("uView", vWindow->getCamera()->getViewMatrix());
		m_pGeoPassShader->setMat4("uProjection", vWindow->getCamera()->getProjectionMatrix());;
		CGameObject::traverse(vWindow->getRootGameObject(), [&](const std::shared_ptr<CGameObject>& vGameObject) {
			m_pGeoPassShader->setMat4("uModel", vGameObject->getModelMatrix());
			for (const auto& pMesh : vGameObject->getMeshes())
			{
				pMesh->draw();
			}
			});

		//LightPass
		vWindow->getFrameBuffer()->bind();
		glm::vec4 SolidColor = vWindow->getCamera()->getSolidColor();
		glClearColor(SolidColor.x, SolidColor.y, SolidColor.z, SolidColor.w);
		glDisable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT);
		glActiveTexture(GL_TEXTURE0);
		m_pFBO->getAttachment(GL_COLOR_ATTACHMENT0)->bind();
		glActiveTexture(GL_TEXTURE1);
		m_pFBO->getAttachment(GL_COLOR_ATTACHMENT1)->bind();
		glActiveTexture(GL_TEXTURE2);
		m_pFBO->getAttachment(GL_DEPTH_ATTACHMENT)->bind();
		CGameObject::traverse(vWindow->getRootGameObject(), [&](const std::shared_ptr<CGameObject>& vGameObject) {
			for (const auto& pModel : vGameObject->getMeshes())
			{
				//pModel->getMaterial()->use(vScene, vNode);
				m_pQuadVAO->draw();
			}
			});
	}

	void CDeferredPipeline::__registerFrameBuffer(int vWidth, int vHeight) const
	{
		m_pFBO->bind();
		// - Position color buffer
		const auto& pPositionTex = std::make_shared<CTexture2D>(vWidth, vHeight, GL_RGB16F, GL_RGB, GL_FLOAT);
		pPositionTex->setTextureParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		pPositionTex->setTextureParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		m_pFBO->setAttachment(GL_COLOR_ATTACHMENT0, pPositionTex);
		// - Normal color buffer
		const auto& pNormalTex = std::make_shared<CTexture2D>(vWidth, vHeight, GL_RGB16F, GL_RGB, GL_FLOAT);
		pNormalTex->setTextureParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		pNormalTex->setTextureParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		m_pFBO->setAttachment(GL_COLOR_ATTACHMENT1, pNormalTex);
		// - Tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
		m_pFBO->drawAttachments(std::vector<GLenum>{GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1});
		// - Create and attach depth buffer (renderbuffer)
		//m_pFBO->setAttachment(GL_DEPTH_ATTACHMENT, std::make_shared<CRenderBuffer>(vWidth, vHeight, GL_DEPTH_COMPONENT));
		const auto& pDepthTex = std::make_shared<CTexture2D>(vWidth, vHeight, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE);
		pDepthTex->setTextureParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		pDepthTex->setTextureParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		m_pFBO->setAttachment(GL_DEPTH_ATTACHMENT, pDepthTex);
		// - Finally check if framebuffer is complete
		CFrameBuffer::checkComplete();
		m_pFBO->unbind();
	}
}