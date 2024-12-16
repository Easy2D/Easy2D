#include <easy2d/e2dmanager.h>
#include <easy2d/e2daction.h>
#include <easy2d/e2dnode.h>

static std::vector<easy2d::Action*> s_vActions;


void easy2d::ActionManager::__update()
{
	if (s_vActions.empty() || Game::isPaused())
		return;

	// 循环遍历所有正在运行的动作
	for (size_t i = 0; i < s_vActions.size(); ++i)
	{
		auto action = s_vActions[i];
		// 获取动作运行状态
		if (action->_isDone())
		{
			action->_target = nullptr;
			action->release();
			s_vActions.erase(s_vActions.begin() + i);
		}
		else
		{
			if (action->isRunning())
			{
				// 执行动作
				action->_update();
			}
		}
	}
}

void easy2d::ActionManager::__resumeAllBoundWith(Node * target)
{
	if (s_vActions.empty() || target == nullptr)
		return;

	for (auto action : s_vActions)
	{
		if (action->getTarget() == target)
		{
			action->resume();
		}
	}
}

void easy2d::ActionManager::__pauseAllBoundWith(Node * target)
{
	if (s_vActions.empty() || target == nullptr)
		return;

	for (auto action : s_vActions)
	{
		if (action->getTarget() == target)
		{
			action->pause();
		}
	}
}

void easy2d::ActionManager::__stopAllBoundWith(Node * target)
{
	if (s_vActions.empty() || target == nullptr)
		return;

	for (auto action : s_vActions)
	{
		if (action->getTarget() == target)
		{
			action->stop();
		}
	}
}

void easy2d::ActionManager::start(Action * action, Node * target, bool paused)
{
	if (action == nullptr) E2D_WARNING("Action NULL pointer exception!");
	if (target == nullptr) E2D_WARNING("Target node NULL pointer exception!");

	if (action && target)
	{
		if (action->_target == nullptr)
		{
			auto iter = std::find(s_vActions.begin(), s_vActions.end(), action);
			if (iter == s_vActions.end())
			{
				action->_startWithTarget(target);
				action->retain();
				action->_running = !paused;
				s_vActions.push_back(action);
			}
		}
		else
		{
			E2D_WARNING("该 Action 已有执行目标");
		}
	}
}

void easy2d::ActionManager::resume(const String& name)
{
	if (s_vActions.empty() || name.empty())
		return;

	for (auto action : s_vActions)
	{
		if (action->getName() == name)
		{
			action->resume();
		}
	}
}

void easy2d::ActionManager::pause(const String& name)
{
	if (s_vActions.empty() || name.empty())
		return;

	for (auto action : s_vActions)
	{
		if (action->getName() == name)
		{
			action->pause();
		}
	}
}

void easy2d::ActionManager::stop(const String& name)
{
	if (s_vActions.empty() || name.empty())
		return;

	for (auto action : s_vActions)
	{
		if (action->getName() == name)
		{
			action->stop();
		}
	}
}

void easy2d::ActionManager::__removeAllBoundWith(Node * target)
{
	if (target)
	{
		for (size_t i = 0; i < s_vActions.size();)
		{
			auto a = s_vActions[i];
			if (a->getTarget() == target)
			{
				GC::release(a);
				s_vActions.erase(s_vActions.begin() + i);
			}
			else
			{
				++i;
			}
		}
	}
}

void easy2d::ActionManager::__uninit()
{
	for (auto action : s_vActions)
	{
		GC::release(action);
	}
	s_vActions.clear();
}

std::vector<easy2d::Action*> easy2d::ActionManager::get(const String& name)
{
	std::vector<Action*> actions;
	for (auto action : s_vActions)
	{
		if (action->isName(name))
		{
			actions.push_back(action);
		}
	}
	return actions;
}

const std::vector<easy2d::Action*>& easy2d::ActionManager::getAll()
{
	return s_vActions;
}

void easy2d::ActionManager::__resetAll()
{
	for (auto action : s_vActions)
	{
		action->_resetTime();
	}
}
