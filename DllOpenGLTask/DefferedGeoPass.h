#pragma once
#include "RenderPass.h"
#include  "dllFile.h"

namespace openGLTask 
{
	class CShader;
	class OPENGLTASK_API CDefferedGeoPass :public CRenderPass
	{
	public:
		using CRenderPass::CRenderPass;
		// Inherited via CRenderPass
		void renderV(const std::shared_ptr<CScene>& vScene, const std::vector<std::shared_ptr<CFrameBuffer>>& vFrameBuffers, 
					const std::vector<size_t> vOutputIndices, size_t vIdxOfPasses) override;
	};
}
