#include <e2daction.h>
#include <e2dmanager.h>

easy2d::Loop::Loop(Action * action, int times /* = -1 */)
	: _action(action)
	, _times(0)
	, _totalTimes(times)
{
	if (action == nullptr) E2D_WARNING(L"Loop NULL pointer exception!");

	if (action)
	{
		_action = action;
		_action->retain();
	}
}

easy2d::Loop::~Loop()
{
}

easy2d::Loop * easy2d::Loop::clone() const
{
	if (_action)
	{
		return Create<Loop>(_action->clone());
	}
	else
	{
		return nullptr;
	}
}

easy2d::Loop * easy2d::Loop::reverse() const
{
	if (_action)
	{
		return Create<Loop>(_action->clone());
	}
	else
	{
		return nullptr;
	}
}

void easy2d::Loop::_init()
{
	Action::_init();

	if (_action)
	{
		_action->_target = _target;
		_action->_init();
	}
}

void easy2d::Loop::_update()
{
	Action::_update();

	if (_times == _totalTimes)
	{
		this->stop();
		return;
	}

	if (_action)
	{
		_action->_update();

		if (_action->_isDone())
		{
			++_times;

			Action::reset();
			_action->reset();
		}
	}
	else
	{
		this->stop();
	}
}

void easy2d::Loop::reset()
{
	Action::reset();

	if (_action) _action->reset();
	_times = 0;
}

void easy2d::Loop::onDestroy()
{
	Action::onDestroy();
	GC::release(_action);
}

void easy2d::Loop::_resetTime()
{
	if (_action) _action->_resetTime();
}
