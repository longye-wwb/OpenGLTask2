#pragma once
#include "dllFile.h"

namespace openGLTask
{
	class CGameObject;
	class OPENGLTASK_API CModelLoader
	{
	public:
		static std::shared_ptr<CGameObject> loadGltfFile(const std::string& vFileName);
	};
}
