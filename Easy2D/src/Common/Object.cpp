#include <easy2d/e2dbase.h>
#include <easy2d/e2dmanager.h>


easy2d::Object::Object()
	: _refCount(1)
{
	// 构造对象时，引用计数置 1
}

easy2d::Object::~Object()
{
}

void easy2d::Object::autorelease()
{
	GC::trace(this);
}

void easy2d::Object::retain()
{
	++_refCount;
}

void easy2d::Object::release()
{
	_refCount--;

	if (_refCount == 0)
	{
		delete this;
	}
}

int easy2d::Object::getRefCount() const
{
	return _refCount;
}