#pragma once
#include <memory>
#include "VertexBuffer.h"
#include "Material.h"

namespace openGLTask
{
	class CMesh
	{
	public:
		CMesh(const std::shared_ptr<CVertexBuffer>& vVAO, const std::shared_ptr<CMaterial>& vMaterial = nullptr)
			:m_pVAO(vVAO), m_pMaterial(vMaterial) {}

		void draw() const { m_pVAO->draw(); }
			
		void setMaterial(const std::shared_ptr<CMaterial>& vMaterial) { m_pMaterial = vMaterial; }
		const auto& getMaterial() const { return m_pMaterial; }

	private:
		std::shared_ptr<CVertexBuffer> m_pVAO;
		std::shared_ptr<CMaterial> m_pMaterial;
	};
}

