#include <e2dcommon.h>

easy2d::Listener::Listener()
	: _name()
	, _callback()
	, _running(true)
	, _done(false)
{
}

easy2d::Listener::Listener(const Callback& func, const String & name, bool paused)
	: _name(name)
	, _callback(func)
	, _running(!paused)
	, _done(false)
{
}

void easy2d::Listener::handle(Event* evt)
{
	if (_callback && _running)
	{
		_callback(evt);
	}
}

bool easy2d::Listener::isRunning() const
{
	return _running;
}

easy2d::String easy2d::Listener::getName() const
{
	return _name;
}

void easy2d::Listener::setName(const String & name)
{
	_name = name;
}

void easy2d::Listener::setCallback(const Callback& func)
{
	_callback = func;
}

void easy2d::Listener::done()
{
	_done = true;
}

bool easy2d::Listener::isDone()
{
	return _done;
}

void easy2d::Listener::start()
{
	_running = true;
}

void easy2d::Listener::stop()
{
	_running = false;
}
