#pragma once
#include <vector>
#include "Transform.h"
#include <memory>
#include <functional>
#include "dllFile.h"

namespace openGLTask
{
	class CMesh;
	class OPENGLTASK_API CGameObject
	{
	public: 
		CGameObject();

		glm::mat4 getModelMatrix() const { return m_ModelMatrix; }
		const std::vector<std::shared_ptr<CGameObject>>& getChildNodes() const { return m_ChildGameObject; }
		void clearChilds() { m_ChildGameObject.clear(); }
		void addChild(const std::shared_ptr<CGameObject>& vGameObject);
		void setPosition(const glm::vec3& vPosition);
		void setRotation(const glm::vec3& vRotation);
		void setScale(const glm::vec3& vScale);
		const CTransform& getTransform() const { return m_Transform; }
		void addMesh(const std::shared_ptr<CMesh>& vMesh);
		const auto& getMeshes() const { return m_Meshes; }
		static void traverse(const std::shared_ptr<CGameObject>& vRootGameObject, std::function<void(const std::shared_ptr<CGameObject>&)> vFuncTraverse);
	
	private:
		void __updateModelMatrix();
		CTransform m_Transform;
		glm::mat4 m_ModelMatrix;
		CGameObject* m_ParentGameObject;
		std::vector<std::shared_ptr<CMesh>> m_Meshes;
		std::vector<std::shared_ptr<CGameObject>> m_ChildGameObject;
	};
}