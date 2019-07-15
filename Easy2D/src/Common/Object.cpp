#include <e2dbase.h>
#include <e2dmanager.h>


easy2d::Object::Object()
	: _refCount(0)
{
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
	GC::notify();
}

int easy2d::Object::getRefCount() const
{
	return _refCount;
}