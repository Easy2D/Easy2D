#include <e2daction.h>

easy2d::Spawn::Spawn()
{
}

easy2d::Spawn::Spawn(const std::vector<Action*>& actions)
{
	this->add(actions);
}

easy2d::Spawn::~Spawn()
{
}

void easy2d::Spawn::_init()
{
	Action::_init();

	if (_target)
	{
		for (auto action : _actions)
		{
			action->_target = _target;
			action->_init();
		}
	}
}

void easy2d::Spawn::onDestroy()
{
	Action::onDestroy();
	for (auto action : _actions)
	{
		GC::release(action);
	}
}

void easy2d::Spawn::_update()
{
	Action::_update();

	size_t doneNum = 0;
	for (auto action : _actions)
	{
		if (action->_isDone())
		{
			++doneNum;
		}
		else
		{
			action->_update();
		}
	}

	if (doneNum == _actions.size())
	{
		this->stop();
	}
}

void easy2d::Spawn::reset()
{
	Action::reset();
	for (auto action : _actions)
	{
		action->reset();
	}
}

void easy2d::Spawn::_resetTime()
{
	for (auto action : _actions)
	{
		action->_resetTime();
	}
}

void easy2d::Spawn::add(Action * action)
{
	if (action)
	{
		_actions.push_back(action);
		action->retain();
	}
}

void easy2d::Spawn::add(const std::vector<Action*>& actions)
{
	for (const auto &action : actions)
	{
		this->add(action);
	}
}

easy2d::Spawn * easy2d::Spawn::clone() const
{
	auto spawn = Create<Spawn>();
	for (const auto& action : _actions)
	{
		if (action)
		{
			spawn->add(action->clone());
		}
	}
	return spawn;
}

easy2d::Spawn * easy2d::Spawn::reverse() const
{
	auto spawn = Create<Spawn>();
	if (spawn && !_actions.empty())
	{
		std::vector<Action*> newActions(_actions.size());
		for (auto iter = _actions.crbegin(), iterCrend = _actions.crend(); iter != iterCrend; ++iter)
		{
			if (*iter)
			{
				newActions.push_back((*iter)->reverse());
			}
		}
		spawn->add(newActions);
	}
	return spawn;
}