#pragma once
#include <memory>
#include "VertexBuffer.h"
#include "dllFile.h"

namespace openGLTask
{
	class OPENGLTASK_API CMesh
	{
	public:
		CMesh(const std::shared_ptr<CVertexBuffer>& vVAO):m_pVAO(vVAO)
		{};

		void draw() const { m_pVAO->draw(); }
			

	private:
		std::shared_ptr<CVertexBuffer> m_pVAO;
	};
}

