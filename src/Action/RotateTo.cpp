#include <easy2d/e2daction.h>
#include <easy2d/e2dnode.h>


easy2d::RotateTo::RotateTo(float duration, float rotation)
	: RotateBy(duration, 0)
{
	_endVal = rotation;
}

easy2d::RotateTo * easy2d::RotateTo::clone() const
{
	return gcnew RotateTo(_duration, _endVal);
}

void easy2d::RotateTo::_init()
{
	RotateBy::_init();
	_deltaVal = _endVal - _startVal;
}
