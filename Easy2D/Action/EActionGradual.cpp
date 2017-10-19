#include "..\eactions.h"
#include "..\Win\winbase.h"

e2d::EActionGradual::EActionGradual(float duration)
{
	m_nDuration = 0;
	m_nTotalDuration = UINT(duration * 1000);
}

bool e2d::EActionGradual::_isEnd() const
{
	return m_nDuration >= m_nTotalDuration;
}

void e2d::EActionGradual::_init()
{
	EAction::_init();
	// ��¼��ǰʱ��
	m_tLast = GetNow();
}

bool e2d::EActionGradual::_isDelayEnough()
{
	// �ж�ʱ�����Ƿ��㹻
	if (GetInterval(m_tLast) > m_nAnimationInterval)
	{
		// ���¼�¼ʱ��
		m_tLast += milliseconds(m_nAnimationInterval);
		m_nDuration += m_nAnimationInterval;
		return true;
	}
	return false;
}

void e2d::EActionGradual::_reset()
{
	EAction::_reset();
	m_nDuration = 0;
	// ��¼��ǰʱ��
	m_tLast = GetNow();
}
