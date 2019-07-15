#include <e2daction.h>
#include <e2dnode.h>


easy2d::ScaleBy::ScaleBy(double duration, double scale)
	: FiniteTimeAction(duration)
{
	_deltaX = scale;
	_deltaY = scale;
}

easy2d::ScaleBy::ScaleBy(double duration, double scaleX, double scaleY)
	: FiniteTimeAction(duration)
{
	_deltaX = scaleX;
	_deltaY = scaleY;
}

void easy2d::ScaleBy::_init()
{
	FiniteTimeAction::_init();

	if (_target)
	{
		_startScaleX = _target->getScaleX();
		_startScaleY = _target->getScaleY();
	}
}

void easy2d::ScaleBy::_update()
{
	FiniteTimeAction::_update();

	if (_target)
	{
		_target->setScale(_startScaleX + _deltaX * _delta, _startScaleY + _deltaY * _delta);
	}
}

easy2d::ScaleBy * easy2d::ScaleBy::clone() const
{
	return gcnew ScaleBy(_duration, _deltaX, _deltaY);
}

easy2d::ScaleBy * easy2d::ScaleBy::reverse() const
{
	return gcnew ScaleBy(_duration, -_deltaX, -_deltaY);
}