#include <e2dmanager.h>
#include <e2dnode.h>
#include <e2dcollider.h>
#include <e2dtool.h>


// ��ײ�弯��
static std::vector<easy2d::Collider*> s_vColliders;

void easy2d::ColliderManager::__updateCollider(easy2d::Collider * pActiveCollider)
{
	// �ж���ײ�����Ƿ��
	if (!Collision::isEnable())
		return;

	Node* pActiveNode = pActiveCollider->_parentNode;
	if (pActiveNode)
	{
		// ��ȡ�ڵ����ڳ���
		Scene* pCurrentScene = pActiveNode->getParentScene();

		// �ж���������ײ��Ľ������
		for (size_t i = 0; i < s_vColliders.size(); ++i)
		{
			auto pPassiveCollider = s_vColliders[i];
			// �ж�������ײ���Ƿ���ͬһ������
			if (pActiveCollider == pPassiveCollider)
				continue;

			// ��ȡ����ײ�ڵ�
			Node* pPassiveNode = pPassiveCollider->_parentNode;
			// �ж����ڵ��Ƿ���ͬһ������
			if (pPassiveNode &&
				pPassiveNode->getParentScene() == pCurrentScene)
			{
				// �ж��������Ƿ����໥��ͻ������
				if (Collision::isCollidable(pActiveNode, pPassiveNode))
				{
					// �ж�����ײ�彻�����
					Collider::Relation relation = pActiveCollider->getRelationWith(pPassiveCollider);
					// ���� UNKNOWN �� DISJOIN ���
					if (relation != Collider::Relation::Unknown && relation != Collider::Relation::Disjoin)
					{
						// ������ײ������
						Collision::__update(pActiveNode, pPassiveNode);
					}
				}
			}
		}
	}
}

void easy2d::ColliderManager::__addCollider(Collider * pCollider)
{
	if (pCollider)
	{
		if (pCollider->_parentNode)
		{
			E2D_WARNING(L"ColliderManager::__add Failed! The shape is already added.");
			return;
		}
		pCollider->retain();
		s_vColliders.push_back(pCollider);
	}
}

void easy2d::ColliderManager::__removeCollider(Collider * pCollider)
{
	if (pCollider)
	{
		for (size_t i = 0; i < s_vColliders.size(); ++i)
		{
			if (s_vColliders[i] == pCollider)
			{
				GC::release(pCollider);
				s_vColliders.erase(s_vColliders.begin() + i);
				return;
			}
		}
	}
}
