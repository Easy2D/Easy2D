#include <e2daction.h>
#include <e2dnode.h>

easy2d::JumpTo::JumpTo(double duration, const Point & pos, double height, int jumps)
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
