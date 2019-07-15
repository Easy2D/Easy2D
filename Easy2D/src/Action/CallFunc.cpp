#include <e2daction.h>

easy2d::CallFunc::CallFunc(const Function<void()>& func) :
	_func(func)
{
}

easy2d::CallFunc * easy2d::CallFunc::clone() const
{
	return gcnew CallFunc(_func);
}

easy2d::CallFunc * easy2d::CallFunc::reverse() const
{
	return gcnew CallFunc(_func);
}

void easy2d::CallFunc::_init()
{
}

void easy2d::CallFunc::_update()
{
	_func();
	this->stop();
}
