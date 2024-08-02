#pragma once
#include "Pipline.h"
#include "FrameBuffer.h"
#include "VertexBuffer.h"

namespace openGLTask 
{
	class CDeferredPipeline:public CPipeline
	{
	public:
		CDeferredPipeline(int vWidth, int vHeight);

		void renderV(const std::shared_ptr<CRenderWindow>& vWindow) override;
		void onViewportChangeV(int vWidth, int vHeight) override { m_pFBO->updateFrameBuffer(vWidth, vHeight); }
	private:
		void __registerFrameBuffer(int vWidth, int vHeight) const;
		std::shared_ptr<CFrameBuffer> m_pFBO;
		std::shared_ptr<CVertexBuffer> m_pQuadVAO;
		std::shared_ptr<CShader> m_pGeoPassShader;
		int m_Width, m_Height;
	};
}
