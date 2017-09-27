#include "..\easy2d.h"

Animation::Animation(float duration)
{
	m_nDuration = 0;
	m_nTotalDuration = UINT(duration * 1000);
}

Animation::~Animation()
{
}

void Animation::_init()
{
	// ��¼��ǰʱ��
	QueryPerformanceCounter(&m_nLast);
}

bool Animation::_exec(LARGE_INTEGER nNow)
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
	m_nDuration = 0;
	// ���¼�¼��ǰʱ��
	QueryPerformanceCounter(&m_nLast);
}
