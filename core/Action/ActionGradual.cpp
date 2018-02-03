#include "..\eactions.h"

e2d::EActionGradual::EActionGradual(float duration)
	: m_fRateOfProgress(0)
{
	m_fDuration = max(duration, 0);
}

void e2d::EActionGradual::_init()
{
	EAction::_init();
}

void e2d::EActionGradual::_update()
{
	EAction::_update();
	// �ж�ʱ�����Ƿ��㹻
	if (m_fDuration == 0)
	{
		m_fRateOfProgress = 1;
		this->stop();
		return;
	}
	// ���㶯������
	m_fRateOfProgress = min((ETime::getTotalTime() - m_fLast) / m_fDuration, 1);
	// �ж϶����Ƿ����
	if (m_fRateOfProgress >= 1)
	{
		this->stop();
	}
}

void e2d::EActionGradual::_reset()
{
	EAction::_reset();
}
