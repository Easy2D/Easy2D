#include <e2daction.h>
#include <e2dnode.h>

easy2d::ScaleTo::ScaleTo(double duration, double scale)
	: ScaleBy(duration, 0, 0)
{
	_endScaleX = scale;
	_endScaleY = scale;
}

easy2d::ScaleTo::ScaleTo(double duration, double scaleX, double scaleY)
	: ScaleBy(duration, 0, 0)
{
	_endScaleX = scaleX;
	_endScaleY = scaleY;
}

easy2d::ScaleTo * easy2d::ScaleTo::clone() const
{
	return Create<ScaleTo>(_duration, _endScaleX, _endScaleY);
}

void easy2d::ScaleTo::_init()
{
	ScaleBy::_init();
	_deltaX = _endScaleX - _startScaleX;
	_deltaY = _endScaleY - _startScaleY;
}
