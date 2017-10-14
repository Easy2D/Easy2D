#include "..\ebase.h"
#include "..\enodes.h"
#include "..\emsg.h"
#include <algorithm>

e2d::EScene::EScene()
	: m_bWillSave(true)
	, m_bSortNeeded(false)
{
}

e2d::EScene::~EScene()
{
	clearAllChildren();
}

void e2d::EScene::_onRender()
{
	this->_sortChildren();

	// �������нڵ�
	for (auto child : m_vChildren)
	{
		child->callOn();
	}
}

void e2d::EScene::_sortChildren()
{
	if (m_bSortNeeded)
	{
		m_bSortNeeded = false;

		// �ӽڵ�����
		std::sort(
			std::begin(m_vChildren),
			std::end(m_vChildren),
			[](ENode * n1, ENode * n2) {
				return n1->getOrder() < n2->getOrder();
			}
		);
	}
}

void e2d::EScene::onEnter()
{
	// ���ó����ϵ����ж�ʱ�����������Ͷ���
	//Timer::notifyAllSceneTimers(m_pNextScene);
	EMsgManager::notifyAllListenersBindWithScene(this);
	//ActionManager::notifyAllSceneActions(m_pNextScene);
}

void e2d::EScene::onExit()
{
	if (m_bWillSave)
	{
		//Timer::waitAllSceneTimers(m_pCurrentScene);
		EMsgManager::waitAllListenersBindWithScene(this);
		//ActionManager::waitAllSceneActions(m_pCurrentScene);
	}
	else
	{
		//Timer::clearAllSceneTimers(m_pCurrentScene);
		EMsgManager::clearAllListenersBindWithScene(this);
		//ActionManager::stopAllSceneActions(m_pCurrentScene);
	}
}

void e2d::EScene::add(ENode * child, int order /* = 0 */)
{
	ASSERT(child != nullptr, "Scene::add NULL pointer exception.");
	ASSERT(child->getParentScene() == nullptr, "Child already added. It can't be added again!");

	if (child)
	{
		child->setParentScene(this);

		child->setOrder(order);

		child->retain();

		m_vChildren.push_back(child);

		m_bSortNeeded = true;
	}
}

bool e2d::EScene::remove(ENode * child, bool autoRelease /* = true */)
{
	if (child == nullptr) return false;

	// Ѱ���Ƿ�����ͬ�ڵ�
	std::vector<ENode*>::iterator iter;
	for (iter = m_vChildren.begin(); iter != m_vChildren.end(); iter++)
	{
		// �ҵ���ͬ�ڵ�
		if (*iter == child)
		{
			if (autoRelease) 
				(*iter)->autoRelease();
			// ��������ü�����һ
			(*iter)->release();
			// ȥ���ýڵ�
			m_vChildren.erase(iter);
			return true;
		}
	}
	// δ�ҵ��ýڵ㷵�� false
	return false;
}

std::vector<e2d::ENode*>& e2d::EScene::getChildren()
{
	return m_vChildren;
}

size_t e2d::EScene::getChildrenCount() const
{
	return m_vChildren.size();
}

e2d::ENode * e2d::EScene::getChild(EString childName) const
{
	return ENode::getChild(childName, m_vChildren);
}

void e2d::EScene::clearAllChildren()
{
	// ���нڵ�����ü�����һ
	for (auto child : m_vChildren)
	{
		child->autoRelease();
		child->release();
	}
	// ��մ���ڵ������
	m_vChildren.clear();
}

void e2d::EScene::bindListener(EMouseListener * listener)
{
	EMsgManager::bindListenerWith(listener, this);
}

void e2d::EScene::bindListener(EKeyboardListener * listener)
{
	EMsgManager::bindListenerWith(listener, this);
}
