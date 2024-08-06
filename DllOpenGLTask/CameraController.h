#pragma once
#include <memory>
#include "InputController.h"

namespace openGLTask
{
	class CCamera;
	class CCameraController : public CInputController
	{
	public:
		virtual void init() {}
		virtual void control(const std::shared_ptr<CCamera>& vCamera) { m_pCamera = vCamera; }
		virtual void update(float vDeltaTime) {}

	protected:
		std::shared_ptr<CCamera> m_pCamera;
	};
}
