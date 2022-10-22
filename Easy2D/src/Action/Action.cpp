#include <easy2d/e2daction.h>
#include <easy2d/e2dmanager.h>
#include <easy2d/e2dnode.h>

easy2d::Action::Action() 
	: _running(false)
	, _done(false)
	, _initialized(false)
	, _removeTarget(false)
	, _target(nullptr)
	, _last(0)
{
}

easy2d::Action::~Action()
{
}

bool easy2d::Action::isRunning()
{
	return _running;
}

void easy2d::Action::resume()
{
	_running = true;
	_last = Time::getTotalTime();
}

void easy2d::Action::pause()
{
	_running = false;
}

void easy2d::Action::stop()
{
	_done = true;
	if (_removeTarget)
	{
		_target->removeFromParent();
		_target = nullptr;
	}
}

easy2d::Node * easy2d::Action::getTarget() const
{
	return _target;
}

void easy2d::Action::removeTargetWhenDone()
{
	_removeTarget = true;
}

void easy2d::Action::reset()
{
	_initialized = false;
	_done = false;
	_last = Time::getTotalTime();
}

bool easy2d::Action::_isDone()
{
	return _done;
}

void easy2d::Action::_startWithTarget(Node* target)
{
	_target = target;
	_running = true;
	this->reset();
}

void easy2d::Action::_init()
{
	_initialized = true;
	_last = Time::getTotalTime();
}

void easy2d::Action::_update()
{
	if (!_initialized)
	{
		_init();
	}
}

void easy2d::Action::_resetTime()
{
}
