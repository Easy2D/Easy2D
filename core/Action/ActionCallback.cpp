#include "..\eactions.h"

e2d::ActionCallback::ActionCallback(const VoidFunction& callback) :
	m_Callback(callback)
{
}

e2d::ActionCallback * e2d::ActionCallback::clone() const
{
	return new ActionCallback(m_Callback);
}

void e2d::ActionCallback::_init()
{
	// ִ�лص������Ķ�������Ҫ��ʼ��
}

void e2d::ActionCallback::_update()
{
	m_Callback();
	this->stop();
}
