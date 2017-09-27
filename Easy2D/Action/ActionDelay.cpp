#include "..\easy2d.h"

ActionDelay::ActionDelay(float duration)
{
	setInterval(UINT(duration * 1000));
}

ActionDelay::~ActionDelay()
{
}

ActionDelay * easy2d::ActionDelay::copy()
{
	return new ActionDelay(*this);
}

void ActionDelay::_init()
{
	// ��¼��ǰʱ��
	QueryPerformanceCounter(&m_nLast);
}

bool ActionDelay::_exec(LARGE_INTEGER nNow)
{
	if (m_bStop) return true;
	if (!m_bRunning) return false;

	// �ж�ʱ�����Ƿ��㹻
	if (nNow.QuadPart - m_nLast.QuadPart > m_nAnimationInterval.QuadPart)
	{
		return true;
	}
	return false;
}

void ActionDelay::_reset()
{
	// ���¼�¼��ǰʱ��
	QueryPerformanceCounter(&m_nLast);
}
