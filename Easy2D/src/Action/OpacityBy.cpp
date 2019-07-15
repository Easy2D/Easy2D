#include <e2daction.h>
#include <e2dnode.h>


easy2d::OpacityBy::OpacityBy(float duration, float opacity)
	: FiniteTimeAction(duration)
{
	_deltaVal = opacity;
}

void easy2d::OpacityBy::_init()
{
	FiniteTimeAction::_init();

	if (_target)
	{
		_startVal = _target->getOpacity();
	}
}

void easy2d::OpacityBy::_update()
{
	FiniteTimeAction::_update();

	if (_target)
	{
		_target->setOpacity(_startVal + _deltaVal * _delta);
	}
}

easy2d::OpacityBy * easy2d::OpacityBy::clone() const
{
	return gcnew OpacityBy(_duration, _deltaVal);
}

easy2d::OpacityBy * easy2d::OpacityBy::reverse() const
{
	return gcnew OpacityBy(_duration, -_deltaVal);
}