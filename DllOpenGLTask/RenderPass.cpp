#include "pch.h"
#include "RenderPass.h"
#include "Framebuffer.h"

namespace openGLTask 
{
	void CRenderPass::renderV(const std::shared_ptr<CScene>& vScene, const std::vector<std::shared_ptr<CFrameBuffer>>& vFrameBuffers, const std::vector<size_t> vOutputIndices, size_t vIdxOfPasses)
	{
		const auto& pFrameBuffer = vFrameBuffers[vOutputIndices[vIdxOfPasses]];
		pFrameBuffer->bind();
		glViewport(0, 0, pFrameBuffer->getWidth(), pFrameBuffer->getHeight());
	}

}