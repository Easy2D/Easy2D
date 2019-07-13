#include <e2dcommon.h>
#include <e2dtool.h>


easy2d::Listener::Listener()
	: _name()
	, _callback()
	, _running(true)
	, _stopped(false)
{
}

easy2d::Listener::Listener(const Function<void()> & func, const String & name, bool paused)
	: _name(name)
	, _callback(func)
	, _running(!paused)
	, _stopped(false)
{
}

void easy2d::Listener::_update()
{
	if (_callback)
	{
		_callback();
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

void easy2d::Listener::setFunc(const Function<void()> & func)
{
	_callback = func;
}

void easy2d::Listener::start()
{
	_running = true;
}

void easy2d::Listener::stop()
{
	_running = false;
}
