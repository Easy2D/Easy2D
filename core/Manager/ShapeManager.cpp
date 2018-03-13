#include "..\emanagers.h"
#include "..\enodes.h"
#include "..\eshape.h"

// ��״����
static std::vector<e2d::Shape*> s_vShapes;
// ��ײ����״̬
static bool s_bCollisionEnable = false;


void e2d::ShapeManager::setCollisionEnable(bool bEnable)
{
	s_bCollisionEnable = bEnable;
}

void e2d::ShapeManager::__updateShape(e2d::Shape * pActiveShape)
{
	// �ж���ײ�����Ƿ��
	if (!s_bCollisionEnable)
		return;

	Node* pActiveNode = pActiveShape->m_pParentNode;
	if (pActiveNode)
	{
		// ��ȡ�ڵ����ڳ���
		Scene* pCurrentScene = pActiveNode->getParentScene();
		
		// �ж���������״�Ľ������
		for (size_t i = 0; i < s_vShapes.size(); i++)
		{
			auto pPassiveShape = s_vShapes[i];
			// �ж�������״�Ƿ���ͬһ������
			if (pActiveShape == pPassiveShape)
				return;
			// �ж��������Ƿ����໥��ͻ������
			if (pActiveShape->m_nCollisionBitmask & pPassiveShape->m_nCategoryBitmask)
			{
				// ��ȡ����ײ�ڵ�
				Node* pPassiveNode = pPassiveShape->m_pParentNode;
				// �ж����ڵ��Ƿ���ͬһ������
				if (pPassiveNode &&
					pPassiveNode->getParentScene() == pCurrentScene)
				{
					// �ж�����״�������
					int relation = pActiveShape->getRelationWith(pPassiveShape);
					// ���� UNKNOWN �� DISJOINT ���
					if (relation != Relation::UNKNOWN && relation != Relation::DISJOINT)
					{
						pActiveNode->onCollide(pPassiveNode, relation);
						pPassiveNode->onCollide(pActiveNode, pPassiveShape->getRelationWith(pActiveShape));
						pCurrentScene->onCollide(pActiveNode, pPassiveNode);
					}
				}
			}
		}
	}
}

void e2d::ShapeManager::__add(Shape * pShape)
{
	if (pShape)
	{
		if (pShape->m_pParentNode)
		{
			WARN_IF(true, "ShapeManager::__add Failed! The shape is already added.");
			return;
		}
		pShape->retain();
		s_vShapes.push_back(pShape);
	}
}

void e2d::ShapeManager::__remove(Shape * pShape)
{
	if (pShape)
	{
		for (size_t i = 0; i < s_vShapes.size(); i++)
		{
			if (s_vShapes[i] == pShape)
			{
				SafeRelease(&pShape);
				s_vShapes.erase(s_vShapes.begin() + i);
				return;
			}
		}
	}
}
