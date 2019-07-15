#include <e2daction.h>
#include <e2dnode.h>

easy2d::ScaleTo::ScaleTo(float duration, float scale)
	: ScaleBy(duration, 0, 0)
{
	_endScaleX = scale;
	_endScaleY = scale;
}

easy2d::ScaleTo::ScaleTo(float duration, float scaleX, float scaleY)
	: ScaleBy(duration, 0, 0)
{
	_endScaleX = scaleX;
	_endScaleY = scaleY;
}

easy2d::ScaleTo * easy2d::ScaleTo::clone() const
{
	return gcnew ScaleTo(_duration, _endScaleX, _endScaleY);
}

void easy2d::ScaleTo::_init()
{
	ScaleBy::_init();
	_deltaX = _endScaleX - _startScaleX;
	_deltaY = _endScaleY - _startScaleY;
}
