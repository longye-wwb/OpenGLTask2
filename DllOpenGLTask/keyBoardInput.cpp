#include "keyBoardInput.h"

namespace openGLTask 
{
	CkeyBoardInput::CkeyBoardInput():m_QPressState(false),m_EPressState(false)
	{
	}
	void CkeyBoardInput::onKeyDown(int key)
	{
		if (key==GLFW_KEY_Q)
		{
			m_QPressState = !m_QPressState;
		}
		if (key == GLFW_KEY_E)
		{
			m_EPressState = !m_EPressState;
		}
	}
	void CkeyBoardInput::onKeyUp(int key)
	{
	}
}

