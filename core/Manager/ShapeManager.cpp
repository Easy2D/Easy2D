#include "..\emanagers.h"
#include "..\enodes.h"
#include "..\eshape.h"

// ��״����
std::vector<e2d::Shape*> s_vShapes;


void e2d::ShapeManager::__updateShape(e2d::Shape * pActiveShape)
{
	Node* pActiveNode = pActiveShape->m_pParentNode;
	if (pActiveNode)
	{
		// ��ȡ�ڵ����ڳ���
		Scene* pCurrentScene = pActiveNode->getParentScene();
		// �ж���������״�Ľ������
		for (auto pPassiveShape : s_vShapes)
		{
			// �ж��������Ƿ����໥��ͻ������
			if (pActiveShape->m_nCollisionBitmask & pPassiveShape->m_nCategoryBitmask)
			{
				// ��ȡ����ײ�ڵ�
				Node* pPassiveNode = pPassiveShape->m_pParentNode;
				// �ж����ڵ��Ƿ���ͬһ������
				if (pPassiveNode &&
					pPassiveNode != pActiveNode &&
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

void e2d::ShapeManager::__addShape(Shape * pShape)
{
	if (pShape)
	{
		pShape->retain();
		s_vShapes.push_back(pShape);
	}
}

void e2d::ShapeManager::__delShape(Shape * pShape)
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
