#pragma once
#include "dllFile.h"
#include "HiveConfig.h"
namespace openGLTask {
	class OPENGLTASK_API CRenderConfiguration : public hiveConfig::CHiveConfig
	{
	public:
		CRenderConfiguration();
		~CRenderConfiguration() = default;
		void __defineAttributesV();
		void __loadDefaultConfigV();
	};
}
