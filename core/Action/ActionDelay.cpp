#include "..\e2daction.h"

e2d::ActionDelay::ActionDelay(double duration)
{
	_fDelayTime = max(duration, 0);
}

e2d::ActionDelay * e2d::ActionDelay::clone() const
{
	return new ActionDelay(_fDelayTime);
}

void e2d::ActionDelay::_init()
{
	ActionBase::_init();
}

void e2d::ActionDelay::_update()
{
	ActionBase::_update();
	// �ж�ʱ�����Ƿ��㹻
	if ((Time::getTotalTime() - _fLast) >= _fDelayTime)
	{
		this->stop();
	}
}
