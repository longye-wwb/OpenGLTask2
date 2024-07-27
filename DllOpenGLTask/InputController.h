#pragma once
#include <GLFW/glfw3.h>
#include "dllFile.h"

namespace openGLTask 
{
	class OPENGLTASK_API CInputController
	{
	public:
		virtual void onKeyDown(int key) {};
		virtual void onKeyUp(int key) {};

	};
}