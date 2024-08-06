#pragma once
#include <memory>
#include <vector>
#include  "dllFile.h"

namespace openGLTask
{
	class CScene;
	class CFrameBuffer;
	class CShader;
	class OPENGLTASK_API CRenderPass
	{
	public:
		CRenderPass(const std::shared_ptr<CShader>& vShaderProgram) :m_pShaderProgram(vShaderProgram) {}
		virtual ~CRenderPass() { m_pShaderProgram.reset(); }

		virtual void renderV(
			const std::shared_ptr<CScene>& vScene,
			const std::vector<std::shared_ptr<CFrameBuffer>>& vFrameBuffers,
			const std::vector<size_t> vOutputIndices,
			size_t vIdxOfPasses
		);

	protected:
		std::shared_ptr<CShader> m_pShaderProgram;
	};
};

