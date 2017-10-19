#include "..\etools.h"
#include "..\eactions.h"

static e2d::EVector<e2d::EAction*> s_vActions;


void e2d::EActionManager::bindAction(EAction * action, ENode * pTargetNode)
{
	ASSERT(
		(!action->m_pTarget),
		"The action is already running, it cannot running again!"
	);
	WARN_IF(action == nullptr, "EAction NULL pointer exception!");
	WARN_IF(pTargetNode == nullptr, "EAction's target is NULL!");

	if (action && pTargetNode)
	{
		action->start();
		action->retain();
		action->m_pTarget = pTargetNode;
		s_vActions.push_back(action);
	}
}

void e2d::EActionManager::startAllActionsBindedWith(EScene * pParentScene)
{
	if (pParentScene)
	{
		for (const auto &child : pParentScene->getChildren())
		{
			EActionManager::startAllActionsBindedWith(child);
		}
	}
}

void e2d::EActionManager::stopAllActionsBindedWith(EScene * pParentScene)
{
	if (pParentScene)
	{
		for (const auto &child : pParentScene->getChildren())
		{
			EActionManager::stopAllActionsBindedWith(child);
		}
	}
}

void e2d::EActionManager::clearAllActionsBindedWith(EScene * pParentScene)
{
	if (pParentScene)
	{
		for (const auto & child : pParentScene->getChildren())
		{
			ETimerManager::clearAllTimersBindedWith(child);
		}
	}
}

void e2d::EActionManager::startAllActionsBindedWith(ENode * pTargetNode)
{
	if (pTargetNode)
	{
		for (const auto &action : s_vActions)
		{
			if (action->getTarget() == pTargetNode)
			{
				action->start();
			}
		}
		for (const auto &child : pTargetNode->getChildren())
		{
			EActionManager::startAllActionsBindedWith(child);
		}
	}
}

void e2d::EActionManager::stopAllActionsBindedWith(ENode * pTargetNode)
{
	if (pTargetNode)
	{
		for (const auto &action : s_vActions)
		{
			if (action->getTarget() == pTargetNode)
			{
				action->stop();
			}
		}
		for (const auto &child : pTargetNode->getChildren())
		{
			EActionManager::stopAllActionsBindedWith(child);
		}
	}
}

void e2d::EActionManager::clearAllActionsBindedWith(ENode * pTargetNode)
{
	if (pTargetNode)
	{
		for (size_t i = 0; i < s_vActions.size();)
		{
			auto a = s_vActions[i];
			if (a->getTarget() == pTargetNode)
			{
				SafeRelease(&a);
				s_vActions.erase(s_vActions.begin() + i);
			}
			else
			{
				i++;
			}
		}
		for (auto child : pTargetNode->getChildren())
		{
			ETimerManager::clearAllTimersBindedWith(child);
		}
	}
}

void e2d::EActionManager::startAllActions()
{
	EActionManager::startAllActionsBindedWith(EApp::getCurrentScene());
}

void e2d::EActionManager::stopAllActions()
{
	EActionManager::stopAllActionsBindedWith(EApp::getCurrentScene());
}

void e2d::EActionManager::clearAllActions()
{
	EActionManager::clearAllActionsBindedWith(EApp::getCurrentScene());
}

void e2d::EActionManager::_waitAllActionsBindedWith(EScene * pParentScene)
{
	if (pParentScene)
	{
		for (const auto &child : pParentScene->getChildren())
		{
			EActionManager::_waitAllActionsBindedWith(child);
		}
	}
}

void e2d::EActionManager::_notifyAllActionsBindedWith(EScene * pParentScene)
{
	if (pParentScene)
	{
		for (const auto &child : pParentScene->getChildren())
		{
			EActionManager::_notifyAllActionsBindedWith(child);
		}
	}
}

void e2d::EActionManager::_waitAllActionsBindedWith(ENode * pTargetNode)
{
	if (pTargetNode)
	{
		for (const auto &action : s_vActions)
		{
			if (action->getTarget() == pTargetNode)
			{
				action->_wait();
			}
		}
		for (const auto &child : pTargetNode->getChildren())
		{
			EActionManager::_waitAllActionsBindedWith(child);
		}
	}
}

void e2d::EActionManager::_notifyAllActionsBindedWith(ENode * pTargetNode)
{
	if (pTargetNode)
	{
		for (const auto &action : s_vActions)
		{
			if (action->getTarget() == pTargetNode)
			{
				action->_notify();
			}
		}
		for (const auto &child : pTargetNode->getChildren())
		{
			EActionManager::_notifyAllActionsBindedWith(child);
		}
	}
}

void e2d::EActionManager::ActionProc()
{
	if (s_vActions.empty())
		return;
	
	EAction * action;
	// ѭ�����������������еĶ���
	for (size_t i = 0; i < s_vActions.size(); i++)
	{
		action = s_vActions[i];
		// ��ȡ��������״̬
		if (action->isRunning())
		{
			if (action->_isEnding())
			{
				// �����Ѿ�����
				SafeRelease(&action);
				s_vActions.erase(s_vActions.begin() + i);
			}
			else
			{
				// ��ʼ������
				if (!action->m_bInit)
				{
					action->_init();
				}
				// ִ�ж���
				action->_callOn();
			}
		}
	}
}
