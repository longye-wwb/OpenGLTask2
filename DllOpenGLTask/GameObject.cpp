#include "GameObject.h"
#include <queue>

namespace openGLTask
{

	CGameObject::CGameObject()
	:m_Transform{}, m_ChildGameObject{}, m_ParentGameObject{ nullptr }, m_ModelMatrix{}
	{
		__updateModelMatrix();
	}

	void CGameObject::addChild(const std::shared_ptr<CGameObject>& vGameObject)
	{
		m_ChildGameObject.push_back(vGameObject);
		vGameObject->m_ParentGameObject = this;
		vGameObject->__updateModelMatrix();
	}

	void CGameObject::setPosition(const glm::vec3& vPosition)
	{
		m_Transform.setLocalPosition(vPosition);
		__updateModelMatrix();
	}

	void CGameObject::setRotation(const glm::vec3& vRotation)
	{
		m_Transform.setLocalRotation(vRotation);
		__updateModelMatrix();
	}

	void CGameObject::setScale(const glm::vec3& vScale)
	{
		m_Transform.setLocalScale(vScale);
		__updateModelMatrix();
	}

	void CGameObject::addMesh(const std::shared_ptr<CMesh>& vMesh)
	{
		m_Meshes.push_back(vMesh);
	}

	void CGameObject::traverse(const std::shared_ptr<CGameObject>& vRootGameObject, std::function<void(const std::shared_ptr<CGameObject>&)> vFuncTraverse)
	{
		std::queue<std::shared_ptr<CGameObject>> QueueObjects;
		QueueObjects.push(vRootGameObject);
		while (!QueueObjects.empty())
		{
			const auto& pCurrNode = QueueObjects.front();
			vFuncTraverse(pCurrNode);
			QueueObjects.pop();
			for (const auto& pNode : pCurrNode->m_ChildGameObject)
				QueueObjects.push(pNode);
		}
	}

	void CGameObject::__updateModelMatrix()
	{
		if (m_ParentGameObject == nullptr) m_ModelMatrix = m_Transform.getLocalModelMatrix();
		else m_ModelMatrix = m_ParentGameObject->getModelMatrix() * m_Transform.getLocalModelMatrix();

		for (const auto& pChild : m_ChildGameObject)
		{
			pChild->__updateModelMatrix();
		}
	}
}