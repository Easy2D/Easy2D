#include <easy2d/e2daction.h>
#include <easy2d/e2dnode.h>

easy2d::JumpTo::JumpTo(float duration, const Point & pos, float height, int jumps)
	: JumpBy(duration, Point(), height, jumps)
	, _endPos(pos)
{
}

easy2d::JumpTo * easy2d::JumpTo::clone() const
{
	return gcnew JumpTo(_duration, _endPos, _height, _jumps);
}

void easy2d::JumpTo::_init()
{
	JumpBy::_init();
	_deltaPos = _endPos - _startPos;
}
