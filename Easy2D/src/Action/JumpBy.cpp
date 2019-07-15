#include <e2daction.h>
#include <e2dnode.h>

easy2d::JumpBy::JumpBy(float duration, const Vector2 & vec, float height, int jumps)
	: FiniteTimeAction(duration)
	, _deltaPos(vec)
	, _height(height)
	, _jumps(jumps)
{
}

easy2d::JumpBy * easy2d::JumpBy::clone() const
{
	return gcnew JumpBy(_duration, _deltaPos, _height, _jumps);
}

easy2d::JumpBy * easy2d::JumpBy::reverse() const
{
	return gcnew JumpBy(_duration, -_deltaPos, _height, _jumps);
}

void easy2d::JumpBy::_init()
{
	FiniteTimeAction::_init();

	if (_target)
	{
		_prevPos = _startPos = _target->getPos();
	}
}

void easy2d::JumpBy::_update()
{
	FiniteTimeAction::_update();

	if (_target)
	{
		float frac = std::fmodf(_delta * _jumps, 1.0f);
		float x = _deltaPos.x * _delta;
		float y = _height * 4 * frac * (1 - frac);
		y += _deltaPos.y * _delta;

		Point currentPos = _target->getPos();

		Vector2 diff = currentPos - _prevPos;
		_startPos = diff + _startPos;

		Point newPos = _startPos + Vector2(x, y);
		_target->setPos(newPos);

		_prevPos = newPos;
	}
}
