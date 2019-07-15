#include <e2daction.h>

easy2d::Delay::Delay(double duration)
	: _delta(0)
	, _delay(max(duration, 0))
{
}

easy2d::Delay * easy2d::Delay::clone() const
{
	return gcnew Delay(_delay);
}

easy2d::Delay * easy2d::Delay::reverse() const
{
	return gcnew Delay(_delay);
}

void easy2d::Delay::reset()
{
	Action::reset();
	_delta = 0;
}

void easy2d::Delay::_init()
{
	Action::_init();
}

void easy2d::Delay::_update()
{
	Action::_update();

	_delta = Time::getTotalTime() - _last;

	if (_delta >= _delay)
	{
		this->stop();
	}
}

void easy2d::Delay::_resetTime()
{
	Action::_resetTime();
	_last = Time::getTotalTime() - _delta;
}
