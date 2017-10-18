#include "..\eactions.h"
#include "..\Win\winbase.h"

e2d::Animation::Animation(float duration)
{
	m_nDuration = 0;
	m_nTotalDuration = UINT(duration * 1000);
}

e2d::Animation::~Animation()
{
}

bool e2d::Animation::_isEnd() const
{
	return m_nDuration >= m_nTotalDuration;
}

void e2d::Animation::_init()
{
	EAction::_init();
	// ��¼��ǰʱ��
	m_nLast = GetNow();
}

bool e2d::Animation::_isDelayEnough()
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

void e2d::Animation::_reset()
{
	EAction::_reset();
	m_nDuration = 0;
	// ��¼��ǰʱ��
	m_nLast = GetNow();
}
