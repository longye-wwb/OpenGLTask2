#pragma once
#include "RenderWindow.h"
namespace openGLTask {
	class CPipeline
	{
	public:
		virtual void render(const std::shared_ptr<CRenderWindow>& vScene) = 0;
		virtual void onViewportChange(int vWidth, int vHeight) {};
	};
}