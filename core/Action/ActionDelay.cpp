#include "..\eactions.h"

e2d::EActionDelay::EActionDelay(float duration)
{
	m_fDelayTime = max(duration, 0);
}

e2d::EActionDelay * e2d::EActionDelay::clone() const
{
	return new EActionDelay(m_fDelayTime);
}

void e2d::EActionDelay::_init()
{
	EAction::_init();
}

void e2d::EActionDelay::_update()
{
	EAction::_update();
	// �ж�ʱ�����Ƿ��㹻
	if ((ETime::getTotalTime() - m_fLast) >= m_fDelayTime)
	{
		this->stop();
	}
}
