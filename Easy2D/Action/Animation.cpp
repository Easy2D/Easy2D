#include "..\easy2d.h"

Animation::Animation(float duration)
{
	m_nDuration = 0;
	m_nTotalDuration = UINT(duration * 1000);
}

Animation::~Animation()
{
}

bool Animation::_isEnd() const
{
	return m_nDuration >= m_nTotalDuration;
}

void Animation::_init()
{
	Action::_init();
	// ��¼��ǰʱ��
	QueryPerformanceCounter(&m_nLast);
}

bool Animation::_isDelayEnough(LARGE_INTEGER nNow)
{
	// �ж�ʱ�����Ƿ��㹻
	if (nNow.QuadPart - m_nLast.QuadPart > m_nAnimationInterval.QuadPart)
	{
		// ������ķ������¼�¼ʱ��
		m_nLast.QuadPart = nNow.QuadPart - (nNow.QuadPart % m_nAnimationInterval.QuadPart);
		m_nDuration += m_nMilliSeconds;
		return true;
	}
	return false;
}

void Animation::_reset()
{
	Action::_reset();
	m_nDuration = 0;
	// ���¼�¼��ǰʱ��
	QueryPerformanceCounter(&m_nLast);
}
