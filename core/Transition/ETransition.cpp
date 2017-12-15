#include "..\etransitions.h"
#include "..\Win\winbase.h"

e2d::ETransition::ETransition(float duration)
	: m_bEnd(false)
	, m_fTotalDuration(duration * 1000)
	, m_fDuration(0)
	, m_fRateOfProgress(0)
	, m_pPrevScene(nullptr)
	, m_pNextScene(nullptr)
{
}

bool e2d::ETransition::isEnding()
{
	return m_bEnd;
}

bool e2d::ETransition::_isDelayEnough()
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
		// ���㶯������
		m_fRateOfProgress = m_fDuration / m_fTotalDuration;
		return true;
	}
	return false;
}

void e2d::ETransition::_stop()
{
	m_bEnd = true;
	_reset();
}

void e2d::ETransition::_setTarget(EScene * prev, EScene * next)
{
	m_tLast = GetNow();
	m_pPrevScene = prev;
	m_pNextScene = next;
	_init();
}
