#pragma once
#include "RenderWindow.h"

namespace openGLTask {
	class CPipeline
	{
	public:
		virtual void renderV(const std::shared_ptr<CRenderWindow>& vScene) = 0;
		virtual void onViewportChangeV(int vWidth, int vHeight) {};
	};
}