#pragma once
#include "Material.h"
#include ""
namespace openGLTask 
{
	class simpleMaterial :public CMaterial
	{
	private:
		std::shared_ptr<CShader> m_pShader;
	};

}
