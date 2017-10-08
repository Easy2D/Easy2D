#include "..\easy2d.h"
#include <assert.h>

static std::vector<Action*> s_vActions;

void ActionManager::__exec()
{
	// ��ȡ��ǰʱ��
	static LARGE_INTEGER nNow;
	QueryPerformanceCounter(&nNow);
	// ��ʱָ��
	Action * action;
	// ѭ�����������������еĶ���
	for (size_t i = 0; i < s_vActions.size(); i++)
	{
		action = s_vActions[i];
		// ��ȡ��������״̬
		if (action->isRunning())
		{
			if (action->isEnding())
			{
				// �����Ѿ�����
				action->release();
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
				action->_exec(nNow);
			}
		}
	}
}

void ActionManager::addAction(Action * action)
{
	if (action)
	{
#ifdef _DEBUG
		for (auto a : s_vActions)
		{
			assert(a != action);
		}
#endif
		action->m_pParentScene = App::getLoadingScene();
		s_vActions.push_back(action);
	}
}

void ActionManager::notifyAllSceneActions(Scene * scene)
{
	for (auto action : s_vActions)
	{
		if (action->m_pParentScene == scene)
		{
			action->notify();
		}
	}
}

void ActionManager::waitAllSceneActions(Scene * scene)
{
	for (auto action : s_vActions)
	{
		if (action->m_pParentScene == scene)
		{
			action->wait();
		}
	}
}

void ActionManager::stopAllSceneActions(Scene * scene)
{
	for (auto action : s_vActions)
	{
		if (action->m_pParentScene == scene)
		{
			action->stop();
		}
	}
}

void ActionManager::startAction(Action * action)
{
	resumeAction(action);
}

void ActionManager::resumeAction(Action * action)
{
	for (auto act : s_vActions)
	{
		if (act == action)
		{
			act->resume();
		}
	}
}

void ActionManager::pauseAction(Action * action)
{
	for (auto act : s_vActions)
	{
		if (act == action)
		{
			act->pause();
		}
	}
}

void ActionManager::stopAction(Action * action)
{
	for (auto act : s_vActions)
	{
		if (act == action)
		{
			act->stop();
		}
	}
}

void ActionManager::startSpriteAllActions(Sprite * sprite)
{
	resumeSpriteAllActions(sprite);
}

void ActionManager::resumeSpriteAllActions(Sprite * sprite)
{
	for (auto action : s_vActions)
	{
		if (action->m_pTargetSprite == sprite)
		{
			action->resume();
		}
	}
}

void ActionManager::pauseSpriteAllActions(Sprite * sprite)
{
	for (auto action : s_vActions)
	{
		if (action->m_pTargetSprite == sprite)
		{
			action->pause();
		}
	}
}

void ActionManager::stopSpriteAllActions(Sprite * sprite)
{
	for (auto action : s_vActions)
	{
		if (action->m_pTargetSprite == sprite)
		{
			action->stop();
		}
	}
}

void ActionManager::startAllActions()
{
	resumeAllActions();
}

void ActionManager::resumeAllActions()
{
	for (auto action : s_vActions)
	{
		action->resume();
	}
}

void ActionManager::pauseAllActions()
{
	for (auto action : s_vActions)
	{
		action->pause();
	}
}

void ActionManager::stopAllActions()
{
	for (auto action : s_vActions)
	{
		action->stop();
	}
}

void ActionManager::clearAllActions()
{
	for (auto action : s_vActions)
	{
		action->autoRelease();
		action->release();
	}
	s_vActions.clear();
}
