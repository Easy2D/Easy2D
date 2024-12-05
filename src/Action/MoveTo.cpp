#include <easy2d/e2daction.h>
#include <easy2d/e2dnode.h>

easy2d::MoveTo::MoveTo(float duration, Point pos)
	: MoveBy(duration, Vector2())
{
	_endPos = pos;
}

easy2d::MoveTo * easy2d::MoveTo::clone() const
{
	return gcnew MoveTo(_duration, _endPos);
}

void easy2d::MoveTo::_init()
{
	MoveBy::_init();
	_deltaPos = _endPos - _startPos;
}
