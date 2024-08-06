#pragma once
#include <memory>
#include "dllFile.h"
namespace openGLTask
{
	class CGameObject;
	class CCamera;
	struct SDirectionalLight;
	struct SPointLight;
	class OPENGLTASK_API CScene
	{
	public:
		CScene() = default;
		~CScene();

		void setRootNode(const std::shared_ptr<CGameObject>& vRootNode) { m_pRootNode = vRootNode; }
		void setCamera(const std::shared_ptr<CCamera>& vCamera) { m_pCamera = vCamera; }
		void setDirectionalLight(const std::shared_ptr<SDirectionalLight>& vDirectionalLight) { m_pDirectionalLight = vDirectionalLight; }
		void setPointLight(const std::shared_ptr<SPointLight>& vPointLight) { m_PointLight = vPointLight; }

		const auto& getRootNode() const { return m_pRootNode; }
		const auto& getCamera() const { return m_pCamera; }
		const auto& getDirectionalLight() const { return m_pDirectionalLight; }
		const auto& getPointLight() const { return m_PointLight; }

	private:
		std::shared_ptr<CGameObject> m_pRootNode;
		std::shared_ptr<CCamera> m_pCamera;
		std::shared_ptr<SDirectionalLight> m_pDirectionalLight;
		std::shared_ptr<SPointLight> m_PointLight;
	};
}