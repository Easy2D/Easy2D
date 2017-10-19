#include "..\eactions.h"
#include "..\Win\winbase.h"

e2d::EAnimation::EAnimation(float duration)
{
	m_nDuration = 0;
	m_nTotalDuration = UINT(duration * 1000);
}

e2d::EAnimation::~EAnimation()
{
}

bool e2d::EAnimation::_isEnd() const
{
	return m_nDuration >= m_nTotalDuration;
}

void e2d::EAnimation::_init()
{
	EAction::_init();
	// ��¼��ǰʱ��
	m_nLast = GetNow();
}

bool e2d::EAnimation::_isDelayEnough()
{
	// �ж�ʱ�����Ƿ��㹻
	if (GetInterval(m_nLast) > m_nAnimationInterval)
	{
		// ���¼�¼ʱ��
		m_nLast += milliseconds(m_nAnimationInterval);
		m_nDuration += m_nAnimationInterval;
		return true;
	}
	return false;
}

void e2d::EAnimation::_reset()
{
	EAction::_reset();
	m_nDuration = 0;
	// ��¼��ǰʱ��
	m_nLast = GetNow();
}
