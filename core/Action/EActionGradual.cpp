#include "..\eactions.h"
#include "..\Win\winbase.h"

e2d::EActionGradual::EActionGradual(float duration)
	: m_fRateOfProgress(0)
	, m_fDuration(0)
	, m_fTotalDuration(duration * 1000)
{
}

bool e2d::EActionGradual::_isEnd() const
{
	return m_fDuration >= m_fTotalDuration;
}

void e2d::EActionGradual::_init()
{
	EAction::_init();
}
#include <iostream>
bool e2d::EActionGradual::_isDelayEnough()
{
	// �ж�ʱ�����Ƿ��㹻
	if (m_fTotalDuration == 0)
	{
		m_fRateOfProgress = 1;
		return true;
	}

	if (IsIntervalFull(m_tLast, m_nAnimationInterval))
	{
		// ���¼�¼ʱ��
		m_tLast.QuadPart += m_nAnimationInterval.QuadPart;
		m_fDuration += static_cast<float>(ToMilliseconds(m_nAnimationInterval.QuadPart));
		std::cout << ToMilliseconds(m_nAnimationInterval.QuadPart) << std::endl;
		// ���㶯������
		m_fRateOfProgress = m_fDuration / m_fTotalDuration;
		return true;
	}
	return false;
}

void e2d::EActionGradual::_reset()
{
	EAction::_reset();
	m_fDuration = 0;
}
