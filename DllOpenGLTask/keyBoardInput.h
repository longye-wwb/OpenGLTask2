#pragma once
#include "InputController.h"
#include "dllFile.h"

namespace openGLTask 
{
	class OPENGLTASK_API CkeyBoardInput : public CInputController
	{
	public:
		CkeyBoardInput();

		void onKeyDown(int key) override;
		bool getQState() { return m_QPressState; }
		void onKeyUp(int key) override;

	private:
		bool m_QPressState = false;
	};

}
