#include "..\e2daction.h"

e2d::Sequence::Sequence()
	: _currIndex(0)
{
}

#ifdef HIGHER_THAN_VS2012
e2d::Sequence::Sequence(const std::initializer_list<Action*>& vActions)
	: _currIndex(0)
{
	this->add(vActions);
}
#else
e2d::Sequence::Sequence(int number, Action * action1, ...) :
	_currIndex(0)
{
	Action ** ppAction = &action1;

	while (number > 0)
	{
		WARN_IF((*ppAction) == nullptr, "Sequence NULL pointer exception!");
		this->add(*ppAction);
		ppAction++;
		number--;
	}
}
#endif

e2d::Sequence::~Sequence()
{
}

void e2d::Sequence::_init()
{
	Action::_init();
	// �����ж�����Ŀ���
	if (_target)
	{
		for (auto action : _actions)
		{
			action->_target = _target;
		}
	}
	// ��ʼ����һ������
	_actions[0]->_init();
}

void e2d::Sequence::onDestroy()
{
	Action::onDestroy();
	for (auto action : _actions)
	{
		SafeRelease(&action);
	}
}

void e2d::Sequence::_update()
{
	Action::_update();

	auto &action = _actions[_currIndex];
	action->_update();

	if (action->_isDone())
	{
		_currIndex++;
		if (_currIndex == _actions.size())
		{
			this->stop();
		}
		else
		{
			_actions[_currIndex]->_init();
		}
	}
}

void e2d::Sequence::reset()
{
	Action::reset();
	for (auto action : _actions)
	{
		action->reset();
	}
	_currIndex = 0;
}

void e2d::Sequence::_resetTime()
{
	for (auto action : _actions)
	{
		action->_resetTime();
	}
}

void e2d::Sequence::add(Action * action)
{
	if (action)
	{
		_actions.push_back(action);
		action->retain();
	}
}

#ifdef HIGHER_THAN_VS2012
void e2d::Sequence::add(const std::initializer_list<Action*>& vActions)
{
	for (const auto &action : vActions)
	{
		this->add(action);
	}
}
#else
void e2d::Sequence::add(int number, Action * action, ...)
{
	Action ** ppAction = &action;

	while (number > 0)
	{
		WARN_IF((*ppAction) == nullptr, "Sequence NULL pointer exception!");
		this->add(*ppAction);
		ppAction++;
		number--;
	}
}
#endif

e2d::Sequence * e2d::Sequence::clone() const
{
	auto a = new Sequence();
	for (auto action : _actions)
	{
		a->add(action->clone());
	}
	return a;
}

e2d::Sequence * e2d::Sequence::reverse() const
{
	auto a = new Sequence();
	for (auto action : _actions)
	{
		a->add(action->reverse());
	}
	// ������˳����������
	a->_actions.reserve(_actions.size());
	return a;
}