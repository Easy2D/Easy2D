#include "..\eactions.h"
#include "..\Win\winbase.h"

e2d::EActionDelay::EActionDelay(float duration)
{
	setInterval(LONGLONG(duration * 1000));
}

e2d::EActionDelay * e2d::EActionDelay::clone() const
{
	return new EActionDelay(m_nAnimationInterval / 1000.0f);
}

void e2d::EActionDelay::_init()
{
	EAction::_init();
	// ��¼��ǰʱ��
	m_tLast = GetNow();
}

void e2d::EActionDelay::_callOn()
{
	// �ж�ʱ�����Ƿ��㹻
	if (GetInterval(m_tLast) > m_nAnimationInterval)
	{
		this->stop();
	}
}

void e2d::EActionDelay::_reset()
{
	EAction::_reset();
	// ��¼��ǰʱ��
	m_tLast = GetNow();
}
