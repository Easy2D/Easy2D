#include "..\eactions.h"

e2d::EActionCallback::EActionCallback(const std::function<void()>& callback) :
	m_Callback(callback)
{
}

e2d::EActionCallback * e2d::EActionCallback::clone() const
{
	return new EActionCallback(m_Callback);
}

void e2d::EActionCallback::_init()
{
	// ִ�лص������Ķ�������Ҫ��ʼ��
}

void e2d::EActionCallback::_update()
{
	m_Callback();
	this->stop();
}
