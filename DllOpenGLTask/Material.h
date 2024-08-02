#pragma once
#include "RenderWindow.h"
#include <memory>
#include "GameObject.h"
#include "Shader.h"

namespace openGLTask 
{
	class CMaterial
	{
	public:
		void use() const;
	private:
		std::shared_ptr<CShader> m_pShader;
	};
}

