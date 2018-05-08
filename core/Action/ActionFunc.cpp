#include "..\e2daction.h"

e2d::ActionFunc::ActionFunc(const Function& func) :
	_Callback(func)
{
}

e2d::ActionFunc * e2d::ActionFunc::clone() const
{
	return new ActionFunc(_Callback);
}

void e2d::ActionFunc::_init()
{
	// ִ�к�������Ķ�������Ҫ��ʼ��
}

void e2d::ActionFunc::_update()
{
	_Callback();
	this->stop();
}
