#include "pch.h"
#include "RenderPipeline.h"
#include "RenderPass.h"
#include "FrameBuffer.h"

namespace openGLTask
{
	void CRenderPipeline::render(const std::shared_ptr<CScene>& vScene) const
	{
		for (size_t i = 0; i < m_RenderPasses.size(); ++i)
		{
			if (m_EnablePasses[i])
			{
				m_RenderPasses[i]->renderV(vScene, m_FrameBuffers, m_OutputIndices, i);
			}
		}
	}

	void CRenderPipeline::addRenderPass(const std::shared_ptr<CRenderPass>& vRenderPass, size_t vOutputIndex, bool vEnableAutoResize)
	{
		m_RenderPasses.push_back(vRenderPass);
		m_OutputIndices.push_back(vOutputIndex);
		m_EnablePasses.push_back(true);
		m_EnableAutoResize.push_back(vEnableAutoResize);
	}

	void CRenderPipeline::resize(int vWidth, int vHeight) const
	{
		for (size_t i = 0; i < m_FrameBuffers.size(); ++i)
		{
			if (m_EnableAutoResize[i])
			{
				m_FrameBuffers[i]->updateFrameBuffer(vWidth, vHeight);
				HIVE_LOG_DEBUG("framebuffer {} resize {} {}", i, vWidth, vHeight);
			}
		}
	}
}