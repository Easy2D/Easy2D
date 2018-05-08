#include "..\e2daction.h"

e2d::ActionGradual::ActionGradual(double duration)
	: _fRateOfProgress(0)
{
	_fDuration = max(duration, 0);
}

void e2d::ActionGradual::_init()
{
	ActionBase::_init();
}

void e2d::ActionGradual::_update()
{
	ActionBase::_update();
	// �ж�ʱ�����Ƿ��㹻
	if (_fDuration == 0)
	{
		_fRateOfProgress = 1;
		this->stop();
		return;
	}
	// ���㶯������
	_fRateOfProgress = min((Time::getTotalTime() - _fLast) / _fDuration, 1);
	// �ж϶����Ƿ����
	if (_fRateOfProgress >= 1)
	{
		this->stop();
	}
}
