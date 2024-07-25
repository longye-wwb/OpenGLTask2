#pragma once
#include "RenderConfiguration.h"

namespace openGLTask {
	class CShaderConfiguration :public CRenderConfiguration
	{
	public:
		CShaderConfiguration();
		~CShaderConfiguration() = default;
		void __defineAttributesV() override;
		void __loadDefaultConfigV() override; 
	};
}


