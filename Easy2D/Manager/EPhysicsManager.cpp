#include "..\emanagers.h"
#include "..\enodes.h"
#include "..\elisteners.h"
#include "..\egeometry.h"

// ����������
e2d::EVector<e2d::EPhysicsListener*> s_vListeners;
// ��״����
e2d::EVector<e2d::EGeometry*> s_vGeometries;


void e2d::EPhysicsManager::PhysicsProc()
{
	if (s_vListeners.empty() || s_vGeometries.empty() || EApp::isPaused())
		return;

	// �ж���������״��Ľ���
	for (auto &g1 : s_vGeometries)
	{
		// ֻ�Խ����˱仯�˶�������ж�
		if (g1->m_bTransformNeeded)
		{
			// �仯����
			g1->_transform();
			// g1 Ϊ������
			EPhysicsMsg::s_pActiveGeometry = g1;
			// �жϱ仯���״̬
			for (auto &g2 : s_vGeometries)
			{
				if (g1 != g2)
				{
					// g2 Ϊ������
					EPhysicsMsg::s_pPassiveGeometry = g2;
					// ��ȡ�����Ĺ�ϵ
					EPhysicsMsg::s_nRelation = g1->_intersectWith(g2);
					// �����ϵ��Ϊδ֪���޽�������Ӧ������
					if (EPhysicsMsg::s_nRelation != EPhysicsMsg::UNKNOWN &&
						EPhysicsMsg::s_nRelation != EPhysicsMsg::DISJOINT)
					{
						PhysicsListenerProc();
					}
				}
			}
		}
	}
}

void e2d::EPhysicsManager::PhysicsListenerProc()
{
	if (s_vListeners.empty()) return;

	// ִ�������Ϣ��������
	for (size_t i = 0; i < s_vListeners.size(); i++)
	{
		auto &listener = s_vListeners[i];

		if (listener->isRunning())
		{
			if (listener->getParentNode() &&
				listener->getParentNode()->getParentScene() == EApp::getCurrentScene())
			{
				listener->_callOn();
			}
		}
	}
}

void e2d::EPhysicsManager::bindListener(EPhysicsListener * listener, EScene * pParentScene)
{
	EPhysicsManager::bindListener(listener, pParentScene->getRoot());
}

void e2d::EPhysicsManager::bindListener(EPhysicsListener * listener, ENode * pParentNode)
{
	WARN_IF(listener == nullptr, "EPhysicsListener NULL pointer exception!");
	WARN_IF(pParentNode == nullptr, "EPhysicsListener add to a NULL ENode pointer!");

	if (listener && pParentNode)
	{
		ASSERT(
			!listener->m_pParentNode,
			"The listener is already binded, it cannot bind again!"
		);

		listener->retain();
		listener->start();
		listener->m_pParentNode = pParentNode;
		s_vListeners.push_back(listener);
	}
}

void e2d::EPhysicsManager::addGeometry(EGeometry * geometry)
{
	if (geometry)
	{
		geometry->retain();
		s_vGeometries.push_back(geometry);
	}
}

void e2d::EPhysicsManager::delGeometry(EGeometry * geometry)
{
	if (geometry)
	{
		for (size_t i = 0; i < s_vGeometries.size(); i++)
		{
			if (s_vGeometries[i] == geometry)
			{
				SafeReleaseAndClear(&geometry);
				s_vGeometries.erase(s_vGeometries.begin() + i);
				return;
			}
		}
	}
}

void e2d::EPhysicsManager::startListeners(const EString & name)
{
	for (auto listener : s_vListeners)
	{
		if (listener->getName() == name)
		{
			listener->start();
		}
	}
}

void e2d::EPhysicsManager::stopListeners(const EString & name)
{
	for (auto listener : s_vListeners)
	{
		if (listener->getName() == name)
		{
			listener->stop();
		}
	}
}

void e2d::EPhysicsManager::delListeners(const EString & name)
{
	EVector<EPhysicsListener*>::iterator iter;
	for (iter = s_vListeners.begin(); iter != s_vListeners.end();)
	{
		if ((*iter)->getName() == name)
		{
			SafeReleaseAndClear(&(*iter));
			iter = s_vListeners.erase(iter);
		}
		else
		{
			iter++;
		}
	}
}

void e2d::EPhysicsManager::startAllListenersBindedWith(EScene * pParentScene)
{
	EPhysicsManager::startAllListenersBindedWith(pParentScene->getRoot());
}

void e2d::EPhysicsManager::stopAllListenersBindedWith(EScene * pParentScene)
{
	EPhysicsManager::stopAllListenersBindedWith(pParentScene->getRoot());
}

void e2d::EPhysicsManager::startAllListenersBindedWith(ENode * pParentNode)
{
	for (auto listener : s_vListeners)
	{
		if (listener->getParentNode() == pParentNode)
		{
			listener->start();
		}
	}
	for (auto child : pParentNode->getChildren())
	{
		EPhysicsManager::startAllListenersBindedWith(child);
	}
}

void e2d::EPhysicsManager::stopAllListenersBindedWith(ENode * pParentNode)
{
	for (auto listener : s_vListeners)
	{
		if (listener->getParentNode() == pParentNode)
		{
			listener->stop();
		}
	}
	for (auto child : pParentNode->getChildren())
	{
		EPhysicsManager::stopAllListenersBindedWith(child);
	}
}

void e2d::EPhysicsManager::startAllListeners()
{
	EPhysicsManager::startAllListenersBindedWith(EApp::getCurrentScene());
}

void e2d::EPhysicsManager::stopAllListeners()
{
	EPhysicsManager::stopAllListenersBindedWith(EApp::getCurrentScene());
}

void e2d::EPhysicsManager::_clearManager()
{
	s_vListeners.clear();
}

void e2d::EPhysicsManager::_clearAllListenersBindedWith(ENode * pParentNode)
{
	for (size_t i = 0; i < s_vListeners.size();)
	{
		auto listener = s_vListeners[i];
		if (listener->getParentNode() == pParentNode)
		{
			SafeReleaseAndClear(&listener);
			s_vListeners.erase(s_vListeners.begin() + i);
		}
		else
		{
			i++;
		}
	}
}
