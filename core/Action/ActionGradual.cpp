#include "..\e2daction.h"

e2d::ActionGradual::ActionGradual(double duration)
	: m_fRateOfProgress(0)
{
	m_fDuration = max(duration, 0);
}

void e2d::ActionGradual::_init()
{
	ActionBase::_init();
}

void e2d::ActionGradual::_update()
{
	ActionBase::_update();
	// �ж�ʱ�����Ƿ��㹻
	if (m_fDuration == 0)
	{
		m_fRateOfProgress = 1;
		this->stop();
		return;
	}
	// ���㶯������
	m_fRateOfProgress = min((Time::getTotalTime() - m_fLast) / m_fDuration, 1);
	// �ж϶����Ƿ����
	if (m_fRateOfProgress >= 1)
	{
		this->stop();
	}
}
