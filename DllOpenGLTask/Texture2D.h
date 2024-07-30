#pragma once
#include <stb_image.h>
#include "dllFile.h"
namespace openGLTask {
	class OPENGLTASK_API CTexture2D
	{
	public:
		CTexture2D();
		unsigned int loadTexture(char const* path);
	};
}

