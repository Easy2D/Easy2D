#include <e2daction.h>
#include <e2dnode.h>


easy2d::RotateTo::RotateTo(double duration, double rotation)
	: RotateBy(duration, 0)
{
	_endVal = rotation;
}

easy2d::RotateTo * easy2d::RotateTo::clone() const
{
	return Create<RotateTo>(_duration, _endVal);
}

void easy2d::RotateTo::_init()
{
	RotateBy::_init();
	_deltaVal = _endVal - _startVal;
}
