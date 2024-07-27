#include "keyBoardInput.h"
#include <GLFW/glfw3.h>

namespace openGLTask 
{
	CkeyBoardInput::CkeyBoardInput() : m_QPressState(false)
	{
	}

	void CkeyBoardInput::onKeyDown(int key)
	{
		if (key == GLFW_KEY_Q)
		{
			m_QPressState = !m_QPressState;
		}
	}

	void CkeyBoardInput::onKeyUp(int key)
	{
	}
}

