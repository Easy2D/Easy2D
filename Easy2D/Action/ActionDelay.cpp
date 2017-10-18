#include "..\eactions.h"
#include "..\Win\winbase.h"

e2d::ActionDelay::ActionDelay(float duration)
{
	setInterval(LONGLONG(duration * 1000));
}

e2d::ActionDelay::~ActionDelay()
{
}

e2d::ActionDelay * e2d::ActionDelay::copy() const
{
	return new ActionDelay(m_nAnimationInterval / 1000.0f);
}

void e2d::ActionDelay::_init()
{
	EAction::_init();
	// ��¼��ǰʱ��
	m_nLast = GetNow();
}

void e2d::ActionDelay::_exec()
{
	// �ж�ʱ�����Ƿ��㹻
	if (GetInterval(m_nLast) > m_nAnimationInterval)
	{
		this->stop();
	}
}

void e2d::ActionDelay::_reset()
{
	EAction::_reset();
	// ��¼��ǰʱ��
	m_nLast = GetNow();
}
