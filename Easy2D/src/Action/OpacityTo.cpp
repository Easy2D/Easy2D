#include <e2daction.h>
#include <e2dnode.h>


easy2d::OpacityTo::OpacityTo(float duration, float opacity)
	: OpacityBy(duration, 0)
{
	_endVal = opacity;
}

easy2d::OpacityTo * easy2d::OpacityTo::clone() const
{
	return gcnew OpacityTo(_duration, _endVal);
}

void easy2d::OpacityTo::_init()
{
	OpacityBy::_init();
	_deltaVal = _endVal - _startVal;
}
