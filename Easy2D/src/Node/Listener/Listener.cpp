#include <easy2d/e2dlistener.h>
#include <easy2d/e2dbase.h>

easy2d::ListenerBase::ListenerBase()
	: _running(true)
	, _done(false)
	, _ignoreGamePaused(false)
	, _name(nullptr)
{
}

easy2d::ListenerBase::~ListenerBase()
{
	if (_name)
	{
		delete _name;
		_name = nullptr;
	}
}

void easy2d::ListenerBase::receive(Node* target, Event* evt)
{
	if (!_ignoreGamePaused && Game::isPaused())
		return;

	if (_running && !_done)
	{
		handle(target, evt);
	}
}

bool easy2d::ListenerBase::isRunning() const
{
	return _running;
}

easy2d::String easy2d::ListenerBase::getName() const
{
	if (_name)
	{
		return *_name;
	}
	return String();
}

void easy2d::ListenerBase::setName(const String & name)
{
	if (!name.empty())
	{
		if (_name)
		{
			*_name = name;
		}
		else
		{
			_name = new String(name);
		}
	}
	else if (_name)
	{
		delete _name;
		_name = nullptr;
	}
}

void easy2d::ListenerBase::done()
{
	_done = true;
}

bool easy2d::ListenerBase::isDone()
{
	return _done;
}

void easy2d::ListenerBase::start()
{
	_running = true;
}

void easy2d::ListenerBase::stop()
{
	_running = false;
}

void easy2d::ListenerBase::ignoreGamePaused()
{
	_ignoreGamePaused = true;
}

easy2d::Listener::Listener()
	: ListenerBase()
	, _callback()
{
}

easy2d::Listener::Listener(const Callback& func, const String& name, bool paused)
	: ListenerBase()
	, _callback(func)
{
	setName(name);
	if (paused)
	{
		stop();
	}
}

void easy2d::Listener::setCallback(const Callback& func)
{
	_callback = func;
}

void easy2d::Listener::handle(Node* target, Event* evt)
{
	if (_callback)
	{
		_callback(evt);
	}
}
