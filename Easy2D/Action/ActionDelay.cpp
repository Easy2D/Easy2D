#include "..\easy2d.h"

ActionDelay::ActionDelay(float duration)
{
	setInterval(UINT(duration * 1000));
}

ActionDelay::~ActionDelay()
{
}

ActionDelay * ActionDelay::copy() const
{
	return new ActionDelay(m_nMilliSeconds / 1000.0f);
}

void ActionDelay::_init()
{
	// ��¼��ǰʱ��
	QueryPerformanceCounter(&m_nLast);
}

void ActionDelay::_exec(LARGE_INTEGER nNow)
{
	// �ж�ʱ�����Ƿ��㹻
	if (nNow.QuadPart - m_nLast.QuadPart > m_nAnimationInterval.QuadPart)
	{
		this->stop();
	}
}

void ActionDelay::_reset()
{
	Action::_reset();
	// ���¼�¼��ǰʱ��
	QueryPerformanceCounter(&m_nLast);
}
