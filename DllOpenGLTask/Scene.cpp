#include "Scene.h"
namespace openGLTask
{
	CScene::~CScene()
	{
		m_pDirectionalLight.reset();
		m_pCamera.reset();
		m_pRootNode.reset();
	}
}