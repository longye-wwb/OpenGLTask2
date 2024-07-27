#pragma once
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