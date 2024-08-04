#pragma once
#include "RenderPass.h"
#include <memory>
#include "dllFile.h"
namespace openGLTask
{
	class CVertexBuffer;
	class OPENGLTASK_API CDeferredLitPass : public CRenderPass
	{
	public:
		CDeferredLitPass(
			const std::shared_ptr<CShader>& vShaderProgram,
			size_t vIdxOfDeferredGeoFB
		);
		~CDeferredLitPass();

		// Inherited via CRenderPass
		void renderV(const std::shared_ptr<CScene>& vScene, const std::vector<std::shared_ptr<CFrameBuffer>>& vFrameBuffers, const std::vector<size_t> vOutputIndices, size_t vIdxOfPasses) override;

	private:
		std::shared_ptr<CVertexBuffer> m_pQuadVAO;
		size_t m_IdxOfDeferredGeoFB;
	};
}
